#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include "timelib_private.h"
#include <stdio.h>
#include <string.h>

TEST_GROUP(parse_date_decimal_relative)
{
	timelib_time *t;
	int           i;
	timelib_error_container *errors;

	void test_parse(const char *input)
	{
		if (t) {
			if (t->tz_info) {
				timelib_tzinfo_dtor(t->tz_info);
			}
			timelib_time_dtor(t);
			timelib_error_container_dtor(errors);
		}

		char *inputCopy = strdup(input);
		t = timelib_strtotime(
			inputCopy, strlen(inputCopy),
			&errors,
			timelib_builtin_db(),
			timelib_parse_tzfile
		);

		free(inputCopy);
	}

	TEST_TEARDOWN()
	{
		if (t->tz_info) {
			timelib_tzinfo_dtor(t->tz_info);
		}
		timelib_time_dtor(t);
		timelib_error_container_dtor(errors);
	}
};

/*
 * Test cases for GH-21027: strtotime() incorrectly parses decimal relative values
 * https://github.com/php/php-src/issues/21027
 */

/* 2.5 weeks = 2.5 * 7 * 86400 = 1512000 seconds */
TEST(parse_date_decimal_relative, decimal_weeks_01)
{
	test_parse("2.5 weeks");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(1512000, t->relative.s);
}

/* 1.5 weeks = 1.5 * 7 * 86400 = 907200 seconds */
TEST(parse_date_decimal_relative, decimal_weeks_02)
{
	test_parse("1.5 weeks");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(907200, t->relative.s);
}

/* 0.5 week = 0.5 * 7 * 86400 = 302400 seconds */
TEST(parse_date_decimal_relative, decimal_weeks_03)
{
	test_parse("0.5 week");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(302400, t->relative.s);
}

/* 2.5 days = 2.5 * 86400 = 216000 seconds */
TEST(parse_date_decimal_relative, decimal_days_01)
{
	test_parse("2.5 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(216000, t->relative.s);
}

/* 1.5 days = 1.5 * 86400 = 129600 seconds */
TEST(parse_date_decimal_relative, decimal_days_02)
{
	test_parse("1.5 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(129600, t->relative.s);
}

/* 2.5 hours = 2.5 * 3600 = 9000 seconds */
TEST(parse_date_decimal_relative, decimal_hours_01)
{
	test_parse("2.5 hours");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(9000, t->relative.s);
}

/* 1.5 hours = 1.5 * 3600 = 5400 seconds */
TEST(parse_date_decimal_relative, decimal_hours_02)
{
	test_parse("1.5 hours");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(5400, t->relative.s);
}

/* 2.5 minutes = 2.5 * 60 = 150 seconds */
TEST(parse_date_decimal_relative, decimal_minutes_01)
{
	test_parse("2.5 minutes");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(150, t->relative.s);
}

/* 1.5 minutes = 1.5 * 60 = 90 seconds */
TEST(parse_date_decimal_relative, decimal_minutes_02)
{
	test_parse("1.5 minutes");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(90, t->relative.s);
}

/* 1.5 seconds = 1 second (truncated) */
TEST(parse_date_decimal_relative, decimal_seconds_01)
{
	test_parse("1.5 seconds");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(1, t->relative.s);
}

/* -2.5 weeks = -2.5 * 7 * 86400 = -1512000 seconds */
TEST(parse_date_decimal_relative, decimal_weeks_negative)
{
	test_parse("-2.5 weeks");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-1512000, t->relative.s);
}

/* -1.5 days = -1.5 * 86400 = -129600 seconds */
TEST(parse_date_decimal_relative, decimal_days_negative)
{
	test_parse("-1.5 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-129600, t->relative.s);
}

/* +2.5 hours = 2.5 * 3600 = 9000 seconds */
TEST(parse_date_decimal_relative, decimal_hours_positive_sign)
{
	test_parse("+2.5 hours");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(9000, t->relative.s);
}

/* 2.5 months = 2.5 * 2629746 = 6574365 seconds */
TEST(parse_date_decimal_relative, decimal_months_01)
{
	test_parse("2.5 months");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(6574365, t->relative.s);
}

/* 1.5 years = 1.5 * 31556952 = 47335428 seconds */
TEST(parse_date_decimal_relative, decimal_years_01)
{
	test_parse("1.5 years");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(47335428, t->relative.s);
}

/* 2.37685 weeks = 2.37685 * 7 * 86400 = 1437518.88 -> 1437518 seconds */
TEST(parse_date_decimal_relative, decimal_weeks_precision)
{
	test_parse("2.37685 weeks");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(1437518, t->relative.s);
}

/* 3.14159 days = 3.14159 * 86400 = 271433.376 -> 271433 seconds */
TEST(parse_date_decimal_relative, decimal_days_precision)
{
	test_parse("3.14159 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(271433, t->relative.s);
}

/* 1.23456 hours = 1.23456 * 3600 = 4444.416 -> 4444 seconds */
TEST(parse_date_decimal_relative, decimal_hours_precision)
{
	test_parse("1.23456 hours");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(4444, t->relative.s);
}

/* 7.89012 minutes = 7.89012 * 60 = 473.4072 -> 473 seconds */
TEST(parse_date_decimal_relative, decimal_minutes_precision)
{
	test_parse("7.89012 minutes");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(473, t->relative.s);
}

/* -1.23456 days = -1.23456 * 86400 = -106665.984 -> -106665 seconds */
TEST(parse_date_decimal_relative, decimal_days_precision_negative)
{
	test_parse("-1.23456 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-106665, t->relative.s);
}
