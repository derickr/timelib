#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include "timelib_private.h"
#include <stdio.h>
#include <string.h>


TEST_GROUP(timelib_sub)
{
	timelib_time *t_now, *t_from, *t_subbed;
	timelib_tzinfo *tzi, *tzi_from;
	timelib_rel_time *i_period;

	void test_sub(const char *tzid_from, const char *from, const char *period)
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

		t_subbed = timelib_sub_wall(t_from, i_period);
	}

	void test_sub(timelib_sll offset_from, const char *from, const char *period)
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

		t_subbed = timelib_sub_wall(t_from, i_period);
	}

	TEST_TEARDOWN()
	{
		if (tzi_from) {
			timelib_tzinfo_dtor(tzi_from);
		}
		timelib_time_dtor(t_from);
		timelib_time_dtor(t_now);
		timelib_time_dtor(t_subbed);
		timelib_rel_time_dtor(i_period);
	}
};

#define CHECK_RES(e_y,e_m,e_d,e_h,e_i,e_s) \
	LONGS_EQUAL(e_y, t_subbed->y); \
	LONGS_EQUAL(e_m, t_subbed->m); \
	LONGS_EQUAL(e_d, t_subbed->d); \
	LONGS_EQUAL(e_h, t_subbed->h); \
	LONGS_EQUAL(e_i, t_subbed->i); \
	LONGS_EQUAL(e_s, t_subbed->s);

TEST(timelib_sub, test_time_fall_type2_prev_type3_prev)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 10, 04, 02, 18, 48);
}

TEST(timelib_sub, test_time_fall_type2_prev_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_sub, test_time_fall_type2_prev_type3_redodt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_sub, test_time_fall_type2_prev_type3_redost)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 06, 17, 38, 28);
}

TEST(timelib_sub, test_time_fall_type2_prev_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 06, 17, 38, 28);
}

TEST(timelib_sub, test_time_fall_type2_prev_type3_post)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_sub, test_time_fall_type2_dt_type3_prev)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type2_dt_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 00:15:35", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type2_dt_type3_redodt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type2_dt_type3_redost)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_sub, test_time_fall_type2_dt_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_sub, test_time_fall_type2_dt_type3_post)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_sub, test_time_fall_type2_redodt_type3_prev)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "-P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type2_redodt_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "-P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type2_redodt_type3_redodt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 01:15:35", "P0Y0M0DT0H3M2S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type2_redodt_type3_redost)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_sub, test_time_fall_type2_redodt_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_sub, test_time_fall_type2_redodt_type3_post)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_sub, test_time_fall_type2_redost_type3_prev)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "-P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 07, 02, 14, 44);
}

TEST(timelib_sub, test_time_fall_type2_redost_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "-P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 07, 02, 14, 44);
}

TEST(timelib_sub, test_time_fall_type2_redost_type3_redodt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "-P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 02, 14, 44);
}

TEST(timelib_sub, test_time_fall_type2_redost_type3_redost)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 01:16:54", "P0Y0M0DT0H2M10S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type2_redost_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type2_redost_type3_post)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type2_st_type3_prev)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "-P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 07, 04, 16, 55);
}

TEST(timelib_sub, test_time_fall_type2_st_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "-P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 07, 04, 16, 55);
}

TEST(timelib_sub, test_time_fall_type2_st_type3_redodt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "-P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 04, 16, 55);
}

TEST(timelib_sub, test_time_fall_type2_st_type3_redost)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "-P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type2_st_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 05:19:56", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type2_st_type3_post)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type2_post_type3_prev)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-06 18:38:28", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_sub, test_time_fall_type2_post_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 00:10:20", "-P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_sub, test_time_fall_type2_post_type3_redodt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 01:12:33", "-P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_sub, test_time_fall_type2_post_type3_redost)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 01:14:44", "-P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_sub, test_time_fall_type2_post_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 03:16:55", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_sub, test_time_fall_type2_post_type3_post)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-08 19:59:59", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 11,  8, 18, 57, 55);
}

TEST(timelib_sub, test_time_fall_type2_dtsec_type3_stsec)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-11-07 01:00:00", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 00, 59, 59);
}

TEST(timelib_sub, test_time_fall_type2_stsec_type3_dtsec)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-11-07 01:59:59", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 02, 00, 00);
}



TEST(timelib_sub, test_time_fall_type3_prev_type3_prev)
{
	test_sub("America/New_York", "2010-11-06 18:38:28", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 10, 04, 02, 18, 48);
}

TEST(timelib_sub, test_time_fall_type3_prev_type3_dt)
{
	test_sub("America/New_York", "2010-11-07 00:10:20", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_sub, test_time_fall_type3_prev_type3_redodt)
{
	test_sub("America/New_York", "2010-11-07 01:12:33", "P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_sub, test_time_fall_type3_prev_type3_redost)
{
	test_sub("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 06, 17, 38, 28);
}

TEST(timelib_sub, test_time_fall_type3_prev_type3_st)
{
	test_sub("America/New_York", "2010-11-07 03:16:55", "P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_sub, test_time_fall_type3_prev_type3_post)
{
	test_sub("America/New_York", "2010-11-08 19:59:59", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11, 06, 18, 38, 28);
}

TEST(timelib_sub, test_time_fall_type3_dt_type3_prev)
{
	test_sub("America/New_York", "2010-11-06 18:38:28", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type3_dt_type3_dt)
{
	test_sub("America/New_York", "2010-11-07 00:15:35", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type3_dt_type3_redodt)
{
	test_sub("America/New_York", "2010-11-07 01:12:33", "P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type3_dt_type3_redost)
{
	test_sub("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 06, 23, 10, 20);
}

TEST(timelib_sub, test_time_fall_type3_dt_type3_st)
{
	test_sub("America/New_York", "2010-11-07 03:16:55", "P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type3_dt_type3_post)
{
	test_sub("America/New_York", "2010-11-08 19:59:59", "P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11, 07, 00, 10, 20);
}

TEST(timelib_sub, test_time_fall_type3_redodt_type3_prev)
{
	test_sub("America/New_York", "2010-11-06 18:38:28", "-P0Y0M0DT6H34M5S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type3_redodt_type3_dt)
{
	test_sub("America/New_York", "2010-11-07 00:10:20", "-P0Y0M0DT1H2M13S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type3_redodt_type3_redodt)
{
	test_sub("America/New_York", "2010-11-07 01:15:35", "P0Y0M0DT0H3M2S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type3_redodt_type3_redost)
{
	test_sub("America/New_York", "2010-11-07 01:14:44", "P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 00, 12, 33);
}

TEST(timelib_sub, test_time_fall_type3_redodt_type3_st)
{
	test_sub("America/New_York", "2010-11-07 03:16:55", "P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type3_redodt_type3_post)
{
	test_sub("America/New_York", "2010-11-08 19:59:59", "P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11, 07, 01, 12, 33);
}

TEST(timelib_sub, test_time_fall_type3_redost_type3_prev)
{
	test_sub("America/New_York", "2010-11-06 18:38:28", "-P0Y0M0DT7H36M16S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type3_redost_type3_dt)
{
	test_sub("America/New_York", "2010-11-07 00:10:20", "-P0Y0M0DT2H4M24S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type3_redost_type3_redodt)
{
	test_sub("America/New_York", "2010-11-07 01:12:33", "-P0Y0M0DT1H2M11S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type3_redost_type3_redost)
{
	test_sub("America/New_York", "2010-11-07 01:16:54", "P0Y0M0DT0H2M10S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type3_redost_type3_st)
{
	test_sub("America/New_York", "2010-11-07 03:16:55", "P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type3_redost_type3_post)
{
	test_sub("America/New_York", "2010-11-08 19:59:59", "P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11, 07, 01, 14, 44);
}

TEST(timelib_sub, test_time_fall_type3_st_type3_prev)
{
	test_sub("America/New_York", "2010-11-06 18:38:28", "-P0Y0M0DT9H38M27S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type3_st_type3_dt)
{
	test_sub("America/New_York", "2010-11-07 00:10:20", "-P0Y0M0DT4H6M35S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type3_st_type3_redodt)
{
	test_sub("America/New_York", "2010-11-07 01:12:33", "-P0Y0M0DT3H4M22S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type3_st_type3_redost)
{
	test_sub("America/New_York", "2010-11-07 01:14:44", "-P0Y0M0DT2H2M11S");
	CHECK_RES(2010, 11, 07, 02, 16, 55);
}

TEST(timelib_sub, test_time_fall_type3_st_type3_st)
{
	test_sub("America/New_York", "2010-11-07 05:19:56", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type3_st_type3_post)
{
	test_sub("America/New_York", "2010-11-08 19:59:59", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11, 07, 03, 16, 55);
}

TEST(timelib_sub, test_time_fall_type3_post_type3_prev)
{
	test_sub("America/New_York", "2010-11-06 18:38:28", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_sub, test_time_fall_type3_post_type3_dt)
{
	test_sub("America/New_York", "2010-11-07 00:10:20", "-P0Y0M1DT20H49M39S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_sub, test_time_fall_type3_post_type3_redodt)
{
	test_sub("America/New_York", "2010-11-07 01:12:33", "-P0Y0M1DT19H47M26S");
	CHECK_RES(2010, 11,  8, 20, 59, 59);
}

TEST(timelib_sub, test_time_fall_type3_post_type3_redost)
{
	test_sub("America/New_York", "2010-11-07 01:14:44", "-P0Y0M1DT18H45M15S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_sub, test_time_fall_type3_post_type3_st)
{
	test_sub("America/New_York", "2010-11-07 03:16:55", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 11,  8, 19, 59, 59);
}

TEST(timelib_sub, test_time_fall_type3_post_type3_post)
{
	test_sub("America/New_York", "2010-11-08 19:59:59", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 11,  8, 18, 57, 55);
}

TEST(timelib_sub, test_time_fall_type3_dtsec_type3_stsec)
{
	test_sub("America/New_York", "2010-11-07 01:00:00", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 00, 59, 59);
}

TEST(timelib_sub, test_time_fall_type3_stsec_type3_dtsec)
{
	test_sub("America/New_York", "2010-11-07 01:59:59", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 11, 07, 01, 00, 00);
}


TEST(timelib_sub, test_time_spring_type2_prev_type3_prev)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-13 18:38:28", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 02, 11, 02, 18, 48);
}

TEST(timelib_sub, test_time_spring_type2_prev_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-14 00:10:20", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_sub, test_time_spring_type2_prev_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-14 03:16:55", "P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 13, 19, 38, 28);
}

TEST(timelib_sub, test_time_spring_type2_prev_type3_post)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-15 19:59:59", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_sub, test_time_spring_type2_st_type3_prev)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-13 18:38:28", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_sub, test_time_spring_type2_st_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-14 00:15:35", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_sub, test_time_spring_type2_st_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-14 03:16:55", "P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 01, 10, 20);
}

TEST(timelib_sub, test_time_spring_type2_st_type3_post)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-15 19:59:59", "P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 14, 01, 10, 20);
}

TEST(timelib_sub, test_time_spring_type2_dt_type3_prev)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-13 18:38:28", "-P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 14, 02, 16, 55);
}

TEST(timelib_sub, test_time_spring_type2_dt_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-14 00:10:20", "-P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 02, 16, 55);
}

TEST(timelib_sub, test_time_spring_type2_dt_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-14 05:19:56", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_sub, test_time_spring_type2_dt_type3_post)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-15 19:59:59", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_sub, test_time_spring_type2_post_type3_prev)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-13 18:38:28", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_sub, test_time_spring_type2_post_type3_st)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-14 00:10:20", "-P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 15, 18, 59, 59);
}

TEST(timelib_sub, test_time_spring_type2_post_type3_dt)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-14 03:16:55", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_sub, test_time_spring_type2_post_type3_post)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-15 19:59:59", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 03, 15, 18, 57, 55);
}

TEST(timelib_sub, test_time_spring_type2_stsec_type3_dtsec)
{
	test_sub(-4 * SECS_PER_HOUR, "2010-03-14 03:00:00", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 02, 59, 59);
}

TEST(timelib_sub, test_time_spring_type2_dtsec_type3_stsec)
{
	test_sub(-5 * SECS_PER_HOUR, "2010-03-14 01:59:59", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 02, 00, 00);
}


TEST(timelib_sub, test_time_spring_type3_prev_type3_prev)
{
	test_sub("America/New_York", "2010-03-13 18:38:28", "P0Y1M2DT16H19M40S");
	CHECK_RES(2010, 02, 11, 02, 18, 48);
}

TEST(timelib_sub, test_time_spring_type3_prev_type3_st)
{
	test_sub("America/New_York", "2010-03-14 00:10:20", "P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_sub, test_time_spring_type3_prev_type3_dt)
{
	test_sub("America/New_York", "2010-03-14 03:16:55", "P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_sub, test_time_spring_type3_prev_type3_post)
{
	test_sub("America/New_York", "2010-03-15 19:59:59", "P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 13, 18, 38, 28);
}

TEST(timelib_sub, test_time_spring_type3_st_type3_prev)
{
	test_sub("America/New_York", "2010-03-13 18:38:28", "-P0Y0M0DT5H31M52S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_sub, test_time_spring_type3_st_type3_st)
{
	test_sub("America/New_York", "2010-03-14 00:15:35", "P0Y0M0DT0H5M15S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_sub, test_time_spring_type3_st_type3_dt)
{
	test_sub("America/New_York", "2010-03-14 03:16:55", "P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_sub, test_time_spring_type3_st_type3_post)
{
	test_sub("America/New_York", "2010-03-15 19:59:59", "P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 14, 00, 10, 20);
}

TEST(timelib_sub, test_time_spring_type3_dt_type3_prev)
{
	test_sub("America/New_York", "2010-03-13 18:38:28", "-P0Y0M0DT7H38M27S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_sub, test_time_spring_type3_dt_type3_st)
{
	test_sub("America/New_York", "2010-03-14 00:10:20", "-P0Y0M0DT2H6M35S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_sub, test_time_spring_type3_dt_type3_dt)
{
	test_sub("America/New_York", "2010-03-14 05:19:56", "P0Y0M0DT2H3M1S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_sub, test_time_spring_type3_dt_type3_post)
{
	test_sub("America/New_York", "2010-03-15 19:59:59", "P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 14, 03, 16, 55);
}

TEST(timelib_sub, test_time_spring_type3_post_type3_prev)
{
	test_sub("America/New_York", "2010-03-13 18:38:28", "-P0Y0M2DT1H21M31S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_sub, test_time_spring_type3_post_type3_st)
{
	test_sub("America/New_York", "2010-03-14 00:10:20", "-P0Y0M1DT18H49M39S");
	CHECK_RES(2010, 03, 15, 18, 59, 59);
}

TEST(timelib_sub, test_time_spring_type3_post_type3_dt)
{
	test_sub("America/New_York", "2010-03-14 03:16:55", "-P0Y0M1DT16H43M4S");
	CHECK_RES(2010, 03, 15, 19, 59, 59);
}

TEST(timelib_sub, test_time_spring_type3_post_type3_post)
{
	test_sub("America/New_York", "2010-03-15 19:59:59", "P0Y0M0DT1H2M4S");
	CHECK_RES(2010, 03, 15, 18, 57, 55);
}

TEST(timelib_sub, test_time_spring_type3_stsec_type3_dtsec)
{
	test_sub("America/New_York", "2010-03-14 03:00:00", "P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 01, 59, 59);
}

TEST(timelib_sub, test_time_spring_type3_dtsec_type3_stsec)
{
	test_sub("America/New_York", "2010-03-14 01:59:59", "-P0Y0M0DT0H0M1S");
	CHECK_RES(2010, 03, 14, 03, 00, 00);
}

