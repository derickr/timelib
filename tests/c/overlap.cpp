#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(overlap)
{
	int dummy_error;
	timelib_time *time;
	timelib_tzinfo *tzi;

	TEST_SETUP()
	{
		dummy_error = 0;
		time = timelib_time_ctor();
		tzi = NULL;
	}

	void test_set_time(timelib_sll timestamp, const char *timezone)
	{
		tzi = timelib_parse_tzfile(timezone, timelib_builtin_db(), &dummy_error);
		time->zone_type = TIMELIB_ZONETYPE_ID;
		time->tz_info = tzi;
		timelib_unixtime2local(time, timestamp);
		time->s = 59;
		time->sse_uptodate = 1;
		timelib_update_ts_first(time, NULL);
	}

	TEST_TEARDOWN()
	{
		timelib_time_dtor(time);
		if (tzi) {
			timelib_tzinfo_dtor(tzi);
		}
	}
};

#define OVERLAP_TEST(name, timestamp, timezone, offset) \
	TEST(overlap, name) \
	{ \
		test_set_time(timestamp, timezone); \
		LONGS_EQUAL(timestamp + 29, time->sse); \
		LONGS_EQUAL(offset, time->z); \
	}

OVERLAP_TEST(dublin_first, 1792887030, "Europe/Dublin", 3600)
OVERLAP_TEST(dublin_second, 1792890630, "Europe/Dublin", 0)
OVERLAP_TEST(london_first, 1792887030, "Europe/London", 3600)
OVERLAP_TEST(london_second, 1792890630, "Europe/London", 0)
