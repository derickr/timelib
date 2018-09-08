/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Derick Rethans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "timelib.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int count, i;
	timelib_tzdb *db;
	const timelib_tzdb_index_entry *entries;
	timelib_tzinfo *tzi;

	if (argc > 2) {
		printf("Usage:\n\tenumerate-timezones [zoneinfo path]\n\tExample: ./enumerate-timezone /usr/share/zoneinfo\"\n\n");
		exit(-1);
	}
	if (argc == 1) {
		db = (timelib_tzdb*) timelib_builtin_db();
	} else {
		db = timelib_zoneinfo(argv[1]);
	}

	entries = timelib_timezone_identifiers_list(db, &count);

	for (i = 0; i < count; i++) {
		int error_code;

		tzi = timelib_parse_tzfile(entries[i].id, db, &error_code);
		if (!tzi) {
			printf("FAIL: %s: [%d] %s\n", entries[i].id, error_code, timelib_get_error_message(error_code));
		} else {
			printf("OK:   %s\n", entries[i].id);
			timelib_tzinfo_dtor(tzi);
		}
	}

	if (db != timelib_builtin_db()) {
		timelib_zoneinfo_dtor(db);
	}
}
