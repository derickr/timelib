#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <string.h>

TEST_GROUP(issues)
{
};

TEST(issues, issue0017_test1)
{
	timelib_sll   ts = 12622608000;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(2369, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(30, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0017_test2)
{
	timelib_sll   ts = 12622694400;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(2369, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0017_test3)
{
	timelib_sll   ts = 12622780800;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(2370, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0019_test1)
{
	timelib_sll   ts = -172800;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(1969, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(30, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0019_test2)
{
	timelib_sll   ts = -86400;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(1969, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0019_test3)
{
	timelib_sll   ts = 0;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0019_test4)
{
	timelib_sll   ts = -12622953600;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(1569, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(30, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0019_test5)
{
	timelib_sll   ts = -12622867200;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(1569, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0019_test6)
{
	timelib_sll   ts = -12622780800;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(1570, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);

	timelib_time_dtor(t);
}

TEST(issues, issue0035_test1)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            str[] = "2017-12-31 23:59:59.999999 +1 microsecond";
	timelib_time   *t     = timelib_strtotime(str, sizeof(str), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_update_ts(t, tzi);

	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);
	LONGS_EQUAL(0, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
	LONGS_EQUAL(0, t->us);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0035_test2)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            str[] = "2017-12-31 23:59:59.999999 +2 microsecond";
	timelib_time   *t     = timelib_strtotime(str, sizeof(str), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_update_ts(t, tzi);

	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);
	LONGS_EQUAL(0, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
	LONGS_EQUAL(1, t->us);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0016_test1)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            str[] = "+10000-01-01 00:00:00.000000";
	timelib_time   *t     = timelib_strtotime(str, sizeof(str), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_update_ts(t, tzi);

	LONGS_EQUAL(10000, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);
	LONGS_EQUAL(0, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
	LONGS_EQUAL(0, t->us);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0016_test2)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            str[] = "-10000-01-01 00:00:00.000000";
	timelib_time   *t     = timelib_strtotime(str, sizeof(str), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_update_ts(t, tzi);

	LONGS_EQUAL(-10000, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);
	LONGS_EQUAL(0, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
	LONGS_EQUAL(0, t->us);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0016_test3)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            str[] = "+291672107014-12-31 23:59:59";
	timelib_time   *t     = timelib_strtotime(str, sizeof(str), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_update_ts(t, tzi);

	LONGS_EQUAL(291672107014, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	LONGS_EQUAL(0, t->us);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0050_test1)
{
	char            str1[] = "2018-10-11 20:59:06.914653";
	char            str2[] = "2018-10-11 20:59:07.237419";
	timelib_time   *t1    = timelib_strtotime(str1, sizeof(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t2    = timelib_strtotime(str2, sizeof(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_rel_time *diff;

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);

	timelib_update_ts(t1, tzi);
	timelib_update_ts(t2, tzi);

	diff = timelib_diff(t1, t2);

	LONGS_EQUAL(0, diff->s);
	LONGS_EQUAL(322766, diff->us);

	timelib_time_dtor(t1);
	timelib_time_dtor(t2);
	timelib_rel_time_dtor(diff);
	timelib_tzinfo_dtor(tzi);
}


TEST(issues, issue0050_test2)
{
	char            str1[] = "2018-10-11 20:59:06.237419";
	char            str2[] = "2018-10-11 20:59:06.914653";
	timelib_time   *t1    = timelib_strtotime(str1, sizeof(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t2    = timelib_strtotime(str2, sizeof(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_rel_time *diff;

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);

	timelib_update_ts(t1, tzi);
	timelib_update_ts(t2, tzi);

	diff = timelib_diff(t1, t2);

	LONGS_EQUAL(0, diff->s);
	LONGS_EQUAL(677234, diff->us);

	timelib_time_dtor(t1);
	timelib_time_dtor(t2);
	timelib_rel_time_dtor(diff);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0051_test1)
{
	char            str1[] = "2018-11-22 13:27:52.089635";
	char            str2[] = "2018-11-22 13:27:52";
	timelib_time   *t1    = timelib_strtotime(str1, sizeof(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t2    = timelib_strtotime(str2, sizeof(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_rel_time *diff;

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);

	timelib_update_ts(t1, tzi);
	timelib_update_ts(t2, tzi);

	diff = timelib_diff(t1, t2);

	LONGS_EQUAL(0, diff->s);
	LONGS_EQUAL(89635, diff->us);
	LONGS_EQUAL(1, diff->invert);

	timelib_time_dtor(t1);
	timelib_time_dtor(t2);
	timelib_rel_time_dtor(diff);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0051_test2)
{
	char            str1[] = "2018-11-22 13:27:52";
	char            str2[] = "2018-11-22 13:27:52.089635";
	timelib_time   *t1    = timelib_strtotime(str1, sizeof(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t2    = timelib_strtotime(str2, sizeof(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_rel_time *diff;

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);

	timelib_update_ts(t1, tzi);
	timelib_update_ts(t2, tzi);

	diff = timelib_diff(t1, t2);

	LONGS_EQUAL(0, diff->s);
	LONGS_EQUAL(89635, diff->us);
	LONGS_EQUAL(0, diff->invert);

	timelib_time_dtor(t1);
	timelib_time_dtor(t2);
	timelib_rel_time_dtor(diff);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0053_test1)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_sll     ts = -61626506832;
	timelib_time   *t  = timelib_time_ctor();

	tzi = timelib_parse_tzfile((char*) "America/Belize", timelib_builtin_db(), &dummy_error);
	t->tz_info = tzi;
	t->zone_type = TIMELIB_ZONETYPE_ID;
	timelib_unixtime2local(t, ts);

	LONGS_EQUAL(17, t->y);
	LONGS_EQUAL(2, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(0, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
	LONGS_EQUAL(-21168, t->z);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0053_test2)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_sll     ts = -1822500433;
	timelib_time   *t  = timelib_time_ctor();

	tzi = timelib_parse_tzfile((char*) "America/Belize", timelib_builtin_db(), &dummy_error);
	t->tz_info = tzi;
	t->zone_type = TIMELIB_ZONETYPE_ID;
	timelib_unixtime2local(t, ts);

	LONGS_EQUAL(-21168, t->z);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0053_test3)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_sll     ts = -1822500432;
	timelib_time   *t  = timelib_time_ctor();

	tzi = timelib_parse_tzfile((char*) "America/Belize", timelib_builtin_db(), &dummy_error);
	t->tz_info = tzi;
	t->zone_type = TIMELIB_ZONETYPE_ID;
	timelib_unixtime2local(t, ts);

	LONGS_EQUAL(-21600, t->z);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
}

/* Test for no transitions */
TEST(issues, issue0065_test1)
{
	int                  dummy_error;
	timelib_tzinfo      *tzi;
	timelib_time_offset *offset;

	tzi = timelib_parse_tzfile((char*) "Etc/Gmt+5", timelib_builtin_db(), &dummy_error);
	offset = timelib_get_time_zone_info(-1822500432, tzi);

	LONGS_EQUAL(INT64_MIN, offset->transition_time);

	timelib_time_offset_dtor(offset);
	timelib_tzinfo_dtor(tzi);
}

/* Test for offset from time before first transition */
TEST(issues, issue0065_test2)
{
	int                  dummy_error;
	timelib_tzinfo      *tzi;
	timelib_time_offset *offset;

	tzi = timelib_parse_tzfile((char*) "Europe/London", timelib_builtin_db(), &dummy_error);

	offset = timelib_get_time_zone_info(-3852662326, tzi);
	LONGS_EQUAL(INT64_MIN, offset->transition_time);
	timelib_time_offset_dtor(offset);

	offset = timelib_get_time_zone_info(-3852662325, tzi);
	LONGS_EQUAL(-3852662325, offset->transition_time);
	timelib_time_offset_dtor(offset);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0069)
{
	char            str1[] = "2019-10-14T15:08:23.123+02:00";
	char            str2[] = "-50000 msec";
	timelib_time   *t1    = timelib_strtotime(str1, sizeof(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t2    = timelib_strtotime(str2, sizeof(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	int             dummy_error;
	timelib_tzinfo *tzi;

	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);

	timelib_update_ts(t1, tzi);

	memcpy(&t1->relative, &t2->relative, sizeof(timelib_rel_time));
	t1->have_relative = 1;
	t1->sse_uptodate = 0;

	timelib_update_ts(t1, NULL);
	timelib_update_from_sse(t1);
	t1->have_relative = 0;

	memset(&t1->relative, 0, sizeof(timelib_rel_time));

	LONGS_EQUAL(123000, t1->us);
	LONGS_EQUAL(33, t1->s);
	LONGS_EQUAL( 7, t1->i);
	LONGS_EQUAL(15, t1->h);

	timelib_time_dtor(t1);
	timelib_time_dtor(t2);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, issue0092)
{
	timelib_error_container *error = NULL;
	char            str[] = "@7.";
	timelib_time   *t     = timelib_strtotime(str, sizeof(str), &error, timelib_builtin_db(), timelib_parse_tzfile);

	LONGS_EQUAL(1, error->error_count);

	timelib_time_dtor(t);
	timelib_error_container_dtor(error);
}

TEST(issues, issue0093_test1)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	timelib_error_container *error = NULL;
	char            str[] = "2006-01-02T15:04:05.123456789Z";
	timelib_time   *t     = timelib_strtotime(str, sizeof(str), &error, timelib_builtin_db(), timelib_parse_tzfile);
	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_update_ts(t, tzi);

	LONGS_EQUAL(0, error->error_count);
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(2, t->d);
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(4, t->i);
	LONGS_EQUAL(5, t->s);
	LONGS_EQUAL(123456, t->us);

	timelib_time_dtor(t);
	timelib_tzinfo_dtor(tzi);
	timelib_error_container_dtor(error);
}

TEST(issues, tzcorparse_01)
{
	timelib_time t;
	int  dst = 0;
	int  tz_not_found = 0;
	const char *str = "+30157";

	timelib_parse_zone((const char **) &str, &dst, &t, &tz_not_found, timelib_builtin_db(), timelib_parse_tzfile);

	LONGS_EQUAL(1, tz_not_found);
}

TEST(issues, tzcorparse_02)
{
	timelib_time t;
	int  dst = 0;
	int  tz_not_found = 0;
	const char *str = "+30:57";
	timelib_sll ret;

	ret = timelib_parse_zone((const char **) &str, &dst, &t, &tz_not_found, timelib_builtin_db(), timelib_parse_tzfile);

	LONGS_EQUAL(0, tz_not_found);
	LONGS_EQUAL(0, t.dst);
	LONGS_EQUAL(1, t.is_localtime);
	LONGS_EQUAL(TIMELIB_ZONETYPE_OFFSET, t.zone_type);
	LONGS_EQUAL(1857 * 60, ret);
}


TEST(issues, weekday_time_part_01)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            current[] = "2020-11-27 12:33:57";
	char            relative[] = "+1 weekday";
	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_time   *t_cur = timelib_strtotime(current, sizeof(current), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t_rel = timelib_strtotime(relative, sizeof(relative), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	timelib_fill_holes(t_rel, t_cur, TIMELIB_NONE);
	timelib_update_ts(t_rel, tzi);

	LONGS_EQUAL(12, t_rel->h);
	LONGS_EQUAL(33, t_rel->i);
	LONGS_EQUAL(57, t_rel->s);

	timelib_time_dtor(t_cur);
	timelib_time_dtor(t_rel);
	timelib_tzinfo_dtor(tzi);
}


TEST(issues, first_day_of_time_01)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            current[] = "2020-11-27 12:33:57";
	char            relative[] = "first day of this month";
	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_time   *t_cur = timelib_strtotime(current, sizeof(current), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t_rel = timelib_strtotime(relative, sizeof(relative), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	timelib_fill_holes(t_rel, t_cur, TIMELIB_NONE);
	timelib_update_ts(t_rel, tzi);

	LONGS_EQUAL(2020, t_rel->y);
	LONGS_EQUAL(11,   t_rel->m);
	LONGS_EQUAL( 1,   t_rel->d);
	LONGS_EQUAL( 0, t_rel->h);
	LONGS_EQUAL( 0, t_rel->i);
	LONGS_EQUAL( 0, t_rel->s);

	timelib_time_dtor(t_cur);
	timelib_time_dtor(t_rel);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, first_day_of_time_02)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            current[] = "2020-11-27 12:33:57";
	char            relative[] = "first day of last month";
	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_time   *t_cur = timelib_strtotime(current, sizeof(current), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t_rel = timelib_strtotime(relative, sizeof(relative), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	timelib_fill_holes(t_rel, t_cur, TIMELIB_NONE);
	timelib_update_ts(t_rel, tzi);

	LONGS_EQUAL(2020, t_rel->y);
	LONGS_EQUAL(10,   t_rel->m);
	LONGS_EQUAL( 1,   t_rel->d);
	LONGS_EQUAL( 0, t_rel->h);
	LONGS_EQUAL( 0, t_rel->i);
	LONGS_EQUAL( 0, t_rel->s);

	timelib_time_dtor(t_cur);
	timelib_time_dtor(t_rel);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, last_day_of_time_01)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            current[] = "2020-11-27 12:33:57";
	char            relative[] = "last day of this month";
	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_time   *t_cur = timelib_strtotime(current, sizeof(current), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t_rel = timelib_strtotime(relative, sizeof(relative), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	timelib_fill_holes(t_rel, t_cur, TIMELIB_NONE);
	timelib_update_ts(t_rel, tzi);

	LONGS_EQUAL(2020, t_rel->y);
	LONGS_EQUAL(11,   t_rel->m);
	LONGS_EQUAL(30,   t_rel->d);
	LONGS_EQUAL( 0, t_rel->h);
	LONGS_EQUAL( 0, t_rel->i);
	LONGS_EQUAL( 0, t_rel->s);

	timelib_time_dtor(t_cur);
	timelib_time_dtor(t_rel);
	timelib_tzinfo_dtor(tzi);
}

TEST(issues, last_day_of_time_02)
{
	int             dummy_error;
	timelib_tzinfo *tzi;
	char            current[] = "2020-11-27 12:33:57";
	char            relative[] = "last day of last month";
	tzi = timelib_parse_tzfile((char*) "UTC", timelib_builtin_db(), &dummy_error);
	timelib_time   *t_cur = timelib_strtotime(current, sizeof(current), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_time   *t_rel = timelib_strtotime(relative, sizeof(relative), NULL, timelib_builtin_db(), timelib_parse_tzfile);

	timelib_fill_holes(t_rel, t_cur, TIMELIB_NONE);
	timelib_update_ts(t_rel, tzi);

	LONGS_EQUAL(2020, t_rel->y);
	LONGS_EQUAL(10,   t_rel->m);
	LONGS_EQUAL(31,   t_rel->d);
	LONGS_EQUAL( 0, t_rel->h);
	LONGS_EQUAL( 0, t_rel->i);
	LONGS_EQUAL( 0, t_rel->s);

	timelib_time_dtor(t_cur);
	timelib_time_dtor(t_rel);
	timelib_tzinfo_dtor(tzi);
}
