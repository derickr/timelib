#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <string.h>

TEST_GROUP(posix)
{
};

#define PARSE_TEST_STD(n,p,e,o) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		STRCMP_EQUAL((e), s->std); \
		LONGS_EQUAL((o), s->std_offset); \
		timelib_posix_str_dtor(s); \
	}

#define PARSE_TEST_STD_NULL(n,p) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		POINTERS_EQUAL(NULL, s); \
	}

PARSE_TEST_STD(numeric_abbr_1, "<-07>7",   "-07", -25200)
PARSE_TEST_STD(numeric_abbr_2, "<+12>-12", "+12", +43200)

PARSE_TEST_STD(str_abbr_1, "UT0",      "UT",         0)
PARSE_TEST_STD(str_abbr_2, "EST5",     "EST",   -18000)
PARSE_TEST_STD(str_abbr_3, "ADSET-11", "ADSET", +39600)

PARSE_TEST_STD_NULL(numeric_abbr_err_1, "<")
PARSE_TEST_STD_NULL(numeric_abbr_err_2, "<-")
PARSE_TEST_STD_NULL(numeric_abbr_err_3, "<-0")
PARSE_TEST_STD_NULL(numeric_abbr_err_4, "<-03")
PARSE_TEST_STD_NULL(numeric_abbr_err_5, "<>")
PARSE_TEST_STD_NULL(numeric_abbr_err_6, "<-03>")
PARSE_TEST_STD_NULL(numeric_abbr_err_7, "<+03>")
PARSE_TEST_STD_NULL(numeric_abbr_err_8, "<8>")
PARSE_TEST_STD_NULL(numeric_abbr_err_9, "<+12>")

PARSE_TEST_STD_NULL(str_abbr_err_1, "")
PARSE_TEST_STD_NULL(str_abbr_err_2, "8")
PARSE_TEST_STD_NULL(str_abbr_err_3, "UT")
PARSE_TEST_STD_NULL(str_abbr_err_4, "EST")
PARSE_TEST_STD_NULL(str_abbr_err_5, "ADSET")
