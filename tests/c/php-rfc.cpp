#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include "timelib_private.h"
#include <string.h>

TEST_GROUP(php_rfc_interval)
{
	int             dummy_error;
	timelib_time   *changed;
	timelib_tzinfo *tzi;
	timelib_time   *t;
	timelib_rel_time *p = NULL;
	timelib_error_container *errors;

	TEST_SETUP()
	{
		t = changed = NULL;
	}

	void test_add_wall(const char *str, const char *interval, const char *tzid)
	{
		timelib_time     *b = NULL, *e = NULL;
		int               r = 0;

		tzi = timelib_parse_tzfile((char*) tzid, timelib_builtin_db(), &dummy_error);
		t = timelib_strtotime(str, strlen(str), NULL, timelib_builtin_db(), timelib_parse_tzfile);
//		printf("\n");
		timelib_update_ts(t, tzi);
		timelib_set_timezone(t, tzi);
		timelib_update_from_sse(t);

//		timelib_dump_date(t, 3);

		timelib_strtointerval(interval, strlen(interval), &b, &e, &p, &r, &errors);

		changed = timelib_add_wall(t, p);
//		timelib_dump_date(changed, 3);
	}

	void test_sub_wall(const char *str, const char *interval, const char *tzid)
	{
		timelib_time     *b = NULL, *e = NULL;
		int               r = 0;

		tzi = timelib_parse_tzfile((char*) tzid, timelib_builtin_db(), &dummy_error);
		t = timelib_strtotime(str, strlen(str), NULL, timelib_builtin_db(), timelib_parse_tzfile);
//		printf("\n");
		timelib_update_ts(t, tzi);
		timelib_set_timezone(t, tzi);
		timelib_update_from_sse(t);

//		timelib_dump_date(t, 3);

		timelib_strtointerval(interval, strlen(interval), &b, &e, &p, &r, &errors);

		changed = timelib_sub_wall(t, p);
//		timelib_dump_date(changed, 3);
	}

	TEST_TEARDOWN()
	{
		timelib_tzinfo_dtor(tzi);
		timelib_time_dtor(t);
		timelib_time_dtor(changed);
		timelib_rel_time_dtor(p);
		timelib_error_container_dtor(errors);
	}
};


TEST(php_rfc_interval, ba1)
{
	test_add_wall("@1289109599", "PT1S", "America/New_York");

	LONGS_EQUAL(1289109600, changed->sse);
	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL(1,    changed->h);
	LONGS_EQUAL(0,    changed->i);
	LONGS_EQUAL(0,    changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba2)
{
	test_add_wall("2010-11-06 04:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 4,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba3)
{
	test_add_wall("2010-11-06 04:30:00", "PT24H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 3,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba4)
{
	test_add_wall("2010-11-06 04:30:00", "PT23H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 2,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba5)
{
	test_add_wall("2010-11-06 04:30:00", "PT22H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 1,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba6)
{
	test_add_wall("2010-11-06 04:30:00", "PT21H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 1,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, ba7)
{
	test_add_wall("2010-11-06 01:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 1,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, ba8)
{
	test_add_wall("2010-11-06 01:30:00", "P1DT1H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 1,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba9)
{
	test_add_wall("2010-11-06 04:30:00", "PT25H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 4,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba10)
{
	test_add_wall("2010-11-06 03:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 3,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, ba11)
{
	test_add_wall("2010-11-06 02:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 2,   changed->h);
	LONGS_EQUAL(30,   changed->i);
	LONGS_EQUAL( 0,   changed->s);
	LONGS_EQUAL(0,    changed->dst);
	STRCMP_EQUAL("EST", changed->tz_abbr);
}

TEST(php_rfc_interval, bs1)
{
	test_sub_wall("@1289109600", "PT1S", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 7,   changed->d);
	LONGS_EQUAL( 1,   changed->h);
	LONGS_EQUAL(59,    changed->i);
	LONGS_EQUAL(59,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs2)
{
	test_sub_wall("2010-11-07 04:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 4,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs3)
{
	test_sub_wall("2010-11-07 03:30:00", "PT24H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 4,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs4)
{
	test_sub_wall("2010-11-07 02:30:00", "PT23H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 4,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs5)
{
	test_sub_wall("@1289111400", "PT22H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 4,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs6)
{
	test_sub_wall("2010-11-07 01:30:00", "PT21H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 4,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs7)
{
	test_sub_wall("2010-11-07 01:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 1,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs8)
{
	test_sub_wall("@1289111400", "P1DT1H", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 0,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs9)
{
	test_sub_wall("2010-11-07 03:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 3,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

TEST(php_rfc_interval, bs10)
{
	test_sub_wall("2010-11-07 02:30:00", "P1D", "America/New_York");

	LONGS_EQUAL(2010, changed->y);
	LONGS_EQUAL(11,   changed->m);
	LONGS_EQUAL( 6,   changed->d);
	LONGS_EQUAL( 2,   changed->h);
	LONGS_EQUAL(30,    changed->i);
	LONGS_EQUAL( 0,    changed->s);
	LONGS_EQUAL(1,    changed->dst);
	STRCMP_EQUAL("EDT", changed->tz_abbr);
}

