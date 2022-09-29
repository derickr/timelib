#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include "timelib_private.h"
#include <stdio.h>
#include <string.h>


TEST_GROUP(timelib_add)
{
	timelib_time *t_now, *t_from, *t_added;
	timelib_tzinfo *tzi, *tzi_from;
	timelib_rel_time *i_period;

	void test_add(const char *tzid_from, const char *from, const char *period)
	{
		int dummy_error;

		tzi_from = timelib_parse_tzfile(tzid_from, timelib_builtin_db(), &dummy_error);

		t_now = timelib_strtotime("now", sizeof("now"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_from = timelib_strtotime(from, strlen(from), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		timelib_fill_holes(t_from, t_now, TIMELIB_NO_CLONE);

		timelib_update_ts(t_from, tzi_from);

		if (period[0] == '-') {
			timelib_strtointerval(period + 1, strlen(period) - 1, NULL, NULL, &i_period, NULL, NULL);
			i_period->invert = 1;
		} else {
			timelib_strtointerval(period, strlen(period), NULL, NULL, &i_period, NULL, NULL);
		}

		t_added = timelib_add_wall(t_from, i_period);
	}

	void test_add(timelib_sll offset_from, const char *from, const char *period)
	{
		t_now = timelib_strtotime("now", sizeof("now"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t_from = timelib_strtotime(from, strlen(from), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		timelib_fill_holes(t_from, t_now, TIMELIB_NO_CLONE);
		t_from->zone_type = TIMELIB_ZONETYPE_OFFSET;
		t_from->z = offset_from;

		timelib_update_ts(t_from, NULL);

		if (period[0] == '-') {
			timelib_strtointerval(period + 1, strlen(period) - 1, NULL, NULL, &i_period, NULL, NULL);
			i_period->invert = 1;
		} else {
			timelib_strtointerval(period, strlen(period), NULL, NULL, &i_period, NULL, NULL);
		}

		t_added = timelib_add_wall(t_from, i_period);
	}

	TEST_TEARDOWN()
	{
		if (tzi_from) {
			timelib_tzinfo_dtor(tzi_from);
		}
		timelib_time_dtor(t_from);
		timelib_time_dtor(t_now);
		timelib_time_dtor(t_added);
		timelib_rel_time_dtor(i_period);
	}
};

#define CHECK_RES(e_y,e_m,e_d,e_h,e_i,e_s) \
	LONGS_EQUAL(e_y, t_added->y); \
	LONGS_EQUAL(e_m, t_added->m); \
	LONGS_EQUAL(e_d, t_added->d); \
	LONGS_EQUAL(e_h, t_added->h); \
	LONGS_EQUAL(e_i, t_added->i); \
	LONGS_EQUAL(e_s, t_added->s);

TEST(timelib_add, test_time_fall_type2_prev_type3_prev)
{
	test_add(-4 * SECS_PER_HOUR, "2010-10-04 02:18:48", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type2_prev_type3_dt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_add, test_time_fall_type2_prev_type3_redodt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_add, test_time_fall_type2_prev_type3_redost)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 07, 02, 14, 44);
}

TEST(timelib_add, test_time_fall_type2_prev_type3_st)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 07, 04, 16, 55);
}

TEST(timelib_add, test_time_fall_type2_prev_type3_post)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type2_dt_type3_prev)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type2_dt_type3_dt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 11, 07, 00, 15, 35);
}

TEST(timelib_add, test_time_fall_type2_dt_type3_redodt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_add, test_time_fall_type2_dt_type3_redost)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 07, 02, 14, 44);
}

TEST(timelib_add, test_time_fall_type2_dt_type3_st)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 07, 04, 16, 55);
}

TEST(timelib_add, test_time_fall_type2_dt_type3_post)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_add, test_time_fall_type2_redodt_type3_prev)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "-P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type2_redodt_type3_dt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "-P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_add, test_time_fall_type2_redodt_type3_redodt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "P0Y0M0DT0H3M2S");
	CHECK_RES(2010, 11, 07, 01, 15, 35);
}

TEST(timelib_add, test_time_fall_type2_redodt_type3_redost)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 02, 14, 44);
}

TEST(timelib_add, test_time_fall_type2_redodt_type3_st)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 04, 16, 55);
}

TEST(timelib_add, test_time_fall_type2_redodt_type3_post)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_add, test_time_fall_type2_redost_type3_prev)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "-P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 06, 17, 38, 28);
}

TEST(timelib_add, test_time_fall_type2_redost_type3_dt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "-P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_add, test_time_fall_type2_redost_type3_redodt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "-P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_add, test_time_fall_type2_redost_type3_redost)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "P0Y0M0DT0H2M10S");
	CHECK_RES(2010, 11, 07, 01, 16, 54);
}

TEST(timelib_add, test_time_fall_type2_redost_type3_st)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type2_redost_type3_post)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type2_st_type3_prev)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "-P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 06, 17, 38, 28);
}

TEST(timelib_add, test_time_fall_type2_st_type3_dt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "-P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_add, test_time_fall_type2_st_type3_redodt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "-P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_add, test_time_fall_type2_st_type3_redost)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "-P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_add, test_time_fall_type2_st_type3_st)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 11, 07, 05, 19, 56);
}

TEST(timelib_add, test_time_fall_type2_st_type3_post)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type2_post_type3_prev)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type2_post_type3_dt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "-P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_add, test_time_fall_type2_post_type3_redodt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "-P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_add, test_time_fall_type2_post_type3_redost)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "-P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_add, test_time_fall_type2_post_type3_st)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type2_post_type3_post)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-08 18:57:55", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type2_dtsec_type3_stsec)
{
	test_add(-4 * SECS_PER_HOUR, "2010-11-07 01:59:59", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 02, 00, 00);
}

TEST(timelib_add, test_time_fall_type2_stsec_type3_dtsec)
{
	test_add(-5 * SECS_PER_HOUR, "2010-11-07 01:00:00", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 00, 59, 59);
}


TEST(timelib_add, test_time_spring_type2_prev_type3_prev)
{
	test_add(-5 * SECS_PER_HOUR, "2010-02-11 02:18:48", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type2_prev_type3_st)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-13 18:38:28", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_add, test_time_spring_type2_prev_type3_dt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-13 18:38:28", "P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 14, 02, 16, 55);
}

TEST(timelib_add, test_time_spring_type2_prev_type3_post)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-13 18:38:28", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type2_st_type3_prev)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-14 00:10:20", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type2_st_type3_st)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-14 00:10:20", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 03, 14, 00, 15, 35);
}

TEST(timelib_add, test_time_spring_type2_st_type3_dt)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-14 00:10:20", "P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 02, 16, 55);
}

TEST(timelib_add, test_time_spring_type2_st_type3_post)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-14 00:10:20", "P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 15, 18, 59, 59);
}

TEST(timelib_add, test_time_spring_type2_dt_type3_prev)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-14 03:16:55", "-P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 13, 19, 38, 28);
}

TEST(timelib_add, test_time_spring_type2_dt_type3_st)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-14 03:16:55", "-P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 01, 10, 20);
}

TEST(timelib_add, test_time_spring_type2_dt_type3_dt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-14 03:16:55", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 03, 14, 05, 19, 56);
}

TEST(timelib_add, test_time_spring_type2_dt_type3_post)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-14 03:16:55", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type2_post_type3_prev)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-15 19:59:59", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type2_post_type3_st)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-15 19:59:59", "-P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 14, 01, 10, 20);
}

TEST(timelib_add, test_time_spring_type2_post_type3_dt)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-15 19:59:59", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_add, test_time_spring_type2_post_type3_post)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-15 18:57:55", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type2_stsec_type3_dtsec)
{
	test_add(-5 * SECS_PER_HOUR, "2010-03-14 01:59:59", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 02, 00, 00);
}

TEST(timelib_add, test_time_spring_type2_dtsec_type3_stsec)
{
	test_add(-4 * SECS_PER_HOUR, "2010-03-14 03:00:00", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 02, 59, 59);
}



TEST(timelib_add, test_time_fall_type3_prev_type2_prev)
{
	test_add("America/New_York", "2010-10-04 02:18:48", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type3_prev_type2_dt)
{
	test_add("America/New_York", "2010-11-06 18:38:28", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_add, test_time_fall_type3_prev_type2_redodt)
{
	test_add("America/New_York", "2010-11-06 18:38:28", "P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_add, test_time_fall_type3_prev_type2_redost)
{
	test_add("America/New_York", "2010-11-06 18:38:28", "P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_add, test_time_fall_type3_prev_type2_st)
{
	test_add("America/New_York", "2010-11-06 18:38:28", "P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_prev_type2_post)
{
	test_add("America/New_York", "2010-11-06 18:38:28", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type3_dt_type2_prev)
{
	test_add("America/New_York", "2010-11-07 00:10:20", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type3_dt_type2_dt)
{
	test_add("America/New_York", "2010-11-07 00:10:20", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 11, 07, 00, 15, 35);
}

TEST(timelib_add, test_time_fall_type3_dt_type2_redodt)
{
	test_add("America/New_York", "2010-11-07 00:10:20", "P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_add, test_time_fall_type3_dt_type2_redost)
{
	test_add("America/New_York", "2010-11-07 00:10:20", "P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_add, test_time_fall_type3_dt_type2_st)
{
	test_add("America/New_York", "2010-11-07 00:10:20", "P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_dt_type2_post)
{
	test_add("America/New_York", "2010-11-07 00:10:20", "P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_add, test_time_fall_type3_redodt_type2_prev)
{
	test_add("America/New_York", "2010-11-07 01:12:33", "-P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type3_redodt_type2_dt)
{
	test_add("America/New_York", "2010-11-07 01:12:33", "-P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_add, test_time_fall_type3_redodt_type2_redodt)
{
	test_add("America/New_York", "2010-11-07 01:12:33", "P0Y0M0DT0H3M2S");
	CHECK_RES(2010, 11, 07, 01, 15, 35);
}

TEST(timelib_add, test_time_fall_type3_redodt_type2_redost)
{
	test_add("America/New_York", "2010-11-07 01:12:33", "P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_add, test_time_fall_type3_redodt_type2_st)
{
	test_add("America/New_York", "2010-11-07 01:12:33", "P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redodt_type2_post)
{
	test_add("America/New_York", "2010-11-07 01:12:33", "P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_prev)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "-P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 06, 17, 38, 28);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_dt)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "-P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_redodt)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "-P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_redost)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT0H2M10S");
	CHECK_RES(2010, 11, 07, 01, 16, 54);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_0)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT0H2M11S");
	CHECK_RES(2010, 11, 07, 01, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_1)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 01, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_2)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 02, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_3)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT3H2M11S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_4)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT4H2M11S");
	CHECK_RES(2010, 11, 07, 04, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_23)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT23H2M11S");
	CHECK_RES(2010, 11, 07, 23, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_24)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT24H2M11S");
	CHECK_RES(2010, 11,  8, 00, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_25)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT25H2M11S");
	CHECK_RES(2010, 11,  8, 01, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_26)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT26H2M11S");
	CHECK_RES(2010, 11,  8, 02, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_1_0)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M1DT0H2M11S");
	CHECK_RES(2010, 11,  8, 01, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_1_1)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M1DT1H2M11S");
	CHECK_RES(2010, 11,  8, 02, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_st_1_2)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M1DT2H2M11S");
	CHECK_RES(2010, 11,  8, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_redost_type2_post)
{
	test_add("America/New_York", "2010-11-07 01:14:44", "P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type3_st_type2_prev)
{
	test_add("America/New_York", "2010-11-07 03:16:55", "-P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type3_st_type2_dt)
{
	test_add("America/New_York", "2010-11-07 03:16:55", "-P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_add, test_time_fall_type3_st_type2_redodt)
{
	test_add("America/New_York", "2010-11-07 03:16:55", "-P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_add, test_time_fall_type3_st_type2_redost)
{
	test_add("America/New_York", "2010-11-07 03:16:55", "-P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_add, test_time_fall_type3_st_type2_st)
{
	test_add("America/New_York", "2010-11-07 03:16:55", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 11, 07, 05, 19, 56);
}

TEST(timelib_add, test_time_fall_type3_st_type2_post)
{
	test_add("America/New_York", "2010-11-07 03:16:55", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type3_post_type2_prev)
{
	test_add("America/New_York", "2010-11-08 19:59:59", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_add, test_time_fall_type3_post_type2_dt)
{
	test_add("America/New_York", "2010-11-08 19:59:59", "-P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_add, test_time_fall_type3_post_type2_redodt)
{
	test_add("America/New_York", "2010-11-08 19:59:59", "-P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_add, test_time_fall_type3_post_type2_redost)
{
	test_add("America/New_York", "2010-11-08 19:59:59", "-P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_add, test_time_fall_type3_post_type2_st)
{
	test_add("America/New_York", "2010-11-08 19:59:59", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_add, test_time_fall_type3_post_type2_post)
{
	test_add("America/New_York", "2010-11-08 18:57:55", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_add, test_time_fall_type3_dtsec_type2_stsec)
{
	test_add("America/New_York", "2010-11-07 01:59:59", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 01, 00, 00);
}

TEST(timelib_add, test_time_fall_type3_stsec_type2_dtsec)
{
	test_add("America/New_York", "2010-11-07 01:00:00", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 00, 59, 59);
}


TEST(timelib_add, test_time_spring_type3_prev_type2_prev)
{
	test_add("America/New_York", "2010-02-11 02:18:48", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_prev_type2_st)
{
	test_add("America/New_York", "2010-03-13 18:38:28", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_add, test_time_spring_type3_prev_type2_dt)
{
	test_add("America/New_York", "2010-03-13 18:38:28", "P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_add, test_time_spring_type3_prev_type2_post)
{
	test_add("America/New_York", "2010-03-13 18:38:28", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_st_type2_prev)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_st_type2_st)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 03, 14, 00, 15, 35);
}

TEST(timelib_add, test_time_spring_type3_st_type2_dt)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_add, test_time_spring_type3_st_type2_post)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 15, 18, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_dt_type2_prev)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "-P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_dt_type2_st)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "-P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_add, test_time_spring_type3_dt_type2_dt)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 03, 14, 05, 19, 56);
}

TEST(timelib_add, test_time_spring_type3_dt_type2_post)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_post_type2_prev)
{
	test_add("America/New_York", "2010-03-15 19:59:59", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_post_type2_st)
{
	test_add("America/New_York", "2010-03-15 19:59:59", "-P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_add, test_time_spring_type3_post_type2_dt)
{
	test_add("America/New_York", "2010-03-15 19:59:59", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_add, test_time_spring_type3_post_type2_post)
{
	test_add("America/New_York", "2010-03-15 18:57:55", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_stsec_type2_dtsec)
{
	test_add("America/New_York", "2010-03-14 01:59:59", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 03, 00, 00);
}

TEST(timelib_add, test_time_spring_type3_dtsec_type2_stsec)
{
	test_add("America/New_York", "2010-03-14 03:00:00", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 01, 59, 59);
}


TEST(timelib_add, test_time_spring_type3_prev_type3_prev)
{
	test_add("America/New_York", "2010-02-11 02:18:48", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_prev_type3_st)
{
	test_add("America/New_York", "2010-03-13 18:38:28", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_add, test_time_spring_type3_prev_type3_dt)
{
	test_add("America/New_York", "2010-03-13 18:38:28", "P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_add, test_time_spring_type3_prev_type3_post)
{
	test_add("America/New_York", "2010-03-13 18:38:28", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_st_type3_prev)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_st_type3_st)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 03, 14, 00, 15, 35);
}

TEST(timelib_add, test_time_spring_type3_st_type3_dt)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_add, test_time_spring_type3_st_type3_post)
{
	test_add("America/New_York", "2010-03-14 00:10:20", "P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 15, 18, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_dt_type3_prev)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "-P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_dt_type3_st)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "-P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_add, test_time_spring_type3_dt_type3_dt)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 03, 14, 05, 19, 56);
}

TEST(timelib_add, test_time_spring_type3_dt_type3_post)
{
	test_add("America/New_York", "2010-03-14 03:16:55", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_post_type3_prev)
{
	test_add("America/New_York", "2010-03-15 19:59:59", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_add, test_time_spring_type3_post_type3_st)
{
	test_add("America/New_York", "2010-03-15 19:59:59", "-P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_add, test_time_spring_type3_post_type3_dt)
{
	test_add("America/New_York", "2010-03-15 19:59:59", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_add, test_time_spring_type3_post_type3_post)
{
	test_add("America/New_York", "2010-03-15 18:57:55", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_add, test_time_spring_type3_stsec_type3_dtsec)
{
	test_add("America/New_York", "2010-03-14 01:59:59", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 03, 00, 00);
}

TEST(timelib_add, test_time_spring_type3_dtsec_type3_stsec)
{
	test_add("America/New_York", "2010-03-14 03:00:00", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 01, 59, 59);
}
