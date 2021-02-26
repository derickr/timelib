#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <string.h>

TEST_GROUP(posix)
{
};

#define PARSE_TEST_STD(n,p,e,o) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		CHECK(s != NULL); \
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

PARSE_TEST_STD(numeric_abbr_min_01, "<-3:30>03:30",  "-3:30",    -12600)
PARSE_TEST_STD(numeric_abbr_min_02, "<-3:30>3:30",   "-3:30",    -12600)
PARSE_TEST_STD(numeric_abbr_min_03, "<+3:30>3:030",  "+3:30",    -12600)
PARSE_TEST_STD(numeric_abbr_min_04, "<+3:30>3:3",    "+3:30",    -10980)
PARSE_TEST_STD(numeric_abbr_min_05, "<3:30>+03:30",  "3:30",     -12600)
PARSE_TEST_STD(numeric_abbr_min_06, "<3:30>+3:30",   "3:30",     -12600)
PARSE_TEST_STD(numeric_abbr_min_07, "<3:3>+3:030",   "3:3",      -12600)
PARSE_TEST_STD(numeric_abbr_min_08, "<3:3>+3:3",     "3:3",      -10980)
PARSE_TEST_STD(numeric_abbr_min_09, "<3:30>-03:30",  "3:30",     +12600)
PARSE_TEST_STD(numeric_abbr_min_10, "<3:30>-3:30",   "3:30",     +12600)
PARSE_TEST_STD(numeric_abbr_min_11, "<3:3>-3:030",   "3:3",      +12600)
PARSE_TEST_STD(numeric_abbr_min_12, "<3:3>-3:3",     "3:3",      +10980)

PARSE_TEST_STD(numeric_abbr_sec_01, "<4:56:02>-04:56:02",  "4:56:02",  +17762)
PARSE_TEST_STD(numeric_abbr_sec_02, "<4:56:02>-4:56:02",   "4:56:02",  +17762)
PARSE_TEST_STD(numeric_abbr_sec_03, "<+4:56:02>-04:56:2",  "+4:56:02", +17762)
PARSE_TEST_STD(numeric_abbr_sec_04, "<+4:56:02>-4:56:2",   "+4:56:02", +17762)
PARSE_TEST_STD(numeric_abbr_sec_05, "<4:56:02>+00:19:32",  "4:56:02",   -1172)
PARSE_TEST_STD(numeric_abbr_sec_06, "<4:56:02>+0:19:32",   "4:56:02",   -1172)
PARSE_TEST_STD(numeric_abbr_sec_07, "<-4:56:02>00:19:32",  "-4:56:02",  -1172)
PARSE_TEST_STD(numeric_abbr_sec_08, "<-4:56:02>0:19:32",   "-4:56:02",  -1172)

PARSE_TEST_STD(str_abbr_hour_1, "UT0",      "UT",         0)
PARSE_TEST_STD(str_abbr_hour_2, "EST5",     "EST",   -18000)
PARSE_TEST_STD(str_abbr_hour_3, "ADSET-11", "ADSET", +39600)

PARSE_TEST_STD(str_abbr_min_01, "NST03:30",  "NST",    -12600)
PARSE_TEST_STD(str_abbr_min_02, "NST3:30",   "NST",    -12600)
PARSE_TEST_STD(str_abbr_min_03, "NST3:030",  "NST",    -12600)
PARSE_TEST_STD(str_abbr_min_04, "NST3:3",    "NST",    -10980)
PARSE_TEST_STD(str_abbr_min_05, "NST+03:30", "NST",    -12600)
PARSE_TEST_STD(str_abbr_min_06, "NST+3:30",  "NST",    -12600)
PARSE_TEST_STD(str_abbr_min_07, "NST+3:030", "NST",    -12600)
PARSE_TEST_STD(str_abbr_min_08, "NST+3:3",   "NST",    -10980)
PARSE_TEST_STD(str_abbr_min_09, "NST-03:30", "NST",    +12600)
PARSE_TEST_STD(str_abbr_min_10, "NST-3:30",  "NST",    +12600)
PARSE_TEST_STD(str_abbr_min_11, "NST-3:030", "NST",    +12600)
PARSE_TEST_STD(str_abbr_min_12, "NST-3:3",   "NST",    +10980)

PARSE_TEST_STD(str_abbr_sec_01, "LMT-04:56:02", "LMT", +17762)
PARSE_TEST_STD(str_abbr_sec_02, "LMT-4:56:02",  "LMT", +17762)
PARSE_TEST_STD(str_abbr_sec_03, "LMT-04:56:2",  "LMT", +17762)
PARSE_TEST_STD(str_abbr_sec_04, "LMT-4:56:2",   "LMT", +17762)
PARSE_TEST_STD(str_abbr_sec_05, "LMT+00:19:32", "LMT",  -1172)
PARSE_TEST_STD(str_abbr_sec_06, "LMT+0:19:32",  "LMT",  -1172)
PARSE_TEST_STD(str_abbr_sec_07, "LMT00:19:32",  "LMT",  -1172)
PARSE_TEST_STD(str_abbr_sec_08, "LMT0:19:32",   "LMT",  -1172)

PARSE_TEST_STD_NULL(numeric_abbr_err_01, "<")
PARSE_TEST_STD_NULL(numeric_abbr_err_02, "<-")
PARSE_TEST_STD_NULL(numeric_abbr_err_03, "<-0")
PARSE_TEST_STD_NULL(numeric_abbr_err_04, "<-03")
PARSE_TEST_STD_NULL(numeric_abbr_err_05, "<>")
PARSE_TEST_STD_NULL(numeric_abbr_err_06, "<-03>")
PARSE_TEST_STD_NULL(numeric_abbr_err_07, "<+03>")
PARSE_TEST_STD_NULL(numeric_abbr_err_08, "<8>")
PARSE_TEST_STD_NULL(numeric_abbr_err_09, "<+12>")
PARSE_TEST_STD_NULL(numeric_abbr_err_10, "<+03>")
PARSE_TEST_STD_NULL(numeric_abbr_err_11, "<+03:30>3:")
PARSE_TEST_STD_NULL(numeric_abbr_err_12, "<+03:30>03:")
PARSE_TEST_STD_NULL(numeric_abbr_err_13, "<+03:30>03:30:")
PARSE_TEST_STD_NULL(numeric_abbr_err_14, "<+03:30>03:45:00:")
PARSE_TEST_STD_NULL(numeric_abbr_err_15, "<+03:30>+3:")
PARSE_TEST_STD_NULL(numeric_abbr_err_16, "<+03:30>+03:")
PARSE_TEST_STD_NULL(numeric_abbr_err_17, "<+03:30>+03:30:")
PARSE_TEST_STD_NULL(numeric_abbr_err_18, "<+03:30>+03:45:00:")
PARSE_TEST_STD_NULL(numeric_abbr_err_19, "<+03:30>-3:")
PARSE_TEST_STD_NULL(numeric_abbr_err_20, "<+03:30>-03:")
PARSE_TEST_STD_NULL(numeric_abbr_err_21, "<+03:30>-03:30:")
PARSE_TEST_STD_NULL(numeric_abbr_err_22, "<+03:30>-03:45:00:")

PARSE_TEST_STD_NULL(str_abbr_err_01, "")
PARSE_TEST_STD_NULL(str_abbr_err_02, "8")
PARSE_TEST_STD_NULL(str_abbr_err_03, "UT")
PARSE_TEST_STD_NULL(str_abbr_err_04, "EST")
PARSE_TEST_STD_NULL(str_abbr_err_05, "ADSET")
PARSE_TEST_STD_NULL(str_abbr_err_06, "NST3:")
PARSE_TEST_STD_NULL(str_abbr_err_07, "NST03:")
PARSE_TEST_STD_NULL(str_abbr_err_08, "NST03:30:")
PARSE_TEST_STD_NULL(str_abbr_err_09, "NST03:45:00:")
PARSE_TEST_STD_NULL(str_abbr_err_10, "NST+3:")
PARSE_TEST_STD_NULL(str_abbr_err_11, "NST+03:")
PARSE_TEST_STD_NULL(str_abbr_err_12, "NST+03:30:")
PARSE_TEST_STD_NULL(str_abbr_err_13, "NST+03:45:00:")
PARSE_TEST_STD_NULL(str_abbr_err_14, "NST-3:")
PARSE_TEST_STD_NULL(str_abbr_err_15, "NST-03:")
PARSE_TEST_STD_NULL(str_abbr_err_16, "NST-03:30:")
PARSE_TEST_STD_NULL(str_abbr_err_17, "NST-03:45:00:")
