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
	signed long long ts;
	timelib_time *t;
	char    *tz;
	timelib_tzinfo  *tzi;

	if (argc < 3) {
		printf("Usage:\n\ttester-render [t] [tz specification]\n\tExample: ./tester-render \"1114819200\" \"Europe/Amsterdam\"\n\n");
		exit(-1);
	}
	ts = atoll(argv[1]);
	tz = argv[2];
	tzi = timelib_parse_tzfile(tz, timelib_builtin_db());

	t = timelib_time_ctor();
	timelib_set_timezone(t, tzi);
	timelib_unixtime2local(t, ts);
	timelib_dump_date(t, 3);
	timelib_tzinfo_dtor(t->tz_info);
	timelib_time_dtor(t);

	return 0;
}
