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

#define PARSE_TEST_NULL(n,p) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		POINTERS_EQUAL(NULL, s); \
	}

PARSE_TEST_NULL(numeric_abbr_err_01, "<")
PARSE_TEST_NULL(numeric_abbr_err_02, "<-")
PARSE_TEST_NULL(numeric_abbr_err_03, "<-0")
PARSE_TEST_NULL(numeric_abbr_err_04, "<-03")
PARSE_TEST_NULL(numeric_abbr_err_05, "<>")
PARSE_TEST_NULL(numeric_abbr_err_06, "<-03>")
PARSE_TEST_NULL(numeric_abbr_err_07, "<+03>")
PARSE_TEST_NULL(numeric_abbr_err_08, "<8>")
PARSE_TEST_NULL(numeric_abbr_err_09, "<+12>")
PARSE_TEST_NULL(numeric_abbr_err_10, "<+03>")
PARSE_TEST_NULL(numeric_abbr_err_11, "<+03:30>3:")
PARSE_TEST_NULL(numeric_abbr_err_12, "<+03:30>03:")
PARSE_TEST_NULL(numeric_abbr_err_13, "<+03:30>03:30:")
PARSE_TEST_NULL(numeric_abbr_err_14, "<+03:30>03:45:00:")
PARSE_TEST_NULL(numeric_abbr_err_15, "<+03:30>+3:")
PARSE_TEST_NULL(numeric_abbr_err_16, "<+03:30>+03:")
PARSE_TEST_NULL(numeric_abbr_err_17, "<+03:30>+03:30:")
PARSE_TEST_NULL(numeric_abbr_err_18, "<+03:30>+03:45:00:")
PARSE_TEST_NULL(numeric_abbr_err_19, "<+03:30>-3:")
PARSE_TEST_NULL(numeric_abbr_err_20, "<+03:30>-03:")
PARSE_TEST_NULL(numeric_abbr_err_21, "<+03:30>-03:30:")
PARSE_TEST_NULL(numeric_abbr_err_22, "<+03:30>-03:45:00:")

PARSE_TEST_NULL(str_abbr_err_01, "")
PARSE_TEST_NULL(str_abbr_err_02, "8")
PARSE_TEST_NULL(str_abbr_err_03, "UT")
PARSE_TEST_NULL(str_abbr_err_04, "EST")
PARSE_TEST_NULL(str_abbr_err_05, "ADSET")
PARSE_TEST_NULL(str_abbr_err_06, "NST3:")
PARSE_TEST_NULL(str_abbr_err_07, "NST03:")
PARSE_TEST_NULL(str_abbr_err_08, "NST03:30:")
PARSE_TEST_NULL(str_abbr_err_09, "NST03:45:00:")
PARSE_TEST_NULL(str_abbr_err_10, "NST+3:")
PARSE_TEST_NULL(str_abbr_err_11, "NST+03:")
PARSE_TEST_NULL(str_abbr_err_12, "NST+03:30:")
PARSE_TEST_NULL(str_abbr_err_13, "NST+03:45:00:")
PARSE_TEST_NULL(str_abbr_err_14, "NST-3:")
PARSE_TEST_NULL(str_abbr_err_15, "NST-03:")
PARSE_TEST_NULL(str_abbr_err_16, "NST-03:30:")
PARSE_TEST_NULL(str_abbr_err_17, "NST-03:45:00:")

// missing transition information
PARSE_TEST_NULL(dst_abbr_1, "EST5EDT")
PARSE_TEST_NULL(dst_abbr_2, "EST+5EDT")
PARSE_TEST_NULL(dst_abbr_3, "EET-2EEST")
PARSE_TEST_NULL(dst_abbr_4, "<+1030>-10:30<+11>-11")

#define TYPE_JD  1
#define TYPE_D   2
#define TYPE_MWD 3

#define PARSE_TEST_STD_DST(n,p,a1,a2,o1,o2,t1,t1d,t1m,t1w,t1dow,t1h,t2,t2d,t2m,t2w,t2dow,t2h) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		\
		CHECK(s != NULL); \
		\
		STRCMP_EQUAL((a1), s->std); \
		LONGS_EQUAL((o1), s->std_offset); \
		STRCMP_EQUAL((a2), s->dst); \
		LONGS_EQUAL((o2), s->dst_offset); \
		\
		CHECK(s->dst_begin != NULL); \
		LONGS_EQUAL((t1), s->dst_begin->type); \
		if ((t1) == TYPE_MWD) { \
			LONGS_EQUAL((t1m), s->dst_begin->mwd.month); \
			LONGS_EQUAL((t1w), s->dst_begin->mwd.week); \
			LONGS_EQUAL((t1dow), s->dst_begin->mwd.dow); \
		} else { \
			LONGS_EQUAL((t1d), s->dst_begin->days); \
		} \
		LONGS_EQUAL((t1h), s->dst_begin->hour); \
		\
		CHECK(s->dst_end != NULL); \
		LONGS_EQUAL((t2), s->dst_end->type); \
		if ((t2) == TYPE_MWD) { \
			LONGS_EQUAL((t2m), s->dst_end->mwd.month); \
			LONGS_EQUAL((t2w), s->dst_end->mwd.week); \
			LONGS_EQUAL((t2dow), s->dst_end->mwd.dow); \
		} else { \
			LONGS_EQUAL((t2d), s->dst_end->days); \
		} \
		LONGS_EQUAL((t2h), s->dst_end->hour); \
		\
		timelib_posix_str_dtor(s); \
	}

// America/New_York
PARSE_TEST_STD_DST(full_1,
	"EST5EDT,M3.2.0,M11.1.0",
	"EST", "EDT",  -18000, -14400,
	TYPE_MWD, -1,  3, 2, 0, 7200,
	TYPE_MWD, -1, 11, 1, 0, 7200
)

// Asia/Amman
PARSE_TEST_STD_DST(full_2,
	"EET-2EEST,M3.5.4/24,M10.5.5/1",
	"EET", "EEST",  +7200, +10800,
	TYPE_MWD, -1,  3, 5, 4, 86400,
	TYPE_MWD, -1, 10, 5, 5,  3600
)

// Australia/Lord_Howe
PARSE_TEST_STD_DST(full_3,
	"<+1030>-10:30<+11>-11,M10.1.0,M4.1.0",
	"+1030", "+11", 37800, 39600,
	TYPE_MWD, -1, 10, 1, 0, 7200,
	TYPE_MWD, -1,  4, 1, 0, 7200
)

// Asia/Tehran
PARSE_TEST_STD_DST(full_4,
	"<+0330>-3:30<+0430>,J79/24,J263/24",
	"+0330", "+0430", +12600, +16200,
	TYPE_JD,  79, -1, -1, -1, 86400,
	TYPE_JD, 263, -1, -1, -1, 86400
)

// Asia/Tehran variant without J (not actually in use)
PARSE_TEST_STD_DST(full_5,
	"<+0330>-3:30<+0430>,79/24,263/24",
	"+0330", "+0430", +12600, +16200,
	TYPE_D,  79, -1, -1, -1, 86400,
	TYPE_D, 263, -1, -1, -1, 86400
)
