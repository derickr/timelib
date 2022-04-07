#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include "timelib_private.h"
#include <string.h>

TEST_GROUP(issue120)
{
	int dummy_error;
	timelib_time *t;
	timelib_tzinfo *tzi;

	TEST_SETUP()
	{
		dummy_error = 0;
		t = NULL;
		tzi = NULL;
	}

	void test_set_timestamp(timelib_sll ts, const char *tzid)
	{
		t = timelib_strtotime("now", sizeof("now"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		tzi = timelib_parse_tzfile((char*) tzid, timelib_builtin_db(), &dummy_error);
		t->tz_info = tzi;
		t->zone_type = TIMELIB_ZONETYPE_ID;
		timelib_unixtime2local(t, ts);
		timelib_update_ts(t, NULL);
	}

	void test_set_timezone(const char *ts, const char *tzid)
	{
		t = timelib_parse_from_format("U", ts, sizeof(ts), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		tzi = timelib_parse_tzfile((char*) tzid, timelib_builtin_db(), &dummy_error);
		timelib_set_timezone(t, tzi);
		timelib_unixtime2local(t, t->sse);
		timelib_update_ts(t, NULL);
	}

	TEST_TEARDOWN()
	{
		timelib_time_dtor(t);
		timelib_tzinfo_dtor(tzi);
	}
};

#define ISSUE120_TESTS(n,ts,tzid,exp_offset) \
	TEST(issue120, n ## _set_timestamp) \
	{ \
		test_set_timestamp(ts, tzid); \
		LONGS_EQUAL(exp_offset, t->z); \
		LONGS_EQUAL(ts, t->sse); \
	} \
	\
	TEST(issue120, n ## _set_timezone) \
	{ \
		test_set_timezone(#ts, tzid); \
		LONGS_EQUAL(exp_offset, t->z); \
		LONGS_EQUAL(ts, t->sse); \
	} \

// around transition to DST
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_01, 1615687199, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_02, 1615687200, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_03, 1615687201, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_04, 1615690799, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_05, 1615690800, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_06, 1615690801, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_07, 1615708799, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_08, 1615708800, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_09, 1615708801, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_10, 1615712399, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_11, 1615712400, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_12, 1615712401, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_before_trans_to_dst_13, 1615715999, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_after_trans_to_dst_1, 1615716000, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_after_trans_to_dst_2, 1615716001, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_after_trans_to_dst_3, 1615719599, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_after_trans_to_dst_4, 1615719600, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_after_trans_to_dst_5, 1615719601, "America/Los_Angeles", -25200)

ISSUE120_TESTS(php_bug_68549_azo_before_trans_to_dst_1, 1616889599, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_before_trans_to_dst_2, 1616889600, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_before_trans_to_dst_3, 1616889601, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_before_trans_to_dst_4, 1616893199, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_to_dst_1, 1616893200, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_to_dst_2, 1616893201, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_to_dst_3, 1616896799, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_to_dst_4, 1616896800, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_to_dst_5, 1616896801, "Atlantic/Azores", 0)

ISSUE120_TESTS(php_bug_68549_ldn_before_trans_to_dst_1, 1616889599, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_before_trans_to_dst_2, 1616889600, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_before_trans_to_dst_3, 1616889601, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_before_trans_to_dst_4, 1616893199, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_to_dst_1, 1616893200, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_to_dst_2, 1616893201, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_to_dst_3, 1616896799, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_to_dst_4, 1616896800, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_to_dst_5, 1616896801, "Europe/London", 3600)

ISSUE120_TESTS(php_bug_68549_ams_before_trans_to_dst_1, 1616885999, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_to_dst_2, 1616886000, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_to_dst_3, 1616886001, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_to_dst_4, 1616889599, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_to_dst_5, 1616889600, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_to_dst_6, 1616889601, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_to_dst_7, 1616893199, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_1, 1616893200, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_2, 1616893201, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_3, 1616896799, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_4, 1616896800, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_5, 1616896801, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_6, 1616900399, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_7, 1616900400, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_to_dst_8, 1616900401, "Europe/Amsterdam", 7200)

// around transition back from DST
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_01, 1636246799, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_02, 1636246800, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_03, 1636246801, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_04, 1636250399, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_05, 1636250400, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_06, 1636250401, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_07, 1636268399, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_08, 1636268400, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_09, 1636268401, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_10, 1636271999, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_11, 1636272000, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_12, 1636272001, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_before_trans_from_dst_13, 1636275599, "America/Los_Angeles", -25200)
ISSUE120_TESTS(php_bug_68549_la_after_trans_from_dst_01, 1636275600, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_after_trans_from_dst_02, 1636275601, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_after_trans_from_dst_03, 1636279199, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_after_trans_from_dst_04, 1636279200, "America/Los_Angeles", -28800)
ISSUE120_TESTS(php_bug_68549_la_after_trans_from_dst_05, 1636279201, "America/Los_Angeles", -28800)

ISSUE120_TESTS(php_bug_68549_azo_before_trans_from_dst_01, 1635638399, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_before_trans_from_dst_02, 1635638400, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_before_trans_from_dst_03, 1635638401, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_before_trans_from_dst_04, 1635641999, "Atlantic/Azores", 0)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_from_dst_01, 1635642000, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_from_dst_02, 1635642001, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_from_dst_03, 1635645599, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_from_dst_04, 1635645600, "Atlantic/Azores", -3600)
ISSUE120_TESTS(php_bug_68549_azo_after_trans_from_dst_05, 1635645601, "Atlantic/Azores", -3600)

ISSUE120_TESTS(php_bug_68549_ldn_before_trans_from_dst_01, 1635638399, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_before_trans_from_dst_02, 1635638400, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_before_trans_from_dst_03, 1635638401, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_before_trans_from_dst_04, 1635641999, "Europe/London", 3600)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_from_dst_01, 1635642000, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_from_dst_02, 1635642001, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_from_dst_03, 1635645599, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_from_dst_04, 1635645600, "Europe/London", 0)
ISSUE120_TESTS(php_bug_68549_ldn_after_trans_from_dst_05, 1635645601, "Europe/London", 0)

ISSUE120_TESTS(php_bug_68549_ams_before_trans_from_dst_01, 1635631199, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_from_dst_02, 1635631200, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_from_dst_03, 1635631201, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_from_dst_04, 1635638399, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_from_dst_05, 1635638400, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_from_dst_06, 1635638401, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_before_trans_from_dst_07, 1635641999, "Europe/Amsterdam", 7200)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_01, 1635642000, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_02, 1635642001, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_03, 1635645599, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_04, 1635645600, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_05, 1635645601, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_06, 1635649199, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_07, 1635649200, "Europe/Amsterdam", 3600)
ISSUE120_TESTS(php_bug_68549_ams_after_trans_from_dst_08, 1635649201, "Europe/Amsterdam", 3600)
