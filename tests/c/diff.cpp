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

	void test_parse(timelib_sll offset_from, bool dst_from, timelib_sll offset_to, bool dst_to, const char *from, const char *to)
	{
		t_now = timelib_strtotime("now", sizeof("now"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_from = timelib_strtotime(from, strlen(from), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_to = timelib_strtotime(to, strlen(to), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		timelib_fill_holes(t_from, t_now, TIMELIB_NO_CLONE);
		timelib_fill_holes(t_to, t_now, TIMELIB_NO_CLONE);

		t_from->zone_type = TIMELIB_ZONETYPE_OFFSET;
		t_from->z = offset_from;
		t_from->dst = dst_from;

		t_to->zone_type = TIMELIB_ZONETYPE_OFFSET;
		t_to->z = offset_to;
		t_to->dst = dst_to;

		timelib_update_ts(t_from, NULL);
		timelib_update_ts(t_to, NULL);

		diff = timelib_diff(t_from, t_to);
	}


	void test_parse(timelib_sll offset_from, timelib_sll offset_to, const char *from, const char *to)
	{
		test_parse(offset_from, false, offset_to, false, from, to);
	}

	void dump_data(void)
	{
		printf("%s", "\n");
		timelib_dump_date(t_from, -1);
		timelib_dump_date(t_to, -1);
		timelib_dump_rel_time(diff);
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

TEST(timelib_diff, php_81263a)
{
	test_parse("Europe/Berlin", "UTC", "2020-07-19 18:30:00", "2020-07-19 16:30:00");
	CHECKDIFF(0, 0, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php_81263b)
{
	test_parse("UTC", "Europe/Berlin", "2020-07-19 16:30:00", "2020-07-19 18:30:00");
	CHECKDIFF(0, 0, 0, 0, 0, 0, 0);
}

TEST(timelib_diff, php_80974a)
{
	test_parse("America/Toronto", "America/Vancouver", "2012-01-01 00:00:00", "2012-01-01 00:00:00");
	CHECKDIFF(0, 0, 0, 3, 0, 0, 0);
}

TEST(timelib_diff, php_80974b)
{
	test_parse("America/Vancouver", "America/Toronto", "2012-01-01 00:00:00", "2012-01-01 00:00:00");
	CHECKDIFF(0, 0, 0, 3, 0, 0, 0);
}

TEST(timelib_diff, test_time_spring_type2_prev_type2_prev)
{
	test_parse(-4 * SECS_PER_HOUR, false, -4 * SECS_PER_HOUR, false, "2010-03-13 18:38:28", "2010-02-11 02:18:48");
	CHECKDIFF(0, 1, 2, 16, 19, 40, 0);
}

TEST(timelib_diff, test_time_spring_type2_prev_type2_st)
{
	test_parse(-4 * SECS_PER_HOUR, false, -4 * SECS_PER_HOUR, false, "2010-03-14 00:10:20", "2010-03-13 18:38:28");
	CHECKDIFF(0, 0, 0, 5, 31, 52, 0);
}

TEST(timelib_diff, test_time_spring_type2_prev_type2_dt)
{
	test_parse(-4 * SECS_PER_HOUR, true, -4 * SECS_PER_HOUR, false, "2010-03-14 03:16:55", "2010-03-13 18:38:28");
	CHECKDIFF(0, 0, 0, 7, 38, 27, 0);
}

TEST(timelib_diff, test_time_spring_type2_prev_type2_post)
{
	test_parse(-4 * SECS_PER_HOUR, true, -4 * SECS_PER_HOUR, false, "2010-03-15 19:59:59", "2010-03-13 18:38:28");
	CHECKDIFF(0, 0, 2, 0, 21, 31, 0);
}

TEST(timelib_diff, datetime_and_daylight_saving_time_type1_fd1)
{
	test_parse(-4 * SECS_PER_HOUR, -5 * SECS_PER_HOUR, "2010-03-14 03:00:00", "2010-03-14 01:59:59");
	CHECKDIFF(0, 0, 0, 0, 0, 1, 0);
}

TEST(timelib_diff, datetime_and_daylight_saving_time_type1_fd2)
{
	test_parse(-4 * SECS_PER_HOUR, -5 * SECS_PER_HOUR, "2010-03-14 04:30:00", "2010-03-13 04:30:00");
	CHECKDIFF(0, 0, 0, 23, 0, 0, 0);
}

TEST(timelib_diff, datetime_and_daylight_saving_time_type1_fd3)
{
	test_parse(-4 * SECS_PER_HOUR, -5 * SECS_PER_HOUR, "2010-03-14 03:30:00", "2010-03-13 04:30:00");
	CHECKDIFF(0, 0, 0, 22, 0, 0, 0);
}

TEST(timelib_diff, datetime_and_daylight_saving_time_type1_fd4)
{
	test_parse(-5 * SECS_PER_HOUR, -5 * SECS_PER_HOUR, "2010-03-14 01:30:00", "2010-03-13 04:30:00");
	CHECKDIFF(0, 0, 0, 21, 0, 0, 0);
}

TEST(timelib_diff, datetime_and_daylight_saving_time_type1_fd5)
{
	test_parse(-5 * SECS_PER_HOUR, -5 * SECS_PER_HOUR, "2010-03-14 01:30:00", "2010-03-13 01:30:00");
	CHECKDIFF(0, 0, 1,  0, 0, 0, 0);
}

TEST(timelib_diff, datetime_and_daylight_saving_time_type1_fd6)
{
	test_parse(-4 * SECS_PER_HOUR, -5 * SECS_PER_HOUR, "2010-03-14 03:30:00", "2010-03-13 03:30:00");
	CHECKDIFF(0, 0, 0, 23, 0, 0, 0);
}

TEST(timelib_diff, datetime_and_daylight_saving_time_type1_fd7)
{
	test_parse(-4 * SECS_PER_HOUR, -5 * SECS_PER_HOUR, "2010-03-14 03:30:00", "2010-03-13 02:30:00");
	CHECKDIFF(0, 0, 1,  0, 0, 0, 0);
}

TEST(timelib_diff, php_gh9382)
{
	test_parse(+2 * SECS_PER_HOUR, "Europe/Berlin", "2022-08-01 07:00:00", "2022-08-01 07:00:00");
	CHECKDIFF(0, 0, 0,  0, 0, 0, 0);
}

TEST(timelib_diff, gh75)
{
	test_parse("PRC", "PRC", "2020-02-01 00:00:00", "2020-03-01 00:00:00");
	CHECKDIFF(0, 1, 0,  0, 0, 0, 0);
}

TEST(timelib_diff, php_gh9699)
{
	test_parse("America/Los_Angeles", "UTC", "2022-10-09 02:41:54.515330", "2022-10-10 08:41:54.534620");
	CHECKDIFF( 0, 0, 0, 23,  0,  0, 19290);
}

TEST(timelib_diff, php_gh9866)
{
	test_parse("America/Chicago", "America/New_York", "2000-11-01 09:29:22.907606", "2022-06-06 11:00:00");
	CHECKDIFF(21, 7, 4, 23, 30, 37, 92394);
}

TEST(timelib_diff, php_gh9880a)
{
	test_parse("America/Los_Angeles", "America/New_York", "2022-11-02 12:18:15", "2022-12-24 13:00:00");
	LONGS_EQUAL(51, diff->days);
	LONGS_EQUAL( 0, diff->invert);
	CHECKDIFF(0, 1, 21, 22, 41, 45, 0);
}

TEST(timelib_diff, php_gh9880b)
{
	test_parse("America/New_York", "America/Los_Angeles", "2022-12-24 13:00:00", "2022-11-02 12:18:15");
	LONGS_EQUAL(51, diff->days);
	LONGS_EQUAL( 1, diff->invert);
	CHECKDIFF(0, 1, 21, 22, 41, 45, 0);
}

TEST(timelib_diff, test_time_fall_type3_redodt_type3_st_fwd)
{
	test_parse("America/New_York", "2010-11-07 01:12:33", "2010-11-07 03:16:55");
	CHECKDIFF(0, 0, 0, 3, 4, 22, 0);
}

TEST(timelib_diff, test_time_fall_type3_redodt_type3_st_rev)
{
	test_parse("America/New_York", "2010-11-07 03:16:55", "2010-11-07 01:12:33");
	CHECKDIFF(0, 0, 0, 3, 4, 22, 0);
}

TEST(timelib_diff, test_time_fall_type3_dtsec_type3_stsec)
{
	test_parse("America/New_York", "2010-11-07 01:59:59", "2010-11-07 01:00:00");
	CHECKDIFF(0, 0, 0, 0, 59, 59, 0);
}

TEST(timelib_diff, test_time_fall_type2_dtsec_type2_stsec)
{
	test_parse("America/New_York", "2010-11-07 01:59:59 EDT", "2010-11-07 01:00:00 EST");
	CHECKDIFF(0, 0, 0, 0, 0, 1, 0);
}

TEST(timelib_diff, test_time_fall_type3_stsec_type3_dtsec)
{
	test_parse("America/New_York", "2010-11-07 01:00:00", "2010-11-07 01:59:59");
	CHECKDIFF(0, 0, 0, 0, 59, 59, 0);
}

TEST(timelib_diff, test_time_fall_type2_stsec_type2_dtsec)
{
	test_parse("America/New_York", "2010-11-07 01:00:00 EST", "2010-11-07 01:59:59 EDT");
	CHECKDIFF(0, 0, 0, 0, 0, 1, 0);
}
