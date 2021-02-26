/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 MongoDB, Inc.
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
#include "timelib_private.h"

/* "<" [+-]? .+? ">" */
char *read_description_numeric_abbr(char **ptr)
{
	const char *begin = *ptr + 1;

	/* skip '<' */
	(*ptr)++;

	while (**ptr != '\0' && **ptr != '>') {
		(*ptr)++;
	}

	if (**ptr == '\0') {
		return NULL;
	}

	if (**ptr == '>') {
		(*ptr)++;
	}

	// Abbreviation may not be empty
	if (*ptr - begin - 1 < 1) {
		return NULL;
	}

	return timelib_strndup(begin, *ptr - begin - 1);
}

/* [A-Z]+ */
char *read_description_abbr(char **ptr)
{
	const char *begin = *ptr;

	// Find the end
	while (**ptr >= 'A' && **ptr <= 'Z') {
		(*ptr)++;
	}

	// Abbreviation may not be empty
	if (*ptr - begin < 1) {
		return NULL;
	}

	return timelib_strndup(begin, *ptr - begin);
}

/* "<" [+-]? .+? ">" | [A-Z]+ */
char *read_description(char **ptr)
{
	if (**ptr == '<') {
		return read_description_numeric_abbr(ptr);
	} else {
		return read_description_abbr(ptr);
	}
}

/* [+-]? */
int read_sign(char **ptr)
{
	int bias = 1;

	if (**ptr == '+') {
		(*ptr)++;
	} else if (**ptr == '-') {
		bias = -1;
		(*ptr)++;
	}

	return bias;
}

/* [+-]? [0-9]+ ( ":" [0-9]+ ( ":" [0-9]+ )? )? */
timelib_sll read_offset(char **ptr)
{
	const char *begin;
	int bias = read_sign(ptr);
	int hours = 0;
	int minutes = 0;
	int seconds = 0;

	begin = *ptr;

	// skip leading 0's
	while (**ptr == '0') {
		(*ptr)++;
	}

	// read through to : or non-digit for hours
	while (**ptr >= '0' && **ptr <= '9') {
		hours = hours * 10;
		hours += (**ptr) - '0';
		(*ptr)++;
	}

	if (begin == *ptr) {
		return TIMELIB_UNSET;
	}

	// multiplication with -1, because the offset in the identifier is the
	// 'wrong' way around as for example EST5 is UTC-5 (and not +5)
	return -1 * bias * (hours * 3600 + minutes * 60 + seconds);
}

void timelib_posix_str_dtor(timelib_posix_str *ps)
{
	if (ps->std) {
		timelib_free(ps->std);
	}

	timelib_free(ps);
}

timelib_posix_str* timelib_parse_posix_str(const char *posix)
{
	timelib_posix_str *tmp = timelib_calloc(1, sizeof(timelib_posix_str));
	char *ptr = (char*) posix;

	/* read standard description (ie. EST or <-03>) */
	tmp->std = read_description(&ptr);
	if (!tmp->std) {
		timelib_posix_str_dtor(tmp);
		return NULL;
	}

	/* read required offset */
	tmp->std_offset = read_offset(&ptr);
	if (tmp->std_offset == TIMELIB_UNSET) {
		timelib_posix_str_dtor(tmp);
		return NULL;
	}

	return tmp;
}
