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

	void setup(const char *base, const char *interval, timelib_sll us, bool invert)
	{
		int dummy_error;

		tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
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
		setup(base, interval, us, invert);
		changed = timelib_add_wall(t, p);
	}

	void test_sub_wall(const char *base, const char *interval, timelib_sll us, bool invert)
	{
		setup(base, interval, us, invert);
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
	CHECKRES(2021,  4,  5, 13, 59, 59, 999999, 1617631200);
}

TEST(timelib_interval, php80998_1b_sub)
{
	test_sub_wall("2021-04-05 11:00:00", "PT3H", 0L, INVERT);
	CHECKRES(2021,  4,  5, 14,  0,  0, 0, 1617631200);
}

TEST(timelib_interval, php80998_2a_sub)
{
	test_sub_wall("2021-04-05 11:00:00", "PT2H59M59S", 999999L, 0);
	CHECKRES(2021,  4,  5,  8,  0,  0, 1, 1617609601);
}

TEST(timelib_interval, php80998_2b_sub)
{
	test_sub_wall("2021-04-05 11:00:00", "PT3H", 0L, 0);
	CHECKRES(2021,  4,  5,  8,  0,  0, 0, 1617609600);
}

