#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include "timelib_private.h"
#include <stdio.h>
#include <string.h>


TEST_GROUP(timelib_diff)
{
	timelib_time *t_now, *t_from, *t_to;
	timelib_tzinfo *tzi, *tzi_from, *tzi_to;
	timelib_rel_time *diff;

	void test_parse(const char *tzid, const char *from, const char *to)
	{
		int dummy_error;

		tzi = timelib_parse_tzfile(tzid, timelib_builtin_db(), &dummy_error);
		tzi_from = NULL;
		tzi_to = NULL;

		t_now = timelib_strtotime("now", sizeof("now"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_from = timelib_strtotime(from, strlen(from), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_to = timelib_strtotime(to, strlen(to), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		timelib_fill_holes(t_from, t_now, TIMELIB_NO_CLONE);
		timelib_fill_holes(t_to, t_now, TIMELIB_NO_CLONE);

		timelib_update_ts(t_from, tzi);
		timelib_update_ts(t_to, tzi);

		diff = timelib_diff(t_from, t_to);
	}

	void test_parse(const char *tzid_from, const char *tzid_to, const char *from, const char *to)
	{
		int dummy_error;

		tzi = NULL;
		tzi_from = timelib_parse_tzfile(tzid_from, timelib_builtin_db(), &dummy_error);
		tzi_to = timelib_parse_tzfile(tzid_to, timelib_builtin_db(), &dummy_error);

		t_now = timelib_strtotime("now", sizeof("now"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_from = timelib_strtotime(from, strlen(from), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_to = timelib_strtotime(to, strlen(to), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		timelib_fill_holes(t_from, t_now, TIMELIB_NO_CLONE);
		timelib_fill_holes(t_to, t_now, TIMELIB_NO_CLONE);

		timelib_update_ts(t_from, tzi_from);
		timelib_update_ts(t_to, tzi_to);

		diff = timelib_diff(t_from, t_to);
	}

	void test_parse(timelib_sll offset_from, const char *tzid_to, const char *from, const char *to)
	{
		int dummy_error;

		tzi = NULL;
		tzi_to = timelib_parse_tzfile(tzid_to, timelib_builtin_db(), &dummy_error);

		t_now = timelib_strtotime("now", sizeof("now"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_from = timelib_strtotime(from, strlen(from), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_to = timelib_strtotime(to, strlen(to), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		timelib_fill_holes(t_from, t_now, TIMELIB_NO_CLONE);
		timelib_fill_holes(t_to, t_now, TIMELIB_NO_CLONE);

		t_from->zone_type = TIMELIB_ZONETYPE_OFFSET;
		t_from->z = offset_from;

		timelib_update_ts(t_from, NULL);
		timelib_update_ts(t_to, tzi_to);

		diff = timelib_diff(t_from, t_to);
	}

	TEST_TEARDOWN()
	{
		if (tzi) {
			timelib_tzinfo_dtor(tzi);
		}
		if (tzi_from) {
			timelib_tzinfo_dtor(tzi_from);
		}
		if (tzi_to) {
			timelib_tzinfo_dtor(tzi_to);
		}
		timelib_time_dtor(t_from);
		timelib_time_dtor(t_to);
		timelib_time_dtor(t_now);
		timelib_rel_time_dtor(diff);
	}
};

#define CHECKDIFF(e_y,e_m,e_d,e_h,e_i,e_s,e_us) \
	LONGS_EQUAL(e_y, diff->y); \
	LONGS_EQUAL(e_m, diff->m); \
	LONGS_EQUAL(e_d, diff->d); \
	LONGS_EQUAL(e_h, diff->h); \
	LONGS_EQUAL(e_i, diff->i); \
	LONGS_EQUAL(e_s, diff->s); \
	LONGS_EQUAL(e_us, diff->us);

TEST(timelib_diff, php62326)
{
	test_parse("Europe/Berlin", "2012-06-01", "2012-12-01");

	CHECKDIFF(0, 6, 0, 0, 0, 0, 0)
}

TEST(timelib_diff, php65003_01)
{
	test_parse("Europe/Moscow", "13-03-01", "13-04-01");
	CHECKDIFF(0, 1, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php65003_02)
{
	test_parse("Europe/Moscow", "13-03-02", "13-04-02");
	CHECKDIFF(0, 1, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php68503_01)
{
	test_parse("Europe/London", "2015-02-01", "2015-05-01");
	CHECKDIFF(0, 3, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php68503_02)
{
	test_parse("UTC", "2015-02-01", "2015-05-01");
	CHECKDIFF(0, 3, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php69378_01)
{
	test_parse("UTC", "2015-04-02 09:55:47", "2014-02-16 02:00:00");
	CHECKDIFF(1, 1, 14, 7, 55, 47, 0);
	LONGS_EQUAL(1, diff->invert);
}

TEST(timelib_diff, php69378_02)
{
	test_parse("UTC", "2014-02-16 02:00:00", "2015-04-02 09:55:47");
	CHECKDIFF(1, 1, 17, 7, 55, 47, 0);
	LONGS_EQUAL(0, diff->invert);
}

TEST(timelib_diff, php71700_01)
{
	test_parse("UTC", "2016-03-01", "2016-03-31");
	CHECKDIFF(0, 0, 30, 0, 0, 0, 0);
}

TEST(timelib_diff, php71826_01)
{
	test_parse("Asia/Tokyo", "2015-02-01", "2015-03-01");
	CHECKDIFF(0, 1, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php71826_02)
{
	test_parse("Asia/Tokyo", "2015-03-01", "2015-03-29");
	CHECKDIFF(0, 0, 28, 0, 0, 0, 0);
}

TEST(timelib_diff, php71826_03)
{
	test_parse("Asia/Tokyo", "2015-04-01", "2015-04-29");
	CHECKDIFF(0, 0, 28, 0, 0, 0, 0);
}

TEST(timelib_diff, php74524)
{
	test_parse("Europe/Paris", "2017-04-03 22:29:15.079459", "2017-11-17 22:05:26.000000");
	CHECKDIFF(0, 7, 13, 23, 36, 10, 920541);
}

TEST(timelib_diff, php77032_01)
{
	test_parse("UTC", "2008-03-01", "2018-03-01");
	CHECKDIFF(10, 0, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php77032_02)
{
	test_parse("Europe/Amsterdam", "2008-03-01", "2018-03-01");
	CHECKDIFF(10, 0, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php76374_01)
{
	test_parse("Europe/Paris", "2017-10-01", "2017-01-01");
	CHECKDIFF(0, 9, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php76374_02)
{
	test_parse("Europe/Paris", "2017-10-01 12:00", "2017-01-01 12:00");
	CHECKDIFF(0, 9, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php78452)
{
	test_parse("Asia/Tehran", "2019-09-24 11:47:24", "2019-08-21 12:47:24");
	CHECKDIFF(0, 1, 2, 23, 0, 0, 0);
}

TEST(timelib_diff, php80974)
{
	test_parse("America/Toronto", "America/Vancouver", "2012-01-01 00:00", "2012-01-01 00:00");
	CHECKDIFF(0, 0, 0, 3, 0, 0, 0);
}

TEST(timelib_diff, php81273)
{
	test_parse("Australia/Sydney", "America/Los_Angeles", "2000-01-01 00:00:00.000000", "2000-01-01 00:00:00.000000");
	CHECKDIFF(0, 0, 0, 19, 0, 0, 0);
}

TEST(timelib_diff, php_gh8730)
{
	test_parse(-4 * SECS_PER_HOUR, "US/Eastern", "2022-06-08 09:15:00", "2022-06-08 09:15:00");
	CHECKDIFF(0, 0, 0, 0, 0, 0, 0);
}
