/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Derick Rethans
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

/*
 * Example that shows how to convert a date/time in its parts, to a 
 * Unix timestamp.
 *
 * Compile with:
 * gcc -ggdb3 -o date-from-parts date-from-parts.c ../timelib.a -lm
 */

#include <stdio.h>
#include <string.h>
#include "timelib.h"

#define TEST_COUNT 33

timelib_sll tests[TEST_COUNT][2][3] = {
	{ { 2014, 52, 1 }, { 2014, 12, 22 } },
	{ { 2014, 52, 7 }, { 2014, 12, 28 } },
	{ { 2015,  1, 1 }, { 2014, 12, 29 } },
	{ { 2015,  1, 3 }, { 2014, 12, 31 } },
	{ { 2015,  1, 4 }, { 2015,  1,  1 } },
	{ { 2015, 52, 7 }, { 2015, 12, 27 } },
	{ { 2015, 53, 1 }, { 2015, 12, 28 } },
	{ { 2015, 53, 4 }, { 2015, 12, 31 } },
	{ { 2015, 53, 5 }, { 2016,  1,  1 } },
	{ { 2016,  1, 1 }, { 2016,  1,  4 } },
	{ { 2016,  1, 3 }, { 2016,  1,  6 } },
	{ { 2016,  1, 4 }, { 2016,  1,  7 } },
	{ { 2016, 51, 7 }, { 2016, 12, 25 } },
	{ { 2016, 52, 1 }, { 2016, 12, 26 } },
	{ { 2016, 52, 4 }, { 2016, 12, 29 } },
	{ { 2016, 52, 7 }, { 2017,  1,  1 } },
	{ { 2017,  8, 6 }, { 2017,  2, 25 } },
	{ { 2017,  8, 7 }, { 2017,  2, 26 } },
	{ { 2017,  9, 1 }, { 2017,  2, 27 } },
	{ { 2017,  9, 2 }, { 2017,  2, 28 } },
	{ { 2017,  9, 3 }, { 2017,  3,  1 } },
	{ { 2020,  9, 2 }, { 2020,  2, 25 } },
	{ { 2020,  9, 3 }, { 2020,  2, 26 } },
	{ { 2020,  9, 5 }, { 2020,  2, 28 } },
	{ { 2020,  9, 6 }, { 2020,  2, 29 } },
	{ { 2020,  9, 7 }, { 2020,  3,  1 } },
	{ { 2043, 53, 1 }, { 2043, 12, 28 } },
	{ { 2043, 53, 2 }, { 2043, 12, 29 } },
	{ { 2043, 53, 3 }, { 2043, 12, 30 } },
	{ { 2043, 53, 4 }, { 2043, 12, 31 } },
	{ { 2043, 53, 5 }, { 2044,  1,  1 } },
	{ { 2043, 53, 6 }, { 2044,  1,  2 } },
	{ { 2043, 53, 7 }, { 2044,  1,  3 } },
};

int main(int argc, char *argv[])
{
	int i;
	timelib_sll y, m, d;
	char expected[20], actual[20];

	for (i = 0; i < TEST_COUNT; i++) {
		timelib_date_from_isodate(tests[i][0][0], tests[i][0][1], tests[i][0][2], &y, &m, &d);
		snprintf(actual, 19, "%04lld-%02lld-%02lld", y, m, d);
		snprintf(expected, 19, "%04lld-%02lld-%02lld", tests[i][1][0], tests[i][1][1], tests[i][1][2]);

		if (strcmp(actual, expected) != 0) {
			printf("FAIL: ACTUAL(%s) != EXPECTED(%s)\n", actual, expected);
		} else {
			printf("OK:   ACTUAL(%s) == EXPECTED(%s)\n", actual, expected);
		}
	}
}
