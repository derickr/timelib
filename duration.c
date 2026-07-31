/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2026 Derick Rethans
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

int timelib_duration_ctor_static(
	timelib_duration *duration,
	timelib_ull       seconds,
	uint32_t          nanoseconds,
	bool              negative
) {
	if (seconds > MAX_DURATION_SECONDS) {
		return TIMELIB_ERROR_SECONDS_OUT_OF_RANGE;
	}
	if (nanoseconds >= NSECS_PER_SEC) {
		return TIMELIB_ERROR_NANOSECONDS_OUT_OF_RANGE;
	}

	duration->seconds = seconds;
	duration->nanoseconds = nanoseconds;
	duration->negative = negative;

	return TIMELIB_ERROR_NO_ERROR;
}

timelib_duration *timelib_duration_ctor(
	timelib_ull            seconds,
	uint32_t               nanoseconds,
	bool                   negative,
	int                   *error_code
) {
	timelib_duration *tmp = calloc(1, sizeof(timelib_duration));

	*error_code = timelib_duration_ctor_static(tmp, seconds, nanoseconds, negative);

	if (*error_code != TIMELIB_ERROR_NO_ERROR) {
		free(tmp);
		return NULL;
	}

	return tmp;
}

void timelib_duration_dtor(timelib_duration *duration)
{
	free(duration);
}

int timelib_duration_add_static(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	const timelib_duration *additional
) {
	int c = 0;

	/* ++ / -- */
	if (original->negative == additional->negative) {
		new_duration->seconds = original->seconds + additional->seconds;
		new_duration->nanoseconds = original->nanoseconds + additional->nanoseconds;
		if (new_duration->nanoseconds >= NSECS_PER_SEC) {
			new_duration->seconds++;
			new_duration->nanoseconds -= NSECS_PER_SEC;
		}
		new_duration->negative = original->negative;

		return TIMELIB_ERROR_NO_ERROR;
	}

	/* +- / -+ */
	c = timelib_duration_abs_compare(original, additional);

	switch (c)
	{
		case 0:
			new_duration->negative = false;
			new_duration->seconds = 0;
			new_duration->nanoseconds = 0;

			return TIMELIB_ERROR_NO_ERROR;

		case 1: {
			timelib_sll tmp_nanoseconds;

			new_duration->negative = additional->negative;
			new_duration->seconds = additional->seconds - original->seconds;
			tmp_nanoseconds = additional->nanoseconds - original->nanoseconds;
			if (tmp_nanoseconds < 0) {
				new_duration->seconds--;
				new_duration->nanoseconds = tmp_nanoseconds + NSECS_PER_SEC;
			} else {
				new_duration->nanoseconds = tmp_nanoseconds;
			}

			return TIMELIB_ERROR_NO_ERROR;
		}

		case -1: {
			timelib_sll tmp_nanoseconds;

			new_duration->negative = original->negative;
			new_duration->seconds = original->seconds - additional->seconds;
			tmp_nanoseconds = original->nanoseconds - additional->nanoseconds;
			if (tmp_nanoseconds < 0) {
				new_duration->seconds++;
				new_duration->nanoseconds = tmp_nanoseconds + NSECS_PER_SEC;
			} else {
				new_duration->nanoseconds = tmp_nanoseconds;
			}

			return TIMELIB_ERROR_NO_ERROR;
		}
	}

	/* Should not be reachable due to semantics of timelib_duration_abs_compare() */
	return TIMELIB_ERROR_NO_ERROR;
}

timelib_duration *timelib_duration_add(
	const timelib_duration *original,
	const timelib_duration *additional,
	int                    *error_code
) {
	timelib_duration *tmp = calloc(1, sizeof(timelib_duration));

	*error_code = timelib_duration_add_static(tmp, original, additional);

	if (*error_code != TIMELIB_ERROR_NO_ERROR) {
		free(tmp);
		return NULL;
	}

	return tmp;
}

/**
 * From: https://stackoverflow.com/questions/1815367/catch-and-compute-overflow-during-multiplication-of-two-large-integers
 */
static uint64_t hi(uint64_t x) {
    return x >> 32;
}

static uint64_t lo(uint64_t x) {
    return ((1ULL << 32) - 1) & x;
}

static void multiply(uint64_t a, uint64_t b, uint64_t *result, uint64_t *carry)
{
    // actually uint32_t would do, but the casting is annoying
    uint64_t s0, s1, s2, s3;

    uint64_t x = lo(a) * lo(b);
    s0 = lo(x);

    x = hi(a) * lo(b) + hi(x);
    s1 = lo(x);
    s2 = hi(x);

    x = s1 + lo(a) * hi(b);
    s1 = lo(x);

    x = s2 + hi(a) * hi(b) + hi(x);
    s2 = lo(x);
    s3 = hi(x);

    *result = s1 << 32 | s0;
    *carry = s3 << 32 | s2;
}

int timelib_duration_mul_static(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	uint64_t                factor
) {
	uint64_t ns_result, ns_carry, s_result, s_carry, extra_seconds;

	multiply(original->nanoseconds, factor, &ns_result, &ns_carry);
	if (ns_carry > 0) {
		/* Handle carry situation of nanoseconds */
		return TIMELIB_ERROR_OVERFLOW;
	}

	multiply(original->seconds, factor, &s_result, &s_carry);
	if (s_carry > 0) {
		return TIMELIB_ERROR_OVERFLOW;
	}

	extra_seconds = ns_result / NSECS_PER_SEC;
	ns_result = ns_result % NSECS_PER_SEC;

	if (UINT64_MAX - extra_seconds < s_result) {
		return TIMELIB_ERROR_OVERFLOW;
	}

	new_duration->nanoseconds = ns_result;
	new_duration->seconds = s_result + extra_seconds;

	if (new_duration->seconds == 0 && new_duration->nanoseconds == 0) {
		new_duration->negative = false;
	} else {
		new_duration->negative = original->negative;
	}

	return TIMELIB_ERROR_NO_ERROR;
}

timelib_duration *timelib_duration_mul(
	const timelib_duration *original,
	uint64_t                factor,
	int                    *error_code
) {
	timelib_duration *tmp = calloc(1, sizeof(timelib_duration));

	*error_code = timelib_duration_mul_static(tmp, original, factor);

	if (*error_code != TIMELIB_ERROR_NO_ERROR) {
		free(tmp);
		return NULL;
	}

	return tmp;
}

int timelib_duration_div_static(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	uint64_t                divisor
) {
	if (divisor < 1) {
		return TIMELIB_ERROR_DIVISION_BY_ZERO;
	}

	new_duration->seconds = original->seconds / divisor;
	new_duration->nanoseconds = original->nanoseconds + ((original->seconds % divisor) * NSECS_PER_SEC);
	new_duration->nanoseconds = new_duration->nanoseconds / divisor;

	if (new_duration->seconds == 0 && new_duration->nanoseconds == 0) {
		new_duration->negative = false;
	} else {
		new_duration->negative = original->negative;
	}

	return TIMELIB_ERROR_NO_ERROR;
}

timelib_duration *timelib_duration_div(
	const timelib_duration *original,
	uint64_t                divisor,
	int                    *error_code
) {
	timelib_duration *tmp = calloc(1, sizeof(timelib_duration));

	*error_code = timelib_duration_div_static(tmp, original, divisor);

	if (*error_code != TIMELIB_ERROR_NO_ERROR) {
		free(tmp);
		return NULL;
	}

	return tmp;
}

int timelib_duration_negate_static(timelib_duration *new_duration, const timelib_duration *original)
{
	new_duration->seconds = original->seconds;
	new_duration->nanoseconds = original->nanoseconds;

	if (original->seconds == 0 && original->nanoseconds == 0) {
		new_duration->negative = false;
	} else {
		new_duration->negative = !original->negative;
	}

	return TIMELIB_ERROR_NO_ERROR;
}

timelib_duration *timelib_duration_negate(const timelib_duration *original, int *error_code)
{
	timelib_duration *tmp = calloc(1, sizeof(timelib_duration));

	*error_code = timelib_duration_negate_static(tmp, original);

	if (*error_code != TIMELIB_ERROR_NO_ERROR) {
		free(tmp);
		return NULL;
	}

	return tmp;
}

int timelib_duration_abs_compare(const timelib_duration *one, const timelib_duration *two)
{
	if (one->seconds < two->seconds) {
		return -1;
	}
	if (one->seconds > two->seconds) {
		return 1;
	}

	if (one->nanoseconds < two->nanoseconds) {
		return -1;
	}
	if (one->nanoseconds > two->nanoseconds) {
		return 1;
	}

	return 0;
}

int timelib_duration_compare(const timelib_duration *one, const timelib_duration *two)
{
	if (one->negative && !two->negative) {
		return -1;
	}
	if (!one->negative && two->negative) {
		return 1;
	}

	if (one->negative && two->negative) {
		return timelib_duration_abs_compare(two, one);
	}

	return timelib_duration_abs_compare(one, two);
}
