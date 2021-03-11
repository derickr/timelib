#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <string.h>

TEST_GROUP(transitions)
{
};

#define TEST_EXISTS(n,tz) \
	TEST(transitions, n) { \
		return; \
		int error; \
		timelib_tzinfo *tzi = timelib_parse_tzfile(tz, timelib_builtin_db(), &error); \
		CHECK(tzi != NULL); \
		CHECK(error == TIMELIB_ERROR_NO_ERROR || error == TIMELIB_ERROR_SLIM_FILE); \
		timelib_dump_tzinfo(tzi); \
		timelib_tzinfo_dtor(tzi); \
	}

#define TEST_TRANSITION(n,tz,ts,eab,eoff) \
	TEST(transitions, n) { \
		int error; \
		timelib_time_offset *tto; \
		timelib_tzinfo *tzi = timelib_parse_tzfile(tz, timelib_builtin_db(), &error); \
		CHECK(tzi != NULL); \
		tto = timelib_get_time_zone_info((ts), tzi); \
		CHECK(tto != NULL); \
		LONGS_EQUAL((eoff), tto->offset); \
		STRCMP_EQUAL((eab), tto->abbr); \
		timelib_time_offset_dtor(tto); \
		timelib_tzinfo_dtor(tzi); \
	}

TEST_EXISTS(utc_00, "UTC")
TEST_TRANSITION(utc_01, "UTC", INT64_MIN / 2, "UTC", 0)
TEST_TRANSITION(utc_02, "UTC", 0,         "UTC", 0)
TEST_TRANSITION(utc_03, "UTC", INT64_MAX / 2, "UTC", 0)

TEST_EXISTS(tokyo_00, "Asia/Tokyo")
TEST_TRANSITION(tokyo_01, "Asia/Tokyo",   INT64_MIN, "LMT", 33539)
TEST_TRANSITION(tokyo_02, "Asia/Tokyo", -2587712401, "LMT", 33539)
TEST_TRANSITION(tokyo_03, "Asia/Tokyo", -2587712400, "JST", 32400)
TEST_TRANSITION(tokyo_04, "Asia/Tokyo", -2587712399, "JST", 32400)
TEST_TRANSITION(tokyo_05, "Asia/Tokyo",  -577962001, "JDT", 36000)
TEST_TRANSITION(tokyo_06, "Asia/Tokyo",  -577962000, "JST", 32400)
TEST_TRANSITION(tokyo_07, "Asia/Tokyo",  -577961999, "JST", 32400)
TEST_TRANSITION(tokyo_08, "Asia/Tokyo",           0, "JST", 32400)
TEST_TRANSITION(tokyo_09, "Asia/Tokyo",   INT64_MAX, "JST", 32400)

TEST_EXISTS(ams_00, "Europe/Amsterdam")
TEST_TRANSITION(ams_01, "Europe/Amsterdam",   INT64_MIN, "LMT",   1172)
TEST_TRANSITION(ams_02, "Europe/Amsterdam", -4260212372, "AMT",   1172)
TEST_TRANSITION(ams_03, "Europe/Amsterdam", -1025745573, "NST",   4772)
TEST_TRANSITION(ams_04, "Europe/Amsterdam", -1025745572, "+0120", 4800)
TEST_TRANSITION(ams_05, "Europe/Amsterdam",   811904399, "CEST",  7200)
TEST_TRANSITION(ams_06, "Europe/Amsterdam",   811904440, "CET",   3600)
TEST_TRANSITION(ams_07, "Europe/Amsterdam",   828234000, "CEST",  7200)

TEST_TRANSITION(ams_08, "Europe/Amsterdam",   846377999, "CEST",  7200)
TEST_TRANSITION(ams_09, "Europe/Amsterdam",   846378000, "CET",   3600)
TEST_TRANSITION(ams_10, "Europe/Amsterdam",   846378001, "CET",   3600)

TEST_TRANSITION(ams_11, "Europe/Amsterdam",   859683599, "CET",   3600)
TEST_TRANSITION(ams_12, "Europe/Amsterdam",   859683600, "CEST",  7200)
TEST_TRANSITION(ams_13, "Europe/Amsterdam",   859683600, "CEST",  7200)
