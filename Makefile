CFLAGS=-O0 -ggdb3 -Wall -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_INTTYPES_H -I.# -DDEBUG_PARSER
LDFLAGS=-lm
CC=gcc

all: parse_date.o tm2unixtime.o unixtime2tm.o dow.o astro.o interval.o \
		tester-parse-string tester-parse-interval \
		tester-create-ts tester-parse-tz \
		tester-render-ts tester-iso-week test-abbr-to-id \
		test-astro Makefile tester-parse-string-by-format \
		tester-diff

#parse_tz: parse_tz.c
#	gcc ${CFLAGS} -c parse_tz parse_tz.c

parse_date.c: timezonemap.h parse_date.re
	re2c -d -b parse_date.re > parse_date.c

parse_iso_intervals.c: parse_iso_intervals.re
	re2c -b parse_iso_intervals.re > parse_iso_intervals.c

timelib.a: parse_iso_intervals.o parse_date.o unixtime2tm.o tm2unixtime.o dow.o parse_tz.o timelib.o astro.o interval.o
	ar -rc timelib.a parse_iso_intervals.o parse_date.o unixtime2tm.o tm2unixtime.o dow.o parse_tz.o timelib.o astro.o interval.o

#parse_date.o: parse_date.c
#	gcc -c parse_date.c -o parse_date.o -DDEBUG_PARSER

tester-diff: timelib.a tester-diff.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-diff tester-diff.c timelib.a

tester-parse-string: timelib.a tester-parse-string.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-parse-string tester-parse-string.c timelib.a

tester-parse-interval: timelib.a tester-parse-interval.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-parse-interval tester-parse-interval.c timelib.a

tester-parse-string-by-format: timelib.a tester-parse-string-by-format.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-parse-string-by-format tester-parse-string-by-format.c timelib.a

tester-create-ts: timelib.a tester-create-ts.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-create-ts tester-create-ts.c timelib.a

tester-parse-tz: timelib.a test-tz-parser.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-parse-tz test-tz-parser.c timelib.a

tester-render-ts: timelib.a tester-render-ts.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-render-ts tester-render-ts.c timelib.a

tester-iso-week: timelib.a tester-iso-week.c
	gcc $(CFLAGS) $(LDFLAGS) -o tester-iso-week tester-iso-week.c timelib.a

test-abbr-to-id: timelib.a test-abbr-to-id.c
	gcc $(CFLAGS) $(LDFLAGS) -o test-abbr-to-id test-abbr-to-id.c timelib.a

test-astro: timelib.a test-astro.c
	gcc $(CFLAGS) $(LDFLAGS) -o test-astro test-astro.c timelib.a -lm

timezonemap.h: gettzmapping.php
	echo Generating timezone mapping file.
	/usr/local/php/5.4dev/bin/php gettzmapping.php > timezonemap.h

clean:
	rm -f parse_iso_intervals.c parse_date.c *.o timelib.a tester-parse-string tester-parse-string-by-format tester-create-ts tester-render-ts parse_date.tar.gz timezonemap.h

test: tester-parse-string tester-create-ts tester-render-ts tester-parse-string-by-format
	php test_all.php

test-parse-string: tester-parse-string
	@for i in tests/*.parse; do echo $$i; php test_parser.php $$i; echo; done

test-parse-format: tester-parse-string-by-format
	@for i in tests/*.parseformat; do echo $$i; php test_parse_format.php $$i; echo; done

test-create-ts: tester-create-ts
	@for i in tests/*.ts; do echo $$i; php test_create.php $$i; echo; done

test-render-ts: tester-render-ts
	@for i in tests/*.render; do echo $$i; php test_render.php $$i; echo; done

package: clean
	tar -cvzf parse_date.tar.gz parse_date.re Makefile tests
