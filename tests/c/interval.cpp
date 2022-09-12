#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <stdio.h>
#include <string.h>


TEST_GROUP(timelib_interval)
{
	timelib_tzinfo *tzi;
	timelib_time     *t;
	timelib_time     *changed;
	timelib_time     *b = NULL, *e = NULL;
	int               r = 0;
	timelib_rel_time *p = NULL;
	timelib_error_container *errors;

	void setup(const char *base, const char *tzid, const char *interval, timelib_sll us, bool invert)
	{
		int dummy_error;

		tzi = timelib_parse_tzfile((char*) tzid, timelib_builtin_db(), &dummy_error);
		t = timelib_strtotime(base, strlen(base), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		timelib_update_ts(t, tzi);
		timelib_set_timezone(t, tzi);
		timelib_update_from_sse(t);

		timelib_strtointerval(interval, strlen(interval), &b, &e, &p, &r, &errors);
		p->us = us;
		p->invert = invert;

	}

	void test_add_wall(const char *base, const char *interval, timelib_sll us, bool invert)
	{
		setup(base, "UTC", interval, us, invert);
		changed = timelib_add_wall(t, p);
	}

	void test_add_wall(const char *base, const char *tzid, const char *interval, timelib_sll us, bool invert)
	{
		setup(base, tzid, interval, us, invert);
		changed = timelib_add_wall(t, p);
	}

	void test_sub_wall(const char *base, const char *interval, timelib_sll us, bool invert)
	{
		setup(base, "UTC", interval, us, invert);
		changed = timelib_sub_wall(t, p);
	}

	TEST_TEARDOWN()
	{
		timelib_time_dtor(t);
		timelib_time_dtor(changed);
		timelib_tzinfo_dtor(tzi);
		timelib_rel_time_dtor(p);
		timelib_error_container_dtor(errors);
	}
};

#define CHECKRES(e_y,e_m,e_d,e_h,e_i,e_s,e_us,e_sse) \
	LONGS_EQUAL(e_y, changed->y); \
	LONGS_EQUAL(e_m, changed->m); \
	LONGS_EQUAL(e_d, changed->d); \
	LONGS_EQUAL(e_h, changed->h); \
	LONGS_EQUAL(e_i, changed->i); \
	LONGS_EQUAL(e_s, changed->s); \
	LONGS_EQUAL(e_us, changed->us); \
	LONGS_EQUAL(e_sse, changed->sse);

#define INVERT 1

TEST(timelib_interval, php80998_1a)
{
	test_add_wall("2021-04-05 14:00:00", "PT10799S", 999999L, INVERT);
	CHECKRES(2021,  4,  5, 11,  0,  0, 1, 1617620400);
}

TEST(timelib_interval, php80998_1b)
{
	test_add_wall("2021-04-05 14:00:00", "PT10800S", 0L, INVERT);
	CHECKRES(2021,  4,  5, 11,  0,  0, 0, 1617620400);
}

TEST(timelib_interval, php80998_2a)
{
	test_add_wall("2000-01-01 00:00:00", "PT1S", 500000L, INVERT);
	CHECKRES(1999, 12, 31, 23, 59, 58, 500000,  946684798);
}

TEST(timelib_interval, php80998_2b)
{
	test_add_wall("2000-01-01 00:00:00", "PT1S", 0L, INVERT);
	CHECKRES(1999, 12, 31, 23, 59, 59, 0, 946684799);
}

TEST(timelib_interval, php80998_1a_sub)
{
	test_sub_wall("2021-04-05 11:00:00", "PT2H59M59S", 999999L, INVERT);
	CHECKRES(2021,  4,  5, 13, 59, 59, 999999, 1617631199);
}

TEST(timelib_interval, php80998_1b_sub)
{
	test_sub_wall("2021-04-05 11:00:00", "PT3H", 0L, INVERT);
	CHECKRES(2021,  4,  5, 14,  0,  0, 0, 1617631200);
}

TEST(timelib_interval, php80998_2a_sub)
{
	test_sub_wall("2021-04-05 11:00:00", "PT2H59M59S", 999999L, 0);
	CHECKRES(2021,  4,  5,  8,  0,  0, 1, 1617609600);
}

TEST(timelib_interval, php80998_2b_sub)
{
	test_sub_wall("2021-04-05 11:00:00", "PT3H", 0L, 0);
	CHECKRES(2021,  4,  5,  8,  0,  0, 0, 1617609600);
}

TEST(timelib_interval, gh8964a)
{
	test_add_wall("2022-07-21 14:50:13", "PT0S", -5L, 0);
	CHECKRES(2022,  7, 21, 14, 50, 12, 999995L, 1658415012);
}

TEST(timelib_interval, gh8964b)
{
	test_add_wall("2022-07-21 14:50:13", "PT0S", -5L, INVERT);
	CHECKRES(2022,  7, 21, 14, 50, 13, 5L, 1658415013);
}

TEST(timelib_interval, gh8964c)
{
	test_sub_wall("2022-07-21 14:50:13", "PT0S", -5L, 0);
	CHECKRES(2022,  7, 21, 14, 50, 13, 5L, 1658415013);
}

TEST(timelib_interval, gh8964d)
{
	test_sub_wall("2022-07-21 14:50:13", "PT0S", -5L, INVERT);
	CHECKRES(2022,  7, 21, 14, 50, 12, 999995L, 1658415012);
}

TEST(timelib_interval, gh8964e)
{
	test_sub_wall("2022-07-21 15:00:10", "PT0S", 900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0,  9, 100000L, 1658415609);
}

TEST(timelib_interval, gh8964f)
{
	test_add_wall("2022-07-21 15:00:10", "PT0S", 900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0, 10, 900000L, 1658415610);
}

TEST(timelib_interval, gh8964g)
{
	test_sub_wall("2022-07-21 15:00:10", "PT0S", -900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0, 10, 900000L, 1658415610);
}

TEST(timelib_interval, gh8964h)
{
	test_add_wall("2022-07-21 15:00:10", "PT0S", -900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0,  9, 100000L, 1658415609);
}

TEST(timelib_interval, gh8964i)
{
	test_sub_wall("2022-07-21 15:00:10", "PT1S", -900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0,  9, 900000L, 1658415609);
}

TEST(timelib_interval, gh8964j)
{
	test_add_wall("2022-07-21 15:00:10", "PT1S", -900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0, 10, 100000L, 1658415610);
}

TEST(timelib_interval, gh8964k)
{
	test_sub_wall("2022-07-21 15:00:10", "PT2S", -900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0,  8, 900000L, 1658415608);
}

TEST(timelib_interval, gh8964l)
{
	test_add_wall("2022-07-21 15:00:10", "PT2S", -900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0, 11, 100000L, 1658415611);
}

TEST(timelib_interval, gh8964m)
{
	test_sub_wall("2022-07-21 15:00:09.100000", "PT0S", -900000L, 0);
	CHECKRES(2022,  7, 21, 15,  0, 10, 0L, 1658415610);
}

TEST(timelib_interval, gh9106a)
{
	test_add_wall("2020-01-01 00:00:00.000000", "PT1S", 500000L, 0);
	CHECKRES(2020,  1,  1,  0,  0,  1, 500000L, 1577836801);
}

TEST(timelib_interval, gh9106b)
{
	test_add_wall("2020-01-01 00:00:01.500000", "PT1S", 500000L, 0);
	CHECKRES(2020,  1,  1,  0,  0,  3,      0L, 1577836803);
}

TEST(timelib_interval, gh9106c)
{
	test_add_wall("2020-01-01 00:00:03.600000", "PT1S", 500000L, 0);
	CHECKRES(2020,  1,  1,  0,  0,  5, 100000L, 1577836805);
}

TEST(timelib_interval, gh8860a)
{
	test_add_wall("2022-10-30 01:00:00", "Europe/Amsterdam", "PT0H", 0, 0);
	CHECKRES(2022, 10, 30,  1,  0,  0,      0L, 1667084400);
	LONGS_EQUAL(changed->z, 7200);
}

TEST(timelib_interval, gh8860b)
{
	test_add_wall("2022-10-30 01:00:00", "Europe/Amsterdam", "PT1H", 0, 0);
	CHECKRES(2022, 10, 30,  2,  0,  0,      0L, 1667088000);
	LONGS_EQUAL(changed->z, 7200);
}

TEST(timelib_interval, gh8860c)
{
	test_add_wall("2022-10-30 02:00:00", "Europe/Amsterdam", "PT1H", 0, 0);
	CHECKRES(2022, 10, 30,  3,  0,  0,      0L, 1667095200);
	LONGS_EQUAL(changed->z, 3600);
}

TEST(timelib_interval, gh8860d)
{
	test_add_wall("2022-10-30 02:00:00", "Europe/Amsterdam", "PT3600S", 0, 0);
	CHECKRES(2022, 10, 30,  3,  0,  0,      0L, 1667095200);
	LONGS_EQUAL(changed->z, 3600);
}

TEST(timelib_interval, long_positive_interval)
{
	test_add_wall("2000-01-01 00:00:01.500000", "P135000D", 0, 0);
	CHECKRES(2369,  8,  14,  0,  0,  1, 500000L, 12610684801);
}

TEST(timelib_interval, long_negative_interval)
{
	test_add_wall("2000-01-01 00:00:01.500000", "P135000D", 0, INVERT);
	CHECKRES(1630,  5,  20,  0,  0,  1, 500000L, -10717315199);
}

TEST(timelib_interval, leap_year_crossing_1)
{
	test_add_wall("2000-02-28 00:00:01.500000", "P1D", 0, 0);
	CHECKRES(2000,  2,  29,  0,  0,  1, 500000L, 951782401);
}

TEST(timelib_interval, leap_year_crossing_2)
{
	test_add_wall("2000-02-29 00:00:01.500000", "P1D", 0, INVERT);
	CHECKRES(2000,  2,  28,  0,  0,  1, 500000L, 951696001);
}

TEST(timelib_interval, leap_year_crossing_3)
{
	test_add_wall("2000-01-28 00:00:01.500000", "P32D", 0, 0);
	CHECKRES(2000,  2,  29,  0,  0,  1, 500000L, 951782401);
}

TEST(timelib_interval, leap_year_crossing_4)
{
	test_add_wall("2000-02-29 00:00:01.500000", "P32D", 0, INVERT);
	CHECKRES(2000,  1,  28,  0,  0,  1, 500000L, 949017601);
}

TEST(timelib_interval, non_leap_year_crossing_1)
{
	test_add_wall("2001-02-28 00:00:01.500000", "P1D", 0, 0);
	CHECKRES(2001,  3,  1,  0,  0,  1, 500000L, 983404801);
}

TEST(timelib_interval, non_leap_year_crossing_2)
{
	test_add_wall("2001-03-01 00:00:01.500000", "P1D", 0, INVERT);
	CHECKRES(2001,  2,  28,  0,  0,  1, 500000L, 983318401);
}

TEST(timelib_interval, non_leap_year_crossing_3)
{
	test_add_wall("2001-01-28 00:00:01.500000", "P32D", 0, 0);
	CHECKRES(2001,  3,  1,  0,  0,  1, 500000L, 983404801);
}

TEST(timelib_interval, non_leap_year_crossing_4)
{
	test_add_wall("2001-03-01 00:00:01.500000", "P32D", 0, INVERT);
	CHECKRES(2001,  1,  28,  0,  0,  1, 500000L, 980640001);
}
