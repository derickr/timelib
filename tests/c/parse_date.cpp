#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include "timelib_private.h"
#include <stdio.h>
#include <string.h>

TEST_GROUP(parse_date)
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

TEST(parse_date, american_00)
{
	test_parse("9/11");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(11, t->d);
}

TEST(parse_date, american_01)
{
	test_parse("09/11");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(11, t->d);
}

TEST(parse_date, american_02)
{
	test_parse("12/22/69");
	LONGS_EQUAL(2069, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, american_03)
{
	test_parse("12/22/70");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, american_04)
{
	test_parse("12/22/78");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, american_05)
{
	test_parse("12/22/1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, american_06)
{
	test_parse("12/22/2078");
	LONGS_EQUAL(2078, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, bug37017_00)
{
	test_parse("2006-05-12 12:59:59 America/New_York");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	STRCMP_EQUAL("America/New_York", t->tz_info->name);
}

TEST(parse_date, bug37017_01)
{
	test_parse("2006-05-12 13:00:00 America/New_York");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("America/New_York", t->tz_info->name);
}

TEST(parse_date, bug37017_02)
{
	test_parse("2006-05-12 13:00:01 America/New_York");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 1, t->s);
	STRCMP_EQUAL("America/New_York", t->tz_info->name);
}

TEST(parse_date, bug37017_03)
{
	test_parse("2006-05-12 12:59:59 GMT");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
}

TEST(parse_date, bug41523_00)
{
	test_parse("0000-00-00");
	LONGS_EQUAL(   0, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_01)
{
	test_parse("0001-00-00");
	LONGS_EQUAL(   1, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_02)
{
	test_parse("0002-00-00");
	LONGS_EQUAL(   2, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_03)
{
	test_parse("0003-00-00");
	LONGS_EQUAL(   3, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_04)
{
	test_parse("000-00-00");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_05)
{
	test_parse("001-00-00");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_06)
{
	test_parse("002-00-00");
	LONGS_EQUAL(2002, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_07)
{
	test_parse("003-00-00");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_08)
{
	test_parse("00-00-00");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_09)
{
	test_parse("01-00-00");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_10)
{
	test_parse("02-00-00");
	LONGS_EQUAL(2002, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41523_11)
{
	test_parse("03-00-00");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL( 0, t->m);
	LONGS_EQUAL( 0, t->d);
}

TEST(parse_date, bug41842_00)
{
	test_parse("-0001-06-28");
	LONGS_EQUAL(  -1, t->y);
	LONGS_EQUAL( 6, t->m);
	LONGS_EQUAL(28, t->d);
}

TEST(parse_date, bug41842_01)
{
	test_parse("-2007-06-28");
	LONGS_EQUAL(-2007, t->y);
	LONGS_EQUAL( 6, t->m);
	LONGS_EQUAL(28, t->d);
}

TEST(parse_date, bug41964_00)
{
	test_parse("Ask the experts");
}

TEST(parse_date, bug41964_01)
{
	test_parse("A");
	STRCMP_EQUAL("A", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, bug41964_02)
{
	test_parse("A Revolution in Development");
	STRCMP_EQUAL("A", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, bug44426_00)
{
	test_parse("Aug 27 2007 12:00:00:000AM");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bug44426_01)
{
	test_parse("Aug 27 2007 12:00:00.000AM");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bug44426_02)
{
	test_parse("Aug 27 2007 12:00:00:000");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bug44426_03)
{
	test_parse("Aug 27 2007 12:00:00.000");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bug44426_04)
{
	test_parse("Aug 27 2007 12:00:00AM");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bug44426_05)
{
	test_parse("Aug 27 2007");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
}

TEST(parse_date, bug44426_06)
{
	test_parse("Aug 27 2007 12:00AM");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bug50392_00)
{
	test_parse("2010-03-06 16:07:25");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
}

TEST(parse_date, bug50392_01)
{
	test_parse("2010-03-06 16:07:25.1");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(100000, t->us);
}

TEST(parse_date, bug50392_02)
{
	test_parse("2010-03-06 16:07:25.12");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(120000, t->us);
}

TEST(parse_date, bug50392_03)
{
	test_parse("2010-03-06 16:07:25.123");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123000, t->us);
}

TEST(parse_date, bug50392_04)
{
	test_parse("2010-03-06 16:07:25.1234");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123400, t->us);
}

TEST(parse_date, bug50392_05)
{
	test_parse("2010-03-06 16:07:25.12345");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123450, t->us);
}

TEST(parse_date, bug50392_06)
{
	test_parse("2010-03-06 16:07:25.123456");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date, bug50392_07)
{
	test_parse("2010-03-06 16:07:25.1234567");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date, bug50392_08)
{
	test_parse("2010-03-06 16:07:25.12345678");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date, bug51096_00)
{
	test_parse("first day");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date, bug51096_01)
{
	test_parse("last day");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date, bug51096_02)
{
	test_parse("next month");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(1, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date, bug51096_03)
{
	test_parse("first day next month");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(1, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date, bug51096_04)
{
	test_parse("first day of next month");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(1, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.first_last_day_of);
	LONGS_EQUAL(0, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, bug51096_05)
{
	test_parse("last day next month");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(1, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date, bug51096_06)
{
	test_parse("last day of next month");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(1, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(2, t->relative.first_last_day_of);
	LONGS_EQUAL(0, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}


TEST(parse_date, bug54597_00)
{
	test_parse("January 0099");
	LONGS_EQUAL(  99, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug54597_01)
{
	test_parse("January 1, 0099");
	LONGS_EQUAL(  99, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug54597_02)
{
	test_parse("0099-1");
	LONGS_EQUAL(  99, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug54597_03)
{
	test_parse("0099-January");
	LONGS_EQUAL(  99, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug54597_04)
{
	test_parse("0099-Jan");
	LONGS_EQUAL(  99, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug54597_05)
{
	test_parse("January 1099");
	LONGS_EQUAL(1099, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug54597_06)
{
	test_parse("January 1, 1299");
	LONGS_EQUAL(1299, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug54597_07)
{
	test_parse("1599-1");
	LONGS_EQUAL(1599, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, bug63470_00)
{
	test_parse("2015-07-12 00:00 this week");
	LONGS_EQUAL(2015, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_01)
{
	test_parse("2015-07-12 00:00 sunday this week");
	LONGS_EQUAL(2015, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_02)
{
	test_parse("2015-07-12 00:00 this week sunday");
	LONGS_EQUAL(2015, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_03)
{
	test_parse("2015-07-12 00:00 sunday");
	LONGS_EQUAL(2015, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_04)
{
	test_parse("2008-04-25 00:00 this week tuesday");
	LONGS_EQUAL(2008, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(2, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_05)
{
	test_parse("2008-04-25 00:00 this week sunday");
	LONGS_EQUAL(2008, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_06)
{
	test_parse("Sun 2017-01-01 00:00 saturday this week");
	LONGS_EQUAL(2017, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_07)
{
	test_parse("Mon 2017-01-02 00:00 saturday this week");
	LONGS_EQUAL(2017, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_08)
{
	test_parse("Tue 2017-01-03 00:00 saturday this week");
	LONGS_EQUAL(2017, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug63470_09)
{
	test_parse("Sun 2017-01-02 00:00 saturday this week");
	LONGS_EQUAL(2017, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, bug74819_00)
{
	test_parse("I06.00am 0");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 6, t->d);
}

TEST(parse_date, bugs_00)
{
	test_parse("04/05/06 0045");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bugs_01)
{
	test_parse("17:00 2004-01-03");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bugs_02)
{
	test_parse("2004-03-10 16:33:17.11403+01");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL(33, t->i);
	LONGS_EQUAL(17, t->s);
	LONGS_EQUAL(114030, t->us);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, bugs_03)
{
	test_parse("2004-03-10 16:33:17+01");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL(33, t->i);
	LONGS_EQUAL(17, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, bugs_04)
{
	test_parse("Sun, 21 Dec 2003 20:38:33 +0000 GMT");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(38, t->i);
	LONGS_EQUAL(33, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, bugs_05)
{
	test_parse("2003-11-19 08:00:00 T");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(19, t->d);
	LONGS_EQUAL( 8, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("T", t->tz_abbr);
	LONGS_EQUAL(-25200, t->z);
}

TEST(parse_date, bugs_06)
{
	test_parse("01-MAY-1982 00:00:00");
	LONGS_EQUAL(1982, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bugs_07)
{
	test_parse("2040-06-12T04:32:12");
	LONGS_EQUAL(2040, t->y);
	LONGS_EQUAL( 6, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL(32, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date, bugs_08)
{
	test_parse("july 14th");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
}

TEST(parse_date, bugs_09)
{
	test_parse("july 14tH");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
	STRCMP_EQUAL("H", t->tz_abbr);
	LONGS_EQUAL(28800, t->z);
}

TEST(parse_date, bugs_10)
{
	test_parse("11Oct");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(11, t->d);
}

TEST(parse_date, bugs_11)
{
	test_parse("11 Oct");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(11, t->d);
}

TEST(parse_date, bugs_12)
{
	test_parse("2005/04/05/08:15:48 last saturday");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, bugs_13)
{
	test_parse("2005/04/05/08:15:48 last sunday");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, bugs_14)
{
	test_parse("2005/04/05/08:15:48 last monday");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, bugs_15)
{
	test_parse("2004-04-07 00:00:00 CET -10 day +1 hour");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL( 7, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-10, t->relative.d);
	LONGS_EQUAL(1, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, bugs_16)
{
	test_parse("Jan14, 2004");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(14, t->d);
}

TEST(parse_date, bugs_17)
{
	test_parse("Jan 14, 2004");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(14, t->d);
}

TEST(parse_date, bugs_18)
{
	test_parse("Jan.14, 2004");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(14, t->d);
}

TEST(parse_date, bugs_19)
{
	test_parse("1999-10-13");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(13, t->d);
}

TEST(parse_date, bugs_20)
{
	test_parse("Oct 13  1999");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(13, t->d);
}

TEST(parse_date, bugs_21)
{
	test_parse("2000-01-19");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(19, t->d);
}

TEST(parse_date, bugs_22)
{
	test_parse("Jan 19  2000");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(19, t->d);
}

TEST(parse_date, bugs_23)
{
	test_parse("2001-12-21");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
}

TEST(parse_date, bugs_24)
{
	test_parse("Dec 21  2001");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
}

TEST(parse_date, bugs_25)
{
	test_parse("2001-12-21 12:16");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(16, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bugs_26)
{
	test_parse("Dec 21 2001 12:16");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(16, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bugs_27)
{
	test_parse("Dec 21  12:16");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(16, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, bugs_28)
{
	test_parse("2001-10-22 21:19:58");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
}

TEST(parse_date, bugs_29)
{
	test_parse("2001-10-22 21:19:58-02");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-7200, t->z);
}

TEST(parse_date, bugs_30)
{
	test_parse("2001-10-22 21:19:58-0213");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-7980, t->z);
}

TEST(parse_date, bugs_31)
{
	test_parse("2001-10-22 21:19:58+02");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(7200, t->z);
}

TEST(parse_date, bugs_32)
{
	test_parse("2001-10-22 21:19:58+0213");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(7980, t->z);
}

TEST(parse_date, bugs_33)
{
	test_parse("2001-10-22T21:20:58-03:40");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-13200, t->z);
}

TEST(parse_date, bugs_34)
{
	test_parse("2001-10-22T211958-2");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-7200, t->z);
}

TEST(parse_date, bugs_35)
{
	test_parse("20011022T211958+0213");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(7980, t->z);
}

TEST(parse_date, bugs_36)
{
	test_parse("20011022T21:20+0215");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(8100, t->z);
}

TEST(parse_date, bugs_37)
{
	test_parse("1997W011");
	LONGS_EQUAL(1997, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, bugs_38)
{
	test_parse("2004W101T05:00+0");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 5, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(60, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, bugs_39)
{
	test_parse("nextyear");
}

TEST(parse_date, bugs_40)
{
	test_parse("next year");
	LONGS_EQUAL(1, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, combined_00)
{
	test_parse("Sat, 24 Apr 2004 21:48:40 +0200");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(24, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(48, t->i);
	LONGS_EQUAL(40, t->s);
	LONGS_EQUAL(7200, t->z);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, combined_01)
{
	test_parse("Sun Apr 25 01:05:41 CEST 2004");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, combined_02)
{
	test_parse("Sun Apr 18 18:36:57 2004");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(36, t->i);
	LONGS_EQUAL(57, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, combined_03)
{
	test_parse("Sat, 24 Apr 2004	21:48:40	+0200");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(24, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(48, t->i);
	LONGS_EQUAL(40, t->s);
	LONGS_EQUAL(7200, t->z);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, combined_04)
{
	test_parse("20040425010541 CEST");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, combined_05)
{
	test_parse("20040425010541");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date, combined_06)
{
	test_parse("19980717T14:08:55");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(17, t->d);
	LONGS_EQUAL(14, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(55, t->s);
}

TEST(parse_date, combined_07)
{
	test_parse("10/Oct/2000:13:55:36 -0700");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(55, t->i);
	LONGS_EQUAL(36, t->s);
	LONGS_EQUAL(-25200, t->z);
}

TEST(parse_date, combined_08)
{
	test_parse("2001-11-29T13:20:01.123");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL( 1, t->s);
	LONGS_EQUAL(123000, t->us);
}

TEST(parse_date, combined_09)
{
	test_parse("2001-11-29T13:20:01.123-05:00");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL( 1, t->s);
	LONGS_EQUAL(123000, t->us);
	LONGS_EQUAL(-18000, t->z);
}

TEST(parse_date, combined_10)
{
	test_parse("Fri Aug 20 11:59:59 1993 GMT");
	LONGS_EQUAL(1993, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, combined_11)
{
	test_parse("Fri Aug 20 11:59:59 1993 UTC");
	LONGS_EQUAL(1993, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, combined_12)
{
	test_parse("Fri	Aug	20	 11:59:59	 1993	UTC");
	LONGS_EQUAL(1993, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, combined_13)
{
	test_parse("May 18th 5:05 UTC");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 5, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date, combined_14)
{
	test_parse("May 18th 5:05pm UTC");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date, combined_15)
{
	test_parse("May 18th 5:05 pm UTC");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date, combined_16)
{
	test_parse("May 18th 5:05am UTC");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 5, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date, combined_17)
{
	test_parse("May 18th 5:05 am UTC");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 5, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date, combined_18)
{
	test_parse("May 18th 2006 5:05pm UTC");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date, common_00)
{
	test_parse("now");
}

TEST(parse_date, common_01)
{
	test_parse("NOW");
}

TEST(parse_date, common_02)
{
	test_parse("noW");
}

TEST(parse_date, common_03)
{
	test_parse("today");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_04)
{
	test_parse("midnight");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_05)
{
	test_parse("noon");
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_06)
{
	test_parse("tomorrow");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_07)
{
	test_parse("yesterday 08:15pm");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_08)
{
	test_parse("yesterday midnight");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_09)
{
	test_parse("tomorrow 18:00");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_10)
{
	test_parse("tomorrow noon");
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_11)
{
	test_parse("TODAY");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_12)
{
	test_parse("MIDNIGHT");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_13)
{
	test_parse("NOON");
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_14)
{
	test_parse("TOMORROW");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_15)
{
	test_parse("YESTERDAY 08:15pm");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_16)
{
	test_parse("YESTERDAY MIDNIGHT");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_17)
{
	test_parse("TOMORROW 18:00");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_18)
{
	test_parse("TOMORROW NOON");
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_19)
{
	test_parse("ToDaY");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_20)
{
	test_parse("mIdNiGhT");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_21)
{
	test_parse("NooN");
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, common_22)
{
	test_parse("ToMoRRoW");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_23)
{
	test_parse("yEstErdAY 08:15pm");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_24)
{
	test_parse("yEsTeRdAY mIdNiGht");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_25)
{
	test_parse("toMOrrOW 18:00");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_26)
{
	test_parse("TOmoRRow nOOn");
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, common_27)
{
	test_parse("TOmoRRow	nOOn");
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(1, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, datefull_00)
{
	test_parse("22 dec 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_01)
{
	test_parse("22-dec-78");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_02)
{
	test_parse("22 Dec 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_03)
{
	test_parse("22DEC78");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_04)
{
	test_parse("22 december 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_05)
{
	test_parse("22-december-78");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_06)
{
	test_parse("22 December 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_07)
{
	test_parse("22DECEMBER78");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_08)
{
	test_parse("22	dec	1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_09)
{
	test_parse("22	Dec	1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_10)
{
	test_parse("22	december	1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datefull_11)
{
	test_parse("22	December	1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datenocolon_00)
{
	test_parse("19781222");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datenoday_00)
{
	test_parse("Oct 2003");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, datenoday_01)
{
	test_parse("20 October 2003");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(20, t->d);
}

TEST(parse_date, datenoday_02)
{
	test_parse("Oct 03");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 3, t->d);
}

TEST(parse_date, datenoday_03)
{
	test_parse("Oct 2003 2045");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, datenoday_04)
{
	test_parse("Oct 2003 20:45");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, datenoday_05)
{
	test_parse("Oct 2003 20:45:37");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL(37, t->s);
}

TEST(parse_date, datenoday_06)
{
	test_parse("20 October 2003 00:00 CEST");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, datenoday_07)
{
	test_parse("Oct 03 21:46m");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(46, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("M", t->tz_abbr);
	LONGS_EQUAL(43200, t->z);
}

TEST(parse_date, datenoday_08)
{
	test_parse("Oct	2003	20:45");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, datenoday_09)
{
	test_parse("Oct	03	21:46m");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(46, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("M", t->tz_abbr);
	LONGS_EQUAL(43200, t->z);
}

TEST(parse_date, date_00)
{
	test_parse("31.01.2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date, date_01)
{
	test_parse("32.01.2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, date_02)
{
	test_parse("28.01.2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(28, t->d);
}

TEST(parse_date, date_03)
{
	test_parse("29.01.2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date, date_04)
{
	test_parse("30.01.2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(30, t->d);
}

TEST(parse_date, date_05)
{
	test_parse("31.01.2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date, date_06)
{
	test_parse("32.01.2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, date_07)
{
	test_parse("31-01-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date, date_08)
{
	test_parse("32-01-2006");
	LONGS_EQUAL(2032, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(20, t->d);
}

TEST(parse_date, date_09)
{
	test_parse("28-01-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(28, t->d);
}

TEST(parse_date, date_10)
{
	test_parse("29-01-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date, date_11)
{
	test_parse("30-01-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(30, t->d);
}

TEST(parse_date, date_12)
{
	test_parse("31-01-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date, date_13)
{
	test_parse("32-01-2006");
	LONGS_EQUAL(2032, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(20, t->d);
}

TEST(parse_date, date_14)
{
	test_parse("29-02-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date, date_15)
{
	test_parse("30-02-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL(30, t->d);
}

TEST(parse_date, date_16)
{
	test_parse("31-02-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date, date_17)
{
	test_parse("01-01-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
}

TEST(parse_date, date_18)
{
	test_parse("31-12-2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date, date_19)
{
	test_parse("31-13-2006");
	LONGS_EQUAL(-46800, t->z);
}

TEST(parse_date, date_20)
{
	test_parse("11/10/2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(10, t->d);
}

TEST(parse_date, date_21)
{
	test_parse("12/10/2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(10, t->d);
}

TEST(parse_date, date_22)
{
	test_parse("13/10/2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(10, t->d);
}

TEST(parse_date, date_23)
{
	test_parse("14/10/2006");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(10, t->d);
}

TEST(parse_date, dateroman_00)
{
	test_parse("22 I 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_01)
{
	test_parse("22. II 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_02)
{
	test_parse("22 III. 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_03)
{
	test_parse("22- IV- 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_04)
{
	test_parse("22 -V -1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_05)
{
	test_parse("22-VI-1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 6, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_06)
{
	test_parse("22.VII.1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_07)
{
	test_parse("22 VIII 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_08)
{
	test_parse("22 IX 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_09)
{
	test_parse("22 X 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_10)
{
	test_parse("22 XI 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateroman_11)
{
	test_parse("22	XII	1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, dateslash_00)
{
	test_parse("2005/8/12");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(12, t->d);
}

TEST(parse_date, dateslash_01)
{
	test_parse("2005/01/02");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, dateslash_02)
{
	test_parse("2005/01/2");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, dateslash_03)
{
	test_parse("2005/1/02");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, dateslash_04)
{
	test_parse("2005/1/2");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, datetextual_00)
{
	test_parse("December 22, 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_01)
{
	test_parse("DECEMBER 22nd 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_02)
{
	test_parse("December 22. 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_03)
{
	test_parse("December 22 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_04)
{
	test_parse("Dec 22, 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_05)
{
	test_parse("DEC 22nd 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_06)
{
	test_parse("Dec 22. 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_07)
{
	test_parse("Dec 22 1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_08)
{
	test_parse("December 22");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_09)
{
	test_parse("Dec 22");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_10)
{
	test_parse("DEC 22nd");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_11)
{
	test_parse("December	22	1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, datetextual_12)
{
	test_parse("DEC	22nd");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, frontof_00)
{
	test_parse("frONt of 0 0");
	LONGS_EQUAL(-1, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, frontof_01)
{
	test_parse("frONt of 4pm");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, frontof_02)
{
	test_parse("frONt of 4 pm");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601date_00)
{
	test_parse("1978-12-22");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, iso8601date_01)
{
	test_parse("0078-12-22");
	LONGS_EQUAL(  78, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, iso8601date_02)
{
	test_parse("078-12-22");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, iso8601date_03)
{
	test_parse("78-12-22");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, iso8601date_04)
{
	test_parse("4-4-25");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
}

TEST(parse_date, iso8601date_05)
{
	test_parse("69-4-25");
	LONGS_EQUAL(2069, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
}

TEST(parse_date, iso8601date_06)
{
	test_parse("70-4-25");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
}

TEST(parse_date, iso8601date_07)
{
	test_parse("1978/12/22");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, iso8601date_08)
{
	test_parse("1978/02/02");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, iso8601date_09)
{
	test_parse("1978/12/02");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, iso8601date_10)
{
	test_parse("1978/02/22");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, iso8601long_00)
{
	test_parse("01:00:03.12345");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
}

TEST(parse_date, iso8601long_01)
{
	test_parse("13:03:12.45678");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
}

TEST(parse_date, iso8601longtz_00)
{
	test_parse("01:00:03.12345 CET");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601longtz_01)
{
	test_parse("13:03:12.45678 CEST");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601longtz_02)
{
	test_parse("15:57:41.0GMT");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date, iso8601longtz_03)
{
	test_parse("15:57:41.0 pdt");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("PDT", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601longtz_04)
{
	test_parse("23:41:00.0Z");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("Z", t->tz_abbr);
}

TEST(parse_date, iso8601longtz_05)
{
	test_parse("23:41:00.0 k");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("K", t->tz_abbr);
	LONGS_EQUAL(36000, t->z);
}

TEST(parse_date, iso8601longtz_06)
{
	test_parse("04:05:07.789cast");
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 7, t->s);
	LONGS_EQUAL(789000, t->us);
	STRCMP_EQUAL("CAST", t->tz_abbr);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date, iso8601longtz_07)
{
	test_parse("01:00:03.12345  +1");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601longtz_08)
{
	test_parse("13:03:12.45678 +0100");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601longtz_09)
{
	test_parse("15:57:41.0-0");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date, iso8601longtz_10)
{
	test_parse("15:57:41.0-8");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date, iso8601longtz_11)
{
	test_parse("23:41:00.0 -0000");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601longtz_12)
{
	test_parse("04:05:07.789 +0930");
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 7, t->s);
	LONGS_EQUAL(789000, t->us);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date, iso8601longtz_13)
{
	test_parse("01:00:03.12345 (CET)");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601longtz_14)
{
	test_parse("13:03:12.45678 (CEST)");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601longtz_15)
{
	test_parse("(CET) 01:00:03.12345");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601longtz_16)
{
	test_parse("(CEST) 13:03:12.45678");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601longtz_17)
{
	test_parse("13:03:12.45678	(CEST)");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601longtz_18)
{
	test_parse("(CEST)	13:03:12.45678");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601nocolon_00)
{
	test_parse("2314");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601nocolon_01)
{
	test_parse("2314 2314");
	LONGS_EQUAL(2314, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601nocolon_02)
{
	test_parse("2314 PST");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("PST", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date, iso8601nocolon_03)
{
	test_parse("231431 CEST");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601nocolon_04)
{
	test_parse("231431 CET");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601nocolon_05)
{
	test_parse("231431");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
}

TEST(parse_date, iso8601nocolon_06)
{
	test_parse("231431 2314");
	LONGS_EQUAL(2314, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
}

TEST(parse_date, iso8601nocolon_07)
{
	test_parse("2314 231431");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601nocolon_08)
{
	test_parse("2314	2314");
	LONGS_EQUAL(2314, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601nocolon_09)
{
	test_parse("2314	PST");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("PST", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date, iso8601nocolon_10)
{
	test_parse("231431	CEST");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601nocolon_11)
{
	test_parse("231431	CET");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601nocolon_12)
{
	test_parse("231431	2314");
	LONGS_EQUAL(2314, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
}

TEST(parse_date, iso8601nocolon_13)
{
	test_parse("2314	231431");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601normtz_00)
{
	test_parse("01:00:03 CET");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601normtz_01)
{
	test_parse("13:03:12 CEST");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601normtz_02)
{
	test_parse("15:57:41GMT");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date, iso8601normtz_03)
{
	test_parse("15:57:41 pdt");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("PDT", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601normtz_04)
{
	test_parse("23:41:02Y");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 2, t->s);
	STRCMP_EQUAL("Y", t->tz_abbr);
	LONGS_EQUAL(-43200, t->z);
}

TEST(parse_date, iso8601normtz_05)
{
	test_parse("04:05:07cast");
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 7, t->s);
	STRCMP_EQUAL("CAST", t->tz_abbr);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date, iso8601normtz_06)
{
	test_parse("01:00:03  +1");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601normtz_07)
{
	test_parse("13:03:12 +0100");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601normtz_08)
{
	test_parse("15:57:41-0");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date, iso8601normtz_09)
{
	test_parse("15:57:41-8");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date, iso8601normtz_10)
{
	test_parse("23:41:01 -0000");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 1, t->s);
}

TEST(parse_date, iso8601normtz_11)
{
	test_parse("04:05:07 +0930");
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 7, t->s);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date, iso8601normtz_12)
{
	test_parse("13:03:12	CEST");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601normtz_13)
{
	test_parse("15:57:41	pdt");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("PDT", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601normtz_14)
{
	test_parse("01:00:03		+1");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601normtz_15)
{
	test_parse("13:03:12	+0100");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601shorttz_00)
{
	test_parse("01:00 CET");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601shorttz_01)
{
	test_parse("13:03 CEST");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601shorttz_02)
{
	test_parse("15:57GMT");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601shorttz_03)
{
	test_parse("15:57 pdt");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("PDT", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date, iso8601shorttz_04)
{
	test_parse("23:41F");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("F", t->tz_abbr);
	LONGS_EQUAL(21600, t->z);
}

TEST(parse_date, iso8601shorttz_05)
{
	test_parse("04:05cast");
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("CAST", t->tz_abbr);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date, iso8601shorttz_06)
{
	test_parse("01:00  +1");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601shorttz_07)
{
	test_parse("13:03 +0100");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, iso8601shorttz_08)
{
	test_parse("15:57-0");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601shorttz_09)
{
	test_parse("15:57-8");
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date, iso8601shorttz_10)
{
	test_parse("23:41 -0000");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, iso8601shorttz_11)
{
	test_parse("04:05 +0930");
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date, last_day_of_00)
{
	test_parse("last saturday of feb 2008");
	LONGS_EQUAL(2008, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
	LONGS_EQUAL(TIMELIB_SPECIAL_LAST_DAY_OF_WEEK_IN_MONTH, t->relative.special.type);
}

TEST(parse_date, last_day_of_01)
{
	test_parse("last tue of 2008-11");
	LONGS_EQUAL(2008, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(2, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
	LONGS_EQUAL(TIMELIB_SPECIAL_LAST_DAY_OF_WEEK_IN_MONTH, t->relative.special.type);
}

TEST(parse_date, last_day_of_02)
{
	test_parse("last sunday of sept");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
	LONGS_EQUAL(TIMELIB_SPECIAL_LAST_DAY_OF_WEEK_IN_MONTH, t->relative.special.type);
}

TEST(parse_date, last_day_of_03)
{
	test_parse("last saturday of this month");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
	LONGS_EQUAL(TIMELIB_SPECIAL_LAST_DAY_OF_WEEK_IN_MONTH, t->relative.special.type);
}

TEST(parse_date, last_day_of_04)
{
	test_parse("last thursday of last month");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(-1, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(4, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
	LONGS_EQUAL(TIMELIB_SPECIAL_LAST_DAY_OF_WEEK_IN_MONTH, t->relative.special.type);
}

TEST(parse_date, last_day_of_05)
{
	test_parse("last wed of fourth month");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(4, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(3, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
	LONGS_EQUAL(TIMELIB_SPECIAL_LAST_DAY_OF_WEEK_IN_MONTH, t->relative.special.type);
}


TEST(parse_date, microsecond_00)
{
	test_parse("+1 ms");
	LONGS_EQUAL(1000, t->relative.us);
}

TEST(parse_date, microsecond_01)
{
	test_parse("+3 msec");
	LONGS_EQUAL(3000, t->relative.us);
}

TEST(parse_date, microsecond_02)
{
	test_parse("+4 msecs");
	LONGS_EQUAL(4000, t->relative.us);
}

TEST(parse_date, microsecond_03)
{
	test_parse("+5 millisecond");
	LONGS_EQUAL(5000, t->relative.us);
}

TEST(parse_date, microsecond_04)
{
	test_parse("+6 milliseconds");
	LONGS_EQUAL(6000, t->relative.us);
}

TEST(parse_date, microsecond_05)
{
	test_parse("+1 µs");
	LONGS_EQUAL(1, t->relative.us);
}

TEST(parse_date, microsecond_06)
{
	test_parse("+3 usec");
	LONGS_EQUAL(3, t->relative.us);
}

TEST(parse_date, microsecond_07)
{
	test_parse("+4 usecs");
	LONGS_EQUAL(4, t->relative.us);
}

TEST(parse_date, microsecond_08)
{
	test_parse("+5 µsec");
	LONGS_EQUAL(5, t->relative.us);
}

TEST(parse_date, microsecond_09)
{
	test_parse("+6 µsecs");
	LONGS_EQUAL(6, t->relative.us);
}

TEST(parse_date, microsecond_10)
{
	test_parse("+7 microsecond");
	LONGS_EQUAL(7, t->relative.us);
}

TEST(parse_date, microsecond_11)
{
	test_parse("+8 microseconds");
	LONGS_EQUAL(8, t->relative.us);
}


TEST(parse_date, mysql_00)
{
	test_parse("19970523091528");
	LONGS_EQUAL(1997, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(23, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL(28, t->s);
}

TEST(parse_date, mysql_01)
{
	test_parse("20001231185859");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(58, t->i);
	LONGS_EQUAL(59, t->s);
}

TEST(parse_date, mysql_02)
{
	test_parse("20500410101010");
	LONGS_EQUAL(2050, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(10, t->h);
	LONGS_EQUAL(10, t->i);
	LONGS_EQUAL(10, t->s);
}

TEST(parse_date, mysql_03)
{
	test_parse("20050620091407");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 6, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 7, t->s);
}

TEST(parse_date, pgsql_00)
{
	test_parse("January 8, 1999");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_01)
{
	test_parse("January	8,	1999");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_02)
{
	test_parse("1999-01-08");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_03)
{
	test_parse("1/8/1999");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_04)
{
	test_parse("1/18/1999");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(18, t->d);
}

TEST(parse_date, pgsql_05)
{
	test_parse("01/02/03");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date, pgsql_06)
{
	test_parse("1999-Jan-08");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_07)
{
	test_parse("Jan-08-1999");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_08)
{
	test_parse("08-Jan-1999");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_09)
{
	test_parse("99-Jan-08");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_10)
{
	test_parse("08-Jan-99");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_11)
{
	test_parse("Jan-08-99");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_12)
{
	test_parse("19990108");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_13)
{
	test_parse("1999.008");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_14)
{
	test_parse("1999.038");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(38, t->d);
}

TEST(parse_date, pgsql_15)
{
	test_parse("1999.238");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(238, t->d);
}

TEST(parse_date, pgsql_16)
{
	test_parse("1999.366");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(366, t->d);
}

TEST(parse_date, pgsql_17)
{
	test_parse("1999008");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_18)
{
	test_parse("1999038");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(38, t->d);
}

TEST(parse_date, pgsql_19)
{
	test_parse("1999238");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(238, t->d);
}

TEST(parse_date, pgsql_20)
{
	test_parse("1999366");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(366, t->d);
}

TEST(parse_date, pgsql_21)
{
	test_parse("1999-008");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date, pgsql_22)
{
	test_parse("1999-038");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(38, t->d);
}

TEST(parse_date, pgsql_23)
{
	test_parse("1999-238");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(238, t->d);
}

TEST(parse_date, pgsql_24)
{
	test_parse("1999-366");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(366, t->d);
}

TEST(parse_date, pointeddate_00)
{
	test_parse("22.12.1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, pointeddate_01)
{
	test_parse("22.7.1978");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, pointeddate_02)
{
	test_parse("22.12.78");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, pointeddate_03)
{
	test_parse("22.7.78");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date, relative_00)
{
	test_parse("2 secs");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(2, t->relative.s);
}

TEST(parse_date, relative_01)
{
	test_parse("+2 sec");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(2, t->relative.s);
}

TEST(parse_date, relative_02)
{
	test_parse("-2 secs");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-2, t->relative.s);
}

TEST(parse_date, relative_15)
{
	test_parse("+2 sec ago");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-2, t->relative.s);
}

TEST(parse_date, relative_16)
{
	test_parse("2 secs ago");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-2, t->relative.s);
}

TEST(parse_date, relative_17)
{
	test_parse("0 second");
}

TEST(parse_date, relative_18)
{
	test_parse("first second");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(1, t->relative.s);
}

TEST(parse_date, relative_19)
{
	test_parse("next second");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(1, t->relative.s);
}

TEST(parse_date, relative_20)
{
	test_parse("second second");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(2, t->relative.s);
}

TEST(parse_date, relative_21)
{
	test_parse("third second");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(3, t->relative.s);
}

TEST(parse_date, relative_22)
{
	test_parse("-3 seconds");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-3, t->relative.s);
}

TEST(parse_date, relative_23)
{
	test_parse("+2 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_24)
{
	test_parse("+2 days ago");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_25)
{
	test_parse("-2 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_26)
{
	test_parse("-3 fortnight");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-42, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_27)
{
	test_parse("+12 weeks");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(84, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_28)
{
	test_parse("- 3 seconds");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(-3, t->relative.s);
}

TEST(parse_date, relative_29)
{
	test_parse("+ 2 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_30)
{
	test_parse("+ 2 days ago");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_31)
{
	test_parse("- 2 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_32)
{
	test_parse("- 3 fortnight");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-42, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_33)
{
	test_parse("+ 12 weeks");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(84, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_34)
{
	test_parse("- 2	days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-2, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_35)
{
	test_parse("-	3 fortnight");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-42, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_36)
{
	test_parse("+	12	weeks");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(84, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_37)
{
	test_parse("6 month 2004-05-05 12:15:23 CEST");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL(23, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(6, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_38)
{
	test_parse("2004-05-05 12:15:23 CEST 6 months");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL(23, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(6, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_39)
{
	test_parse("2004-05-05 12:15:23 CEST 6 months ago");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL(23, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(-6, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_40)
{
	test_parse("6 months ago 4 days");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(-6, t->relative.m);
	LONGS_EQUAL(4, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_41)
{
	test_parse("first month");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(1, t->relative.m);
	LONGS_EQUAL(0, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, relative_42)
{
	test_parse("saturday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, relative_43)
{
	test_parse("saturday ago");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(-6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, relative_44)
{
	test_parse("this saturday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, relative_45)
{
	test_parse("this saturday ago");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(-6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, relative_46)
{
	test_parse("last saturday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_47)
{
	test_parse("last saturday ago");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(-6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_48)
{
	test_parse("first saturday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_49)
{
	test_parse("first saturday ago");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(-6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_50)
{
	test_parse("next saturday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_51)
{
	test_parse("next saturday ago");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(-6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_52)
{
	test_parse("third saturday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(14, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_53)
{
	test_parse("third saturday ago");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-14, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(-6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_54)
{
	test_parse("previous saturday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(0, t->relative.weekday_behavior);
}

TEST(parse_date, relative_55)
{
	test_parse("this weekday");
	LONGS_EQUAL(TIMELIB_SPECIAL_WEEKDAY, t->relative.special.type);
	LONGS_EQUAL(0, t->relative.special.amount);
}

TEST(parse_date, relative_56)
{
	test_parse("last weekday");
	LONGS_EQUAL(TIMELIB_SPECIAL_WEEKDAY, t->relative.special.type);
	LONGS_EQUAL(-1, t->relative.special.amount);
}

TEST(parse_date, relative_57)
{
	test_parse("next weekday");
	LONGS_EQUAL(TIMELIB_SPECIAL_WEEKDAY, t->relative.special.type);
	LONGS_EQUAL(1, t->relative.special.amount);
}

TEST(parse_date, relative_58)
{
	test_parse("8 weekdays ago");
	LONGS_EQUAL(TIMELIB_SPECIAL_WEEKDAY, t->relative.special.type);
	LONGS_EQUAL(-8, t->relative.special.amount);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date, relative_59)
{
	test_parse("Sun, 21 Dec 2003 20:38:33 +0000 GMT");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(38, t->i);
	LONGS_EQUAL(33, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, relative_60)
{
	test_parse("Mon, 08 May 2006 13:06:44 -0400 +30 days");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL( 8, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 6, t->i);
	LONGS_EQUAL(44, t->s);
	LONGS_EQUAL(-14400, t->z);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(30, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date, special_00)
{
	test_parse("1998-9-15T09:05:32+4:0");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date, special_01)
{
	test_parse("1998-09-15T09:05:32+04:00");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date, special_02)
{
	test_parse("1998-09-15T09:05:32.912+04:00");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(912000, t->us);
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date, special_03)
{
	test_parse("1998-09-15T09:05:32");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
}

TEST(parse_date, special_04)
{
	test_parse("19980915T09:05:32");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
}

TEST(parse_date, special_05)
{
	test_parse("19980915t090532");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
}

TEST(parse_date, special_06)
{
	test_parse("1998-09-15T09:05:32+4:9");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14940, t->z);
}

TEST(parse_date, special_07)
{
	test_parse("1998-9-15T09:05:32+4:30");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date, special_08)
{
	test_parse("1998-09-15T09:05:32+04:9");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14940, t->z);
}

TEST(parse_date, special_09)
{
	test_parse("1998-9-15T09:05:32+04:30");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date, timelong12_00)
{
	test_parse("01:00:03am");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
}

TEST(parse_date, timelong12_01)
{
	test_parse("01:03:12pm");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date, timelong12_02)
{
	test_parse("12:31:13 A.M.");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(31, t->i);
	LONGS_EQUAL(13, t->s);
}

TEST(parse_date, timelong12_03)
{
	test_parse("08:13:14 P.M.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(13, t->i);
	LONGS_EQUAL(14, t->s);
}

TEST(parse_date, timelong12_04)
{
	test_parse("11:59:15 AM");
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(15, t->s);
}

TEST(parse_date, timelong12_05)
{
	test_parse("06:12:16 PM");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(12, t->i);
	LONGS_EQUAL(16, t->s);
}

TEST(parse_date, timelong12_06)
{
	test_parse("07:08:17 am");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(17, t->s);
}

TEST(parse_date, timelong12_07)
{
	test_parse("08:09:18 p.m.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL(18, t->s);
}

TEST(parse_date, timelong12_08)
{
	test_parse("01.00.03am");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
}

TEST(parse_date, timelong12_09)
{
	test_parse("01.03.12pm");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date, timelong12_10)
{
	test_parse("12.31.13 A.M.");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(31, t->i);
	LONGS_EQUAL(13, t->s);
}

TEST(parse_date, timelong12_11)
{
	test_parse("08.13.14 P.M.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(13, t->i);
	LONGS_EQUAL(14, t->s);
}

TEST(parse_date, timelong12_12)
{
	test_parse("11.59.15 AM");
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(15, t->s);
}

TEST(parse_date, timelong12_13)
{
	test_parse("06.12.16 PM");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(12, t->i);
	LONGS_EQUAL(16, t->s);
}

TEST(parse_date, timelong12_14)
{
	test_parse("07.08.17 am");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(17, t->s);
}

TEST(parse_date, timelong12_15)
{
	test_parse("08.09.18 p.m.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL(18, t->s);
}

TEST(parse_date, timelong12_16)
{
	test_parse("07.08.17	am");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(17, t->s);
}

TEST(parse_date, timelong12_17)
{
	test_parse("08.09.18	p.m.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL(18, t->s);
}

TEST(parse_date, timelong24_00)
{
	test_parse("01:00:03");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
}

TEST(parse_date, timelong24_01)
{
	test_parse("13:03:12");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date, timelong24_02)
{
	test_parse("24:03:12");
	LONGS_EQUAL(24, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date, timelong24_03)
{
	test_parse("01.00.03");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
}

TEST(parse_date, timelong24_04)
{
	test_parse("13.03.12");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date, timelong24_05)
{
	test_parse("24.03.12");
	LONGS_EQUAL(24, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date, timeshort12_00)
{
	test_parse("01:00am");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_01)
{
	test_parse("01:03pm");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_02)
{
	test_parse("12:31 A.M.");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(31, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_03)
{
	test_parse("08:13 P.M.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(13, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_04)
{
	test_parse("11:59 AM");
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_05)
{
	test_parse("06:12 PM");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(12, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_06)
{
	test_parse("07:08 am");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_07)
{
	test_parse("08:09 p.m.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_08)
{
	test_parse("01.00am");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_09)
{
	test_parse("01.03pm");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_10)
{
	test_parse("12.31 A.M.");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(31, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_11)
{
	test_parse("08.13 P.M.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(13, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_12)
{
	test_parse("11.59 AM");
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_13)
{
	test_parse("06.12 PM");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(12, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_14)
{
	test_parse("07.08 am");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_15)
{
	test_parse("08.09 p.m.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_16)
{
	test_parse("07.08	am");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort12_17)
{
	test_parse("08.09	p.m.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort24_00)
{
	test_parse("01:00");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort24_01)
{
	test_parse("13:03");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort24_02)
{
	test_parse("01.00");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timeshort24_03)
{
	test_parse("13.03");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL( 0, t->s);
}


TEST(parse_date, timestamp_00)
{
	test_parse("@1508765076.3");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(300000,     t->relative.us);
}

TEST(parse_date, timestamp_01)
{
	test_parse("@1508765076.34");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(340000,     t->relative.us);
}

TEST(parse_date, timestamp_02)
{
	test_parse("@1508765076.347");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(347000,     t->relative.us);
}

TEST(parse_date, timestamp_03)
{
	test_parse("@1508765076.3479");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(347900,     t->relative.us);
}

TEST(parse_date, timestamp_04)
{
	test_parse("@1508765076.34795");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(347950,     t->relative.us);
}

TEST(parse_date, timestamp_05)
{
	test_parse("@1508765076.347958");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(347958,     t->relative.us);
}

TEST(parse_date, timestamp_06)
{
	test_parse("@1508765076.003");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(  3000,     t->relative.us);
}

TEST(parse_date, timestamp_07)
{
	test_parse("@1508765076.0003");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(   300,     t->relative.us);
}

TEST(parse_date, timestamp_08)
{
	test_parse("@1508765076.00003");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(    30,     t->relative.us);
}

TEST(parse_date, timestamp_09)
{
	test_parse("@1508765076.000003");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1508765076, t->relative.s);
	LONGS_EQUAL(     3,     t->relative.us);
}

TEST(parse_date, php_gh_7758)
{
	test_parse("@-0.4");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL( 0, t->relative.s);
	LONGS_EQUAL(-400000, t->relative.us);
}


TEST(parse_date, timetiny12_00)
{
	test_parse("01am");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_01)
{
	test_parse("01pm");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_02)
{
	test_parse("12 A.M.");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_03)
{
	test_parse("08 P.M.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_04)
{
	test_parse("11 AM");
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_05)
{
	test_parse("06 PM");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_06)
{
	test_parse("07 am");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_07)
{
	test_parse("08 p.m.");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_08)
{
	test_parse("09	am");
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny12_09)
{
	test_parse("10	p.m.");
	LONGS_EQUAL(22, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, tzcorrection_00)
{
	test_parse("+4:30");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date, tzcorrection_01)
{
	test_parse("+4");
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date, tzcorrection_02)
{
	test_parse("+1");
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date, tzcorrection_03)
{
	test_parse("+14");
	LONGS_EQUAL(50400, t->z);
}

TEST(parse_date, tzcorrection_04)
{
	test_parse("+42");
	LONGS_EQUAL(151200, t->z);
}

TEST(parse_date, tzcorrection_05)
{
	test_parse("+4:0");
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date, tzcorrection_06)
{
	test_parse("+4:01");
	LONGS_EQUAL(14460, t->z);
}

TEST(parse_date, tzcorrection_07)
{
	test_parse("+4:30");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date, tzcorrection_08)
{
	test_parse("+401");
	LONGS_EQUAL(14460, t->z);
}

TEST(parse_date, tzcorrection_09)
{
	test_parse("+402");
	LONGS_EQUAL(14520, t->z);
}

TEST(parse_date, tzcorrection_10)
{
	test_parse("+430");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date, tzcorrection_11)
{
	test_parse("+0430");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date, tzcorrection_12)
{
	test_parse("+04:30");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date, tzcorrection_13)
{
	test_parse("+04:9");
	LONGS_EQUAL(14940, t->z);
}

TEST(parse_date, tzcorrection_14)
{
	test_parse("+04:09");
	LONGS_EQUAL(14940, t->z);
}

TEST(parse_date, tzcorrection_15)
{
	test_parse("+040915");
	LONGS_EQUAL(14955, t->z);
}

TEST(parse_date, tzcorrection_16)
{
	test_parse("-040916");
	LONGS_EQUAL(-14956, t->z);
}

TEST(parse_date, tzcorrection_17)
{
	test_parse("+04:09:15");
	LONGS_EQUAL(14955, t->z);
}

TEST(parse_date, tzcorrection_18)
{
	test_parse("-04:09:25");
	LONGS_EQUAL(-14965, t->z);
}

TEST(parse_date, tz_identifier_00)
{
	test_parse("01:00:03.12345 Europe/Amsterdam");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("Europe/Amsterdam", t->tz_info->name);
}

TEST(parse_date, tz_identifier_01)
{
	test_parse("01:00:03.12345 America/Indiana/Knox");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("America/Indiana/Knox", t->tz_info->name);
}

TEST(parse_date, tz_identifier_02)
{
	test_parse("2005-07-14 22:30:41 America/Los_Angeles");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(22, t->h);
	LONGS_EQUAL(30, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("America/Los_Angeles", t->tz_info->name);
}

TEST(parse_date, tz_identifier_03)
{
	test_parse("2005-07-14	22:30:41	America/Los_Angeles");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(22, t->h);
	LONGS_EQUAL(30, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("America/Los_Angeles", t->tz_info->name);
}

TEST(parse_date, tz_identifier_04)
{
	test_parse("Africa/Dar_es_Salaam");
	STRCMP_EQUAL("Africa/Dar_es_Salaam", t->tz_info->name);
}

TEST(parse_date, tz_identifier_05)
{
	test_parse("Africa/Porto-Novo");
	STRCMP_EQUAL("Africa/Porto-Novo", t->tz_info->name);
}

TEST(parse_date, tz_identifier_06)
{
	test_parse("America/Blanc-Sablon");
	STRCMP_EQUAL("America/Blanc-Sablon", t->tz_info->name);
}

TEST(parse_date, tz_identifier_07)
{
	test_parse("America/Port-au-Prince");
	STRCMP_EQUAL("America/Port-au-Prince", t->tz_info->name);
}

TEST(parse_date, tz_identifier_08)
{
	test_parse("America/Port_of_Spain");
	STRCMP_EQUAL("America/Port_of_Spain", t->tz_info->name);
}

TEST(parse_date, tz_identifier_09)
{
	test_parse("Antarctica/DumontDUrville");
	STRCMP_EQUAL("Antarctica/DumontDUrville", t->tz_info->name);
}

TEST(parse_date, tz_identifier_10)
{
	test_parse("Antarctica/McMurdo");
	STRCMP_EQUAL("Antarctica/McMurdo", t->tz_info->name);
}

TEST(parse_date, weeknr_00)
{
	test_parse("1995W051");
	LONGS_EQUAL(1995, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(29, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, weeknr_01)
{
	test_parse("2004W30");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(200, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, weeknr_02)
{
	test_parse("1995-W051");
	LONGS_EQUAL(1995, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(29, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, weeknr_03)
{
	test_parse("2004-W30");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(200, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, weeknr_04)
{
	test_parse("1995W05-1");
	LONGS_EQUAL(1995, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(29, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, weeknr_05)
{
	test_parse("1995-W05-1");
	LONGS_EQUAL(1995, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(29, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
}

TEST(parse_date, week_00)
{
	test_parse("this week");
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_01)
{
	test_parse("this week monday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_02)
{
	test_parse("this week tuesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(2, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_03)
{
	test_parse("this week wednesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(3, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_04)
{
	test_parse("thursday this week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(4, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_05)
{
	test_parse("friday this week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_06)
{
	test_parse("saturday this week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_07)
{
	test_parse("sunday this week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_08)
{
	test_parse("last week");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_09)
{
	test_parse("last week monday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_10)
{
	test_parse("last week tuesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(2, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_11)
{
	test_parse("last week wednesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(3, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_12)
{
	test_parse("thursday last week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(4, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_13)
{
	test_parse("friday last week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_14)
{
	test_parse("saturday last week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_15)
{
	test_parse("sunday last week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_16)
{
	test_parse("previous week");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_17)
{
	test_parse("previous week monday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_18)
{
	test_parse("previous week tuesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(2, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_19)
{
	test_parse("previous week wednesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(3, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_20)
{
	test_parse("thursday previous week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(4, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_21)
{
	test_parse("friday previous week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_22)
{
	test_parse("saturday previous week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_23)
{
	test_parse("sunday previous week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(-7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_24)
{
	test_parse("next week");
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_25)
{
	test_parse("next week monday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_26)
{
	test_parse("next week tuesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(2, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_27)
{
	test_parse("next week wednesday");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(3, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_28)
{
	test_parse("thursday next week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(4, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_29)
{
	test_parse("friday next week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_30)
{
	test_parse("saturday next week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, week_31)
{
	test_parse("sunday next week");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(0, t->relative.y);
	LONGS_EQUAL(0, t->relative.m);
	LONGS_EQUAL(7, t->relative.d);
	LONGS_EQUAL(0, t->relative.h);
	LONGS_EQUAL(0, t->relative.i);
	LONGS_EQUAL(0, t->relative.s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(2, t->relative.weekday_behavior);
}

TEST(parse_date, year_long_00)
{
	test_parse("+10000-01-01T00:00:00");
	LONGS_EQUAL(10000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_01)
{
	test_parse("+99999-01-01T00:00:00");
	LONGS_EQUAL(99999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_02)
{
	test_parse("+100000-01-01T00:00:00");
	LONGS_EQUAL(100000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_03)
{
	test_parse("+4294967296-01-01T00:00:00");
	LONGS_EQUAL(4294967296, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_04)
{
	test_parse("+9223372036854775807-01-01T00:00:00");
	LONGS_EQUAL(9223372036854775807, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_05)
{
	test_parse("-10000-01-01T00:00:00");
	LONGS_EQUAL(-10000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_06)
{
	test_parse("-99999-01-01T00:00:00");
	LONGS_EQUAL(-99999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_07)
{
	test_parse("-100000-01-01T00:00:00");
	LONGS_EQUAL(-100000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_08)
{
	test_parse("-4294967296-01-01T00:00:00");
	LONGS_EQUAL(-4294967296, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, year_long_09)
{
	test_parse("-9223372036854775807-01-01T00:00:00");
	LONGS_EQUAL(-9223372036854775807, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date, timetiny24_00)
{
	test_parse("1978-12-22T23");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, timetiny24_01)
{
	test_parse("T9");
	LONGS_EQUAL(9, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, timetiny24_02)
{
	test_parse("T23Z");
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, timetiny24_03)
{
	test_parse("1978-12-22T9");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(9, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, timetiny24_04)
{
	test_parse("1978-12-22T23Z");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, timetiny24_05)
{
	test_parse("1978-12-03T09-03");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(03, t->d);
	LONGS_EQUAL(9, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
	LONGS_EQUAL(-10800, t->z);
	POINTERS_EQUAL(NULL, t->tz_abbr);
}

TEST(parse_date, timetiny24_06)
{
	test_parse("T09-03");
	LONGS_EQUAL(9, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
	LONGS_EQUAL(-10800, t->z);
	POINTERS_EQUAL(NULL, t->tz_abbr);
}

TEST(parse_date, gh_124a)
{
	test_parse("@-9223372036854775808");
	LONGS_EQUAL(0x8000000000000000, t->relative.s);
}

TEST(parse_date, ozfuzz_27360)
{
	test_parse("@10000000000000000000 2SEC");
	LONGS_EQUAL(errors->error_count, 1);
	LONGS_EQUAL(errors->error_messages[0].error_code, TIMELIB_ERR_NUMBER_OUT_OF_RANGE);
}

TEST(parse_date, ozfuzz_33011)
{
	test_parse("@21641666666666669708sun");
	LONGS_EQUAL(errors->error_count, 1);
	LONGS_EQUAL(errors->error_messages[0].error_code, TIMELIB_ERR_NUMBER_OUT_OF_RANGE);
}

TEST(parse_date, ozfuzz_55330)
{
	test_parse("@-25666666666666663653");
	LONGS_EQUAL(errors->error_count, 1);
	LONGS_EQUAL(errors->error_messages[0].error_code, TIMELIB_ERR_NUMBER_OUT_OF_RANGE);
}

#define NBSP "\xC2\xA0"
#define NNBSP "\xE2\x80\xAF"

TEST(parse_date, icu_nnbsp_timetiny12)
{
	test_parse("8" NNBSP "pm");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, icu_nnbsp_timeshort12_01)
{
	test_parse("8:43" NNBSP "pm");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(43, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, icu_nnbsp_timeshort12_02)
{
	test_parse("8:43" NNBSP NNBSP "pm");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(43, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date, icu_nnbsp_timelong12)
{
	test_parse("8:43.43" NNBSP "pm");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(43, t->i);
	LONGS_EQUAL(43, t->s);
}

TEST(parse_date, icu_nnbsp_iso8601normtz_00)
{
	test_parse("T17:21:49" "GMT+0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL(21, t->i);
	LONGS_EQUAL(49, t->s);
	LONGS_EQUAL(9000, t->z);
}

TEST(parse_date, icu_nnbsp_iso8601normtz_01)
{
	test_parse("T17:21:49" NNBSP "GMT+0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL(21, t->i);
	LONGS_EQUAL(49, t->s);
	LONGS_EQUAL(9000, t->z);
}

TEST(parse_date, icu_nnbsp_iso8601normtz_02)
{
	test_parse("T17:21:49" NNBSP NNBSP "GMT+0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL(21, t->i);
	LONGS_EQUAL(49, t->s);
	LONGS_EQUAL(9000, t->z);
}

TEST(parse_date, icu_nnbsp_iso8601normtz_03)
{
	test_parse("T17:21:49" NBSP "GMT+0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL(21, t->i);
	LONGS_EQUAL(49, t->s);
	LONGS_EQUAL(9000, t->z);
}

TEST(parse_date, icu_nnbsp_iso8601normtz_04)
{
	test_parse("T17:21:49" NNBSP NBSP "GMT+0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL(21, t->i);
	LONGS_EQUAL(49, t->s);
	LONGS_EQUAL(9000, t->z);
}

TEST(parse_date, icu_nnbsp_iso8601normtz_05)
{
	test_parse("T17:21:49" NBSP NNBSP "GMT+0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL(21, t->i);
	LONGS_EQUAL(49, t->s);
	LONGS_EQUAL(9000, t->z);
}

TEST(parse_date, icu_nnbsp_iso8601normtz_06)
{
	test_parse("T17:21:49" NBSP NBSP "GMT+0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL(21, t->i);
	LONGS_EQUAL(49, t->s);
	LONGS_EQUAL(9000, t->z);
}

TEST(parse_date, icu_nnbsp_clf_01)
{
	test_parse("10/Oct/2000:13:55:36" NNBSP "-0230");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(55, t->i);
	LONGS_EQUAL(36, t->s);
	LONGS_EQUAL(-9000, t->z);
}

TEST(parse_date, cf1)
{
	test_parse("@9223372036854775807 9sec");
	LONGS_EQUAL(1, errors->error_count);
}
