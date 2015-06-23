all: index

clean:
	rm -rf code
	rm timezonedb.idx.php timezonedb.dta timezonedb.idx version-info.txt
	rm timezonedb-201*.tgz timezonedb.tgz

tzdb:
	rm -rf code
	mkdir code
	tar -C code -xzf tzdata*tar.gz
	tar -C code -xzf tzcode*tar.gz

code/zone.tab: tzdb
	make -C code
	for i in africa antarctica australasia asia etcetera europe factory northamerica pacificnew southamerica backward; do \
		cat code/$$i | ./code/zic -d code/data -; \
		echo "- $$i"; \
	done;

timezonedb.idx.php: code/zone.tab
	php build-idx.php

timezonedb.dta: code/zone.tab
	php build-idx.php

timezonedb.idx: timezonedb.idx.php timezonedb.dta

version-info.txt:
	find . -name tzdata*.tar.gz | sed 's/.*20/20/' | sed 's/\.tar.*//' > version-info.txt
	find . -name tzdata*.tar.gz | php -r "preg_match('@([0-9]{4})([a-z])@', file_get_contents('php://stdin'), \$$a); echo \$$a[1], '.', ord(\$$a[2]) - ord('a') + 1;" >> version-info.txt

timezonedb.h: timezonedb.idx version-info.txt create_dot_h_file.php
	php create_dot_h_file.php

index: timezonedb.h

timezonedb.tgz: index
	cp timezonedb.h ~/dev/php/pecl/timezonedb/trunk
	php update-package-xml.php ~/dev/php/pecl/timezonedb/trunk
	php update-package-version.php ~/dev/php/pecl/timezonedb/trunk
	pecl package ~/dev/php/pecl/timezonedb/trunk/package.xml
	cp timezonedb-`cat version-info.txt | tail -n 1`.tgz timezonedb.tgz

release-pecl: timezonedb.tgz
	cd ~/dev/php/pecl/timezonedb/trunk
	svn commit -m "- Updated to version "`cat version-info.txt | tail -n 1`" ("cat version-info.txt | head -n 1`")" timezonedb.c timezonedb.h package.xml package2.xml
	cd -

release-docs: timezonedb.tgz
	pecl upgrade -f timezonedb.tgz
	cd ~/dev/php/phpdoc/en/reference/datetime/
	-php ../../../doc-base/scripts/gen-phpdoc-tz-list.php > timezones.xml
	echo svn commit -m "- Updated to version "`cat version-info.txt | tail -n 1`" ("`cat version-info.txt | head -n 1`")" timezones.xml
	cd -
