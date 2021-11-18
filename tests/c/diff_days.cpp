#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <stdio.h>
#include <string.h>


TEST_GROUP(diff_days)
{
	int days;
	timelib_time *t1, *t2;
	timelib_tzinfo *tzi1, *tzi2;
	int dummy_error;

	TEST_SETUP()
	{
		days = 0;
		t1 = t2 = NULL;
		tzi1 = NULL;
		tzi2 = NULL;
	}

	void test_parse(const char *str1, const char *str2)
	{
		t1 = timelib_strtotime(str1, strlen(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t2 = timelib_strtotime(str2, strlen(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		days = timelib_diff_days(t1, t2);
	}

	void test_parse_with_tz(const char *str1, const char *str2, const char *tz1, const char *tz2)
	{
		t1 = timelib_strtotime(str1, strlen(str1), NULL, timelib_builtin_db(), timelib_parse_tzfile);
		t2 = timelib_strtotime(str2, strlen(str2), NULL, timelib_builtin_db(), timelib_parse_tzfile);

		if (tz1) {
			tzi1 = timelib_parse_tzfile((char*) tz1, timelib_builtin_db(), &dummy_error);
			timelib_update_ts(t1, tzi1);
		} else {
			timelib_update_ts(t1, NULL);
		}
		if (tz2) {
			tzi2 = timelib_parse_tzfile((char*) tz2, timelib_builtin_db(), &dummy_error);
			timelib_update_ts(t2, tzi2);
		} else {
			timelib_update_ts(t2, NULL);
		}

		days = timelib_diff_days(t1, t2);
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

TEST(diff_days, a_year)
{
	test_parse("2021-01-01 GMT", "2021-12-31 GMT");
	LONGS_EQUAL(364, days);
}

/* PHP Bug #81458
$first = (new DateTime('2018-07-01 00:00:00.000000 America/Toronto'))
    ->setTimezone(new DateTimeZone('UTC'));
$second = new DateTime('2018-07-02 00:00:00.000000 America/Toronto');
*/
TEST(diff_days, php81458_1)
{
	test_parse_with_tz("2018-07-01 04:00 GMT+0000", "2018-07-02 00:00", NULL, "America/Toronto");
	LONGS_EQUAL(1, days);
}

TEST(diff_days, php81458_2)
{
	test_parse_with_tz("2018-12-01 00:00", "2018-12-02 00:01", "UTC", "UTC");
	LONGS_EQUAL(1, days);
}

TEST(diff_days, php78452)
{
	test_parse_with_tz("2019-09-24 11:47:24", "2019-08-21 12:47:24", "Asia/Tehran", "Asia/Tehran");
	LONGS_EQUAL(33, days);
}

TEST(diff_days, php74524)
{
	test_parse_with_tz("2017-11-17 22:05:26.000000", "2017-04-03 22:29:15.079459", "Europe/Amsterdam", "Europe/Amsterdam");
	LONGS_EQUAL(227, days);
}

TEST(diff_days, DateTime_data_fall_type2_type2)
{
	test_parse("2010-11-07 00:15:35 EDT", "2010-11-07 00:10:20 EDT");
	LONGS_EQUAL(0, days);
}

TEST(diff_days, DateTime_data_fall_type3_type3)
{
	test_parse_with_tz("2010-11-07 00:15:35", "2010-11-07 00:10:20", "America/New_York", "America/New_York");
	LONGS_EQUAL(0, days);
}
