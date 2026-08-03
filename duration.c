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

static int timelib_duration_add_abs_internal(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	const timelib_duration *additional
) {
	timelib_ull seconds = original->seconds + additional->seconds;
	uint32_t nanoseconds = original->nanoseconds + additional->nanoseconds;

	if (nanoseconds >= NSECS_PER_SEC) {
		seconds++;
		nanoseconds -= NSECS_PER_SEC;
	}

	return timelib_duration_ctor_static(new_duration, seconds, nanoseconds, original->negative);
}


static int timelib_duration_sub_abs_internal(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	const timelib_duration *minus
) {
	timelib_ull seconds = original->seconds - minus->seconds;
	timelib_sll nanoseconds = (timelib_sll)original->nanoseconds - (timelib_sll)minus->nanoseconds;

	if (nanoseconds < 0) {
		seconds--;
		nanoseconds += NSECS_PER_SEC;
	}

	return timelib_duration_ctor_static(new_duration, seconds, nanoseconds, original->negative);
}

static int timelib_duration_null_abs_internal(timelib_duration *new_duration)
{
	new_duration->negative = false;
	new_duration->seconds = 0;
	new_duration->nanoseconds = 0;

	return TIMELIB_ERROR_NO_ERROR;
}


int timelib_duration_add_static(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	const timelib_duration *additional
) {
	int c = 0;

	/* ++ / -- */
	if (original->negative == additional->negative) {
		return timelib_duration_add_abs_internal(new_duration, original, additional);
	}

	/* +- / -+ */
	c = timelib_duration_abs_compare(original, additional);

	switch (c)
	{
		case 0:
			return timelib_duration_null_abs_internal(new_duration);

		case -1:
			return timelib_duration_sub_abs_internal(new_duration, additional, original);

		case 1:
			return timelib_duration_add_abs_internal(new_duration, additional, original);
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


int timelib_duration_sub_static(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	const timelib_duration *minus
) {
	/* +- */
	if (!original->negative && minus->negative) {
		return timelib_duration_add_abs_internal(new_duration, original, minus);
	}

	/* -+ */
	if (original->negative && !minus->negative) {
		int result = timelib_duration_add_abs_internal(new_duration, original, minus);

		if (result != TIMELIB_ERROR_NO_ERROR) {
			return result;
		}

		new_duration->negative = true;

		return TIMELIB_ERROR_NO_ERROR;
	}

	/* ++ / -- */
	if (original->negative == minus->negative) {
		int c = timelib_duration_abs_compare(original, minus);

		switch (c)
		{
			case 0:
				return timelib_duration_null_abs_internal(new_duration);

			case -1: {
				timelib_duration tmp_duration;

				int result = timelib_duration_sub_abs_internal(&tmp_duration, minus, original);
				if (result != TIMELIB_ERROR_NO_ERROR) {
					return result;
				}

				return timelib_duration_negate_static(new_duration, &tmp_duration);
			}

			case 1:
				return timelib_duration_sub_abs_internal(new_duration, original, minus);
		}
	}

	/* Should not be reachable due to semantics of comparisons above */
	return TIMELIB_ERROR_NO_ERROR;
}

timelib_duration *timelib_duration_sub(
	const timelib_duration *original,
	const timelib_duration *minus,
	int                    *error_code
) {
	timelib_duration *tmp = calloc(1, sizeof(timelib_duration));

	*error_code = timelib_duration_sub_static(tmp, original, minus);

	if (*error_code != TIMELIB_ERROR_NO_ERROR) {
		free(tmp);
		return NULL;
	}

	return tmp;
}

int timelib_duration_mul_static(
	timelib_duration       *new_duration,
	const timelib_duration *original,
	uint64_t                factor
) {
	uint64_t seconds, extra_seconds, nanoseconds;

	if (factor == 0) {
		return timelib_duration_null_abs_internal(new_duration);
	}

	if (original->seconds > UINT64_MAX / factor) {
		return TIMELIB_ERROR_OVERFLOW;
	}

	seconds = original->seconds * factor;

	/* Calculate the number of whole seconds in the nanoseconds product.
	 *
	 * extra_seconds is guaranteed to be smaller than factor, because
	 * original->nanoseconds is smaller than NSECS_PER_SEC. */
	extra_seconds = original->nanoseconds * (factor / NSECS_PER_SEC);

	/* This cannot overflow either, because NSECS_PER_SEC * NSECS_PER_SEC fits uint64_t.
	 *
	 * (nanoseconds * factor) % NSECS_PER_SEC is mathematically equivalent
	 * to (nanoseconds * (factor % NSECS_PER_SEC)) % NSECS_PER_SEC. */
	nanoseconds = original->nanoseconds * (factor % NSECS_PER_SEC);
	extra_seconds += nanoseconds / NSECS_PER_SEC;
	nanoseconds %= NSECS_PER_SEC;

	if (UINT64_MAX - extra_seconds < seconds) {
		return TIMELIB_ERROR_OVERFLOW;
	}

	seconds += extra_seconds;

	new_duration->nanoseconds = nanoseconds;
	new_duration->seconds = seconds;
	new_duration->negative = original->negative;

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
