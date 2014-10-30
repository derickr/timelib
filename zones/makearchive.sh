#!/bin/bash

echo "Checking out PHP:"
cd /tmp
git clone git@git.php.net:php-src.git tz-tmp || exit
cd -

echo "Unpacking and compiling conversion tool:"
rm -rf code
mkdir code
cd code
tar -xzf ../old*tar.gz
tar -xzf ../tzdata*tar.gz

make

echo " Done"

echo -n "Generating TZ files:"
for i in africa antarctica australasia asia etcetera europe factory northamerica pacificnew southamerica backward; do
	./zic -d data $i
	echo "- $i"
done
cd ..
echo " "

rm -f timezonedb.idx timezonedb.idx.php timezonedb.dta timezonedb.gz
touch timezonedb.idx timezonedb.dta

echo "Building index:"
echo "<?php return array(" >> timezonedb.idx.php
for i in `find code/data -type f | sort`; do
	l=`stat -c "%s" timezonedb.dta`
	j=`echo $i | sed 's/code\/data\///'`
	php create-entry.php $j >> timezonedb.dta
	php -r "echo \"\\t{ \". sprintf('%-36s, 0x%06X', '\"$j\"', $l). \" },\\n\";" >> timezonedb.idx
	php -r "echo \"\\t\". sprintf('%s => 0x%06X', '\"$j\"', $l). \",\\n\";" >> timezonedb.idx.php
	echo "- $j"
done
echo "); ?>" >> timezonedb.idx.php
echo " "
echo "Done"

echo "Sorting index:"
php sort-index.php > timezonedb.idx.tmp
mv timezonedb.idx.tmp timezonedb.idx

echo "Creating .h file:"
echo -n "const timelib_tzdb_index_entry timezonedb_idx_builtin[" > ../timezonedb.h
echo -n `cat timezonedb.idx | wc -l ` >> ../timezonedb.h
echo "] = {" >> ../timezonedb.h
cat timezonedb.idx >> ../timezonedb.h
echo "};" >> ../timezonedb.h

php create_dot_h_file.php >> ../timezonedb.h
echo >> ../timezonedb.h
echo -n "const timelib_tzdb timezonedb_builtin = { \"" >> ../timezonedb.h
version=`find . -name tzdata*.tar.gz | php -r "preg_match('@([0-9]{4})([a-z])@', file_get_contents('php://stdin'), \\\$a); echo \\\$a[1], '.', ord(\\\$a[2]) - ord('a') + 1;"`
tzversion=`find . -name tzdata*.tar.gz | sed 's/.*20/20/' | sed 's/\.tar.*//'`
version="$version$VERSUF"

echo -n $version >> ../timezonedb.h
echo -n "\", " >> ../timezonedb.h
echo -n `cat timezonedb.idx | wc -l ` >> ../timezonedb.h 
echo ", timezonedb_idx_builtin, timelib_timezone_db_data_builtin };" >> ../timezonedb.h
echo " "
echo "Done"

cd /tmp/tz-tmp
git checkout PHP-5.5
cd -

cp ../timezonedb.h /tmp/tz-tmp/ext/date/lib

cd /tmp/tz-tmp
git commit -m "- Updated to version $version ($tzversion)" \
	ext/date/lib/timezonedb.h || exit 5
git checkout PHP-5.6 || exit 5
git merge PHP-5.5 || exit 5
git checkout master || exit 5
git merge PHP-5.6 || exit 5
cd -

cp ../timezonedb.h ~/dev/php/pecl/timezonedb/trunk
cd ~/dev/php/pecl/timezonedb/trunk

d=`date +'%Y-%m-%d'`
cat package.xml | sed "s/version>.*<\/version>/version>${version}<\/version>/" | sed "s/version 20.*/version ${version} (${tzversion})/" | sed "s/date>.*<\/date/date>$d<\/date/" > /tmp/package.xml
mv /tmp/package.xml package.xml

cat php_timezonedb.h | sed "s/\"20[0-1][0-9].*\"/\"${version}\"/" > /tmp/php_timezonedb.h
mv /tmp/php_timezonedb.h php_timezonedb.h

pecl convert

svn commit -m "- Updated to version $version ($tzversion)" timezonedb.c timezonedb.h package.xml package2.xml

pecl package

tag=`echo $version | tr "." "_"`
tag="RELEASE_$tag"

svn cp -m "- Released version $version ($tzversion)" \
	https://svn.php.net/repository/pecl/timezonedb/trunk \
	https://svn.php.net/repository/pecl/timezonedb/tags/$tag

pecl upgrade timezonedb-$version.tgz
cd ~/dev/php/phpdoc/doc-base
php -dextension=timezonedb.so scripts/gen-phpdoc-tz-list.php > ../en/reference/datetime/timezones.xml

cd ../en/reference/datetime/
svn commit -m "- Updated to version $version ($tzversion)" timezones.xml

cd /tmp/tz-tmp
git push origin PHP-5.5 PHP-5.6 master || exit

rm -rf /tmp/tz-tmp
