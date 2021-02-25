#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <string.h>

TEST_GROUP(warn_on_slim)
{
};


TEST(warn_on_slim, warn_on_slim_001)
{
	int             error_code;
	timelib_tzinfo *tzi;
	timelib_tzdb   *test_directory = timelib_zoneinfo("tests/c/files");

	tzi = timelib_parse_tzfile((char*) "New_York_Slim", test_directory, &error_code);

	LONGS_EQUAL(TIMELIB_ERROR_SLIM_FILE, error_code);

	timelib_tzinfo_dtor(tzi);
	timelib_zoneinfo_dtor(test_directory);
}

TEST(warn_on_slim, dont_warn_on_fat_001)
{
	int             error_code;
	timelib_tzinfo *tzi;
	timelib_tzdb   *test_directory = timelib_zoneinfo("tests/c/files");

	tzi = timelib_parse_tzfile((char*) "New_York_Fat", test_directory, &error_code);

	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	timelib_tzinfo_dtor(tzi);
	timelib_zoneinfo_dtor(test_directory);
}
