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

int main(int argc, char *argv[])
{
	timelib_time *t1, *t2, *now;
	timelib_rel_time *rt;

	if (argc < 3) {
		printf("Usage:\n\ttester-diff [t1] [t2]\n\tExample: ./tester-create-ts \"9/11\" \"2008-03-26\"\n\n");
		exit(-1);
	}

	t1 = timelib_strtotime(argv[1], strlen(argv[1]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	t2 = timelib_strtotime(argv[2], strlen(argv[2]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	now = timelib_strtotime("1970-01-01 00:00:00 UTC", strlen("1970-01-01 00:00:00 UTC"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_fill_holes(t1, now, TIMELIB_OVERRIDE_TIME);
	timelib_fill_holes(t2, now, TIMELIB_OVERRIDE_TIME);

	timelib_update_ts(t1, t1->tz_info);
	timelib_update_ts(t2, t2->tz_info);

	timelib_dump_date(t1, 1);
	timelib_dump_date(t2, 1);
	rt = timelib_diff(t1, t2);

	timelib_dump_rel_time(rt);

	timelib_time_dtor(t1);
	timelib_time_dtor(t2);
	timelib_time_dtor(now);
	timelib_rel_time_dtor(rt);

	return 0;
}
