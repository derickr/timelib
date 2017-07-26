#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(get_current_offset)
{
};

TEST(get_current_offset, UTCplus0500)
{
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone_from_offset(t, 5 * 3600);

	timelib_unixtime2local(t, 1483280063);
	LONGS_EQUAL(18000, timelib_get_current_offset(t));

	timelib_time_dtor(t);
}

TEST(get_current_offset, EST)
{
	timelib_abbr_info ai = { -5 * 3600, (char*) "EST", 0 };
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone_from_abbr(t, ai);

	timelib_unixtime2local(t, 1483280063);
	LONGS_EQUAL(-18000, timelib_get_current_offset(t));

	timelib_time_dtor(t);
}

TEST(get_current_offset, EDT)
{
	timelib_abbr_info ai = { -5 * 3600, (char*) "EDT", 1 };
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone_from_abbr(t, ai);

	timelib_unixtime2local(t, 1483280063);
	LONGS_EQUAL(-14400, timelib_get_current_offset(t));

	timelib_time_dtor(t);
}

TEST(get_current_offset, London)
{
	int code;
	timelib_tzinfo* tzi = timelib_parse_tzfile((char*) "Europe/London", timelib_builtin_db(), &code);
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone(t, tzi);

	timelib_unixtime2local(t, 1483280063);
	LONGS_EQUAL(0, timelib_get_current_offset(t));
	
	timelib_unixtime2local(t, 1501074654);
	LONGS_EQUAL(3600, timelib_get_current_offset(t));

	timelib_tzinfo_dtor(tzi);
	timelib_time_dtor(t);
}

TEST(get_current_offset, Amsterdam)
{
	int code;
	timelib_tzinfo* tzi = timelib_parse_tzfile((char*) "Europe/Amsterdam", timelib_builtin_db(), &code);
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone(t, tzi);

	timelib_unixtime2local(t, 1483280063);
	LONGS_EQUAL(3600, timelib_get_current_offset(t));
	
	timelib_unixtime2local(t, 1501074654);
	LONGS_EQUAL(7200, timelib_get_current_offset(t));

	timelib_tzinfo_dtor(tzi);
	timelib_time_dtor(t);
}
