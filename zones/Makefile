all: index

clean:
	rm -rf code
	rm timezonedb.idx.php timezonedb.dta timezonedb.idx version-info.txt

checkoutphp:

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

timezonedb.h: timezonedb.idx version-info.txt
	php create_dot_h_file.php

index: timezonedb.h

createpeclfile: index


