#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <string.h>

TEST_GROUP(timezone_same)
{
	int same;
	timelib_time *t1, *t2;
	timelib_tzinfo *tzi1, *tzi2;
	int dummy_error;

	TEST_SETUP()
	{
		same = 0;
		t1 = t2 = NULL;
		tzi1 = NULL;
		tzi2 = NULL;
	}

	void test_parse(const char *str1, const char *str2)
	{
		t1 = timelib_strtotime(str1, strlen(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t2 = timelib_strtotime(str2, strlen(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		same = timelib_same_timezone(t1, t2);
	}

	void test_parse_with_tz(const char *str1, const char *str2, const char *tz1, const char *tz2)
	{
		t1 = timelib_strtotime(str1, strlen(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t2 = timelib_strtotime(str2, strlen(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		if (tz1) {
			tzi1 = timelib_parse_tzfile((char*) tz1, timelib_builtin_db(), &dummy_error);
			timelib_update_ts(t1, tzi1);
		}
		if (tz2) {
			tzi2 = timelib_parse_tzfile((char*) tz2, timelib_builtin_db(), &dummy_error);
			timelib_update_ts(t2, tzi2);
		}

		same = timelib_same_timezone(t1, t2);
	}

	TEST_TEARDOWN()
	{
		if (t1) {
			timelib_time_dtor(t1);
		}
		if (t2) {
			timelib_time_dtor(t2);
		}
		if (tzi1) {
			timelib_tzinfo_dtor(tzi1);
		}
		if (tzi2) {
			timelib_tzinfo_dtor(tzi2);
		}
	}
};

#define CHECK_SAME(s,ty1,ty2) \
	LONGS_EQUAL(s, same); \
	LONGS_EQUAL(ty1, t1->zone_type); \
	LONGS_EQUAL(ty2, t2->zone_type);

TEST(timezone_same, type1_type1_same1)
{
	test_parse("2021-11-05 11:23:39 GMT+0100", "2021-11-05 11:24:07 GMT+0100");
	CHECK_SAME(1, TIMELIB_ZONETYPE_OFFSET, TIMELIB_ZONETYPE_OFFSET);
}

TEST(timezone_same, type1_type1_notsame1)
{
	test_parse("2021-11-05 11:23:39 GMT+0200", "2021-11-05 11:24:07 GMT+0100");
	CHECK_SAME(0, TIMELIB_ZONETYPE_OFFSET, TIMELIB_ZONETYPE_OFFSET);
}

TEST(timezone_same, type1_type1_notsame2)
{
	test_parse("2021-11-05 11:23:39 GMT+0100", "2021-11-05 11:24:07 GMT+0200");
	CHECK_SAME(0, TIMELIB_ZONETYPE_OFFSET, TIMELIB_ZONETYPE_OFFSET);
}


TEST(timezone_same, type2_type2_same1)
{
	test_parse("2021-11-05 11:23:39 CET", "2021-11-05 11:24:07 CET");
	CHECK_SAME(1, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_ABBR);
}

TEST(timezone_same, type2_type2_same2)
{
	test_parse("2021-11-05 11:23:39 BST", "2021-11-05 11:24:07 CET");
	CHECK_SAME(1, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_ABBR);
}

TEST(timezone_same, type2_type2_same3)
{
	test_parse("2021-11-05 11:23:39 CDT", "2021-11-05 11:24:07 EST");
	CHECK_SAME(1, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_ABBR);
}

TEST(timezone_same, type2_type2_same4)
{
	test_parse("2021-11-05 11:23:39 EST", "2021-11-05 11:24:07 CDT");
	CHECK_SAME(1, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_ABBR);
}

TEST(timezone_same, type2_type2_notsame1)
{
	test_parse("2021-11-05 11:23:39 EDT", "2021-11-05 11:24:07 CDT");
	CHECK_SAME(0, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_ABBR);
}

TEST(timezone_same, type2_type2_notsame2)
{
	test_parse("2021-11-05 11:23:39 CET", "2021-11-05 11:24:07 CEST");
	CHECK_SAME(0, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_ABBR);
}


TEST(timezone_same, type3_type3_same1)
{
	test_parse_with_tz("2021-11-05 11:23:39", "2021-11-05 11:24:07", "Europe/Amsterdam", "Europe/Amsterdam");
	CHECK_SAME(1, TIMELIB_ZONETYPE_ID, TIMELIB_ZONETYPE_ID);
}

TEST(timezone_same, type3_type3_notsame1)
{
	test_parse_with_tz("2021-11-05 11:23:39", "2021-11-05 11:24:07", "Europe/Amsterdam", "America/Chicago");
	CHECK_SAME(0, TIMELIB_ZONETYPE_ID, TIMELIB_ZONETYPE_ID);
}


TEST(timezone_same, type1_type2)
{
	test_parse_with_tz("2021-11-05 11:23:39 GMT+0100", "2021-11-05 11:24:07 BST", NULL, NULL);
	CHECK_SAME(0, TIMELIB_ZONETYPE_OFFSET, TIMELIB_ZONETYPE_ABBR);
}

TEST(timezone_same, type1_type3)
{
	test_parse_with_tz("2021-11-05 11:23:39 GMT+0100", "2021-11-05 11:24:07", NULL, "Europe/Berlin");
	CHECK_SAME(0, TIMELIB_ZONETYPE_OFFSET, TIMELIB_ZONETYPE_ID);
}

TEST(timezone_same, type2_type1)
{
	test_parse_with_tz("2021-11-05 11:23:39 CEST", "2021-11-05 11:24:07 GMT+0200", NULL, NULL);
	CHECK_SAME(0, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_OFFSET);
}

TEST(timezone_same, type2_type3)
{
	test_parse_with_tz("2021-11-05 11:23:39 CET", "2021-11-05 11:24:07", NULL, "Europe/Berlin");
	CHECK_SAME(0, TIMELIB_ZONETYPE_ABBR, TIMELIB_ZONETYPE_ID);
}

TEST(timezone_same, type3_type1)
{
	test_parse_with_tz("2021-11-05 11:23:39", "2021-11-05 11:24:07 GMT+0200", "Europe/Berlin", NULL);
	CHECK_SAME(0, TIMELIB_ZONETYPE_ID, TIMELIB_ZONETYPE_OFFSET);
}

TEST(timezone_same, type3_type2)
{
	test_parse_with_tz("2021-11-05 11:23:39", "2021-11-05 11:24:07 CET", "Europe/Berlin", NULL);
	CHECK_SAME(0, TIMELIB_ZONETYPE_ID, TIMELIB_ZONETYPE_ABBR);
}
