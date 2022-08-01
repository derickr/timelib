#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(get_offset_info)
{
};

TEST(get_offset_info, UTCPlusOffset)
{
	int32_t offset;
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone_from_offset(t, 5 * 3600);

	timelib_unixtime2local(t, 1483280063);
	CHECK_FALSE(timelib_get_time_zone_offset_info(t->sse, t->tz_info, &offset, NULL, NULL));

	timelib_time_dtor(t);
}

TEST(get_offset_info, AbbreviatedTimeZone)
{
	int32_t offset;
	timelib_abbr_info ai = { -5 * 3600, (char*) "EST", 0 };
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone_from_abbr(t, ai);

	timelib_unixtime2local(t, 1483280063);
	CHECK_FALSE(timelib_get_time_zone_offset_info(t->sse, t->tz_info, &offset, NULL, NULL));

	timelib_time_dtor(t);
}

TEST(get_offset_info, London)
{
	int code;
	int32_t offset;
	timelib_sll transition;
	unsigned int is_dst;
	timelib_tzinfo* tzi = timelib_parse_tzfile((char*) "Europe/London", timelib_builtin_db(), &code);
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone(t, tzi);

	timelib_unixtime2local(t, 1483280063); // 1483280063 = 2017-01-01
	CHECK_TRUE(timelib_get_time_zone_offset_info(t->sse, t->tz_info, &offset, &transition, &is_dst));
	LONGS_EQUAL(0, offset);
	CHECK_FALSE(is_dst);
	LONGS_EQUAL(1477789200, transition); // 1477789200 = 2016-10-30

	timelib_unixtime2local(t, 1501074654); // 1501074654 = 2017-07-26
	CHECK_TRUE(timelib_get_time_zone_offset_info(t->sse, t->tz_info, &offset, &transition, &is_dst));
	LONGS_EQUAL(3600, offset);
	CHECK_TRUE(is_dst);
	LONGS_EQUAL(1490490000, transition); // 1490490000 = 2017-03-26

	timelib_tzinfo_dtor(tzi);
	timelib_time_dtor(t);
}

TEST(get_offset_info, Amsterdam)
{
	int code;
	int32_t offset;
	timelib_tzinfo* tzi = timelib_parse_tzfile((char*) "Europe/Amsterdam", timelib_builtin_db(), &code);
	timelib_time* t = timelib_time_ctor();
	timelib_set_timezone(t, tzi);

	timelib_unixtime2local(t, 1483280063);
	CHECK_TRUE(timelib_get_time_zone_offset_info(t->sse, t->tz_info, &offset, NULL, NULL));
	LONGS_EQUAL(3600, offset);

	timelib_unixtime2local(t, 1501074654);
	CHECK_TRUE(timelib_get_time_zone_offset_info(t->sse, t->tz_info, &offset, NULL, NULL));
	LONGS_EQUAL(7200, offset);

	timelib_tzinfo_dtor(tzi);
	timelib_time_dtor(t);
}
