#include "CppUTest/TestHarness.h"
#include "timelib.h"

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
