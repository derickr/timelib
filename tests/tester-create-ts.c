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
#include <string.h>

int main(int argc, char *argv[])
{
	timelib_time *t, *now;
	char    *tz;
	timelib_tzinfo *tzi;
	int dummy_error;

	if (argc < 4) {
		printf("Usage:\n\ttester-create-ts [t] [reference] [tz specification]\n\tExample: ./tester-create-ts \"9/11\" \"00:00:00\" \"Europe/Amsterdam\"\n\n");
		exit(-1);
	}

	t = timelib_strtotime(argv[1], strlen(argv[1]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	now = timelib_strtotime(argv[2], strlen(argv[2]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	tz = argv[3];
	tzi = timelib_parse_tzfile(tz, timelib_builtin_db(), &dummy_error);

	timelib_fill_holes(t, now, TIMELIB_OVERRIDE_TIME);
	timelib_time_dtor(now);
	timelib_update_ts(t, tzi);

	timelib_dump_date(t, 1);
	timelib_time_dtor(t);
	if (tzi) {
		timelib_tzinfo_dtor(tzi);
	}

	return 0;
}
