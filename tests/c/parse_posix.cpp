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

/* Test whether all the used ones can be parsed */
#define PARSE_TEST_SUCCESS(n,p) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		CHECK(NULL != s); \
		timelib_posix_str_dtor(s); \
	}

PARSE_TEST_SUCCESS(str_001, "<-00>0")
PARSE_TEST_SUCCESS(str_002, "<+00>0<+02>-2,M3.5.0/1,M10.5.0/3")
PARSE_TEST_SUCCESS(str_003, "<+01>-1")
PARSE_TEST_SUCCESS(str_004, "<-01>1")
PARSE_TEST_SUCCESS(str_005, "<-01>1<+00>,M3.5.0/0,M10.5.0/1")
PARSE_TEST_SUCCESS(str_006, "<+02>-2")
PARSE_TEST_SUCCESS(str_007, "<-02>2")
PARSE_TEST_SUCCESS(str_008, "<+03>-3")
PARSE_TEST_SUCCESS(str_009, "<-03>3")
PARSE_TEST_SUCCESS(str_010, "<-03>3<-02>,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_011, "<-03>3<-02>,M3.5.0/-2,M10.5.0/-1")
PARSE_TEST_SUCCESS(str_012, "<+0330>-3:30<+0430>,J79/24,J263/24")
PARSE_TEST_SUCCESS(str_013, "<+0430>-4:30")
PARSE_TEST_SUCCESS(str_014, "<+04>-4")
PARSE_TEST_SUCCESS(str_015, "<-04>4")
PARSE_TEST_SUCCESS(str_016, "<-04>4<-03>,M10.1.0/0,M3.4.0/0")
PARSE_TEST_SUCCESS(str_017, "<-04>4<-03>,M9.1.6/24,M4.1.6/24")
PARSE_TEST_SUCCESS(str_018, "<+0530>-5:30")
PARSE_TEST_SUCCESS(str_019, "<+0545>-5:45")
PARSE_TEST_SUCCESS(str_020, "<+05>-5")
PARSE_TEST_SUCCESS(str_021, "<-05>5")
PARSE_TEST_SUCCESS(str_022, "<+0630>-6:30")
PARSE_TEST_SUCCESS(str_023, "<+06>-6")
PARSE_TEST_SUCCESS(str_024, "<-06>6")
PARSE_TEST_SUCCESS(str_025, "<-06>6<-05>,M9.1.6/22,M4.1.6/22")
PARSE_TEST_SUCCESS(str_026, "<+07>-7")
PARSE_TEST_SUCCESS(str_027, "<-07>7")
PARSE_TEST_SUCCESS(str_028, "<+0845>-8:45")
PARSE_TEST_SUCCESS(str_029, "<+08>-8")
PARSE_TEST_SUCCESS(str_030, "<-08>8")
PARSE_TEST_SUCCESS(str_031, "<-0930>9:30")
PARSE_TEST_SUCCESS(str_032, "<+09>-9")
PARSE_TEST_SUCCESS(str_033, "<-09>9")
PARSE_TEST_SUCCESS(str_034, "<+10>-10")
PARSE_TEST_SUCCESS(str_035, "<-10>10")
PARSE_TEST_SUCCESS(str_036, "<+1030>-10:30<+11>-11,M10.1.0,M4.1.0")
PARSE_TEST_SUCCESS(str_037, "<+11>-11")
PARSE_TEST_SUCCESS(str_038, "<-11>11")
PARSE_TEST_SUCCESS(str_039, "<+11>-11<+12>,M10.1.0,M4.1.0/3")
PARSE_TEST_SUCCESS(str_040, "<+12>-12")
PARSE_TEST_SUCCESS(str_041, "<-12>12")
PARSE_TEST_SUCCESS(str_042, "<+12>-12<+13>,M11.2.0,M1.2.3/99")
PARSE_TEST_SUCCESS(str_043, "<+1245>-12:45<+1345>,M9.5.0/2:45,M4.1.0/3:45")
PARSE_TEST_SUCCESS(str_044, "<+13>-13")
PARSE_TEST_SUCCESS(str_045, "<+13>-13<+14>,M9.5.0/3,M4.1.0/4")
PARSE_TEST_SUCCESS(str_046, "<+14>-14")
PARSE_TEST_SUCCESS(str_047, "ACST-9:30")
PARSE_TEST_SUCCESS(str_048, "ACST-9:30ACDT,M10.1.0,M4.1.0/3")
PARSE_TEST_SUCCESS(str_049, "AEST-10")
PARSE_TEST_SUCCESS(str_050, "AEST-10AEDT,M10.1.0,M4.1.0/3")
PARSE_TEST_SUCCESS(str_051, "AKST9AKDT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_052, "AST4")
PARSE_TEST_SUCCESS(str_053, "AST4ADT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_054, "AWST-8")
PARSE_TEST_SUCCESS(str_055, "CAT-2")
PARSE_TEST_SUCCESS(str_056, "CET-1")
PARSE_TEST_SUCCESS(str_057, "CET-1CEST,M3.5.0,M10.5.0/3")
PARSE_TEST_SUCCESS(str_059, "CST5CDT,M3.2.0/0,M11.1.0/1")
PARSE_TEST_SUCCESS(str_060, "CST6")
PARSE_TEST_SUCCESS(str_061, "CST6CDT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_062, "CST6CDT,M4.1.0,M10.5.0")
PARSE_TEST_SUCCESS(str_063, "CST-8")
PARSE_TEST_SUCCESS(str_064, "EAT-3")
PARSE_TEST_SUCCESS(str_065, "EET-2")
PARSE_TEST_SUCCESS(str_066, "EET-2EEST,M3.4.4/48,M10.4.4/49")
PARSE_TEST_SUCCESS(str_067, "EET-2EEST,M3.5.0/0,M10.5.0/0")
PARSE_TEST_SUCCESS(str_068, "EET-2EEST,M3.5.0/3,M10.5.0/4")
PARSE_TEST_SUCCESS(str_069, "EET-2EEST,M3.5.0,M10.5.0/3")
PARSE_TEST_SUCCESS(str_070, "EET-2EEST,M3.5.4/24,M10.5.5/1")
PARSE_TEST_SUCCESS(str_071, "EET-2EEST,M3.5.5/0,M10.5.5/0")
PARSE_TEST_SUCCESS(str_072, "EST5")
PARSE_TEST_SUCCESS(str_073, "EST5EDT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_074, "GMT0")
PARSE_TEST_SUCCESS(str_075, "GMT0BST,M3.5.0/1,M10.5.0")
PARSE_TEST_SUCCESS(str_076, "HKT-8")
PARSE_TEST_SUCCESS(str_077, "HST10")
PARSE_TEST_SUCCESS(str_078, "HST10HDT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_079, "IST-1GMT0,M10.5.0,M3.5.0/1")
PARSE_TEST_SUCCESS(str_080, "IST-2IDT,M3.4.4/26,M10.5.0")
PARSE_TEST_SUCCESS(str_081, "IST-5:30")
PARSE_TEST_SUCCESS(str_082, "JST-9")
PARSE_TEST_SUCCESS(str_083, "KST-9")
PARSE_TEST_SUCCESS(str_084, "MET-1MEST,M3.5.0,M10.5.0/3")
PARSE_TEST_SUCCESS(str_085, "MSK-3")
PARSE_TEST_SUCCESS(str_086, "MST7")
PARSE_TEST_SUCCESS(str_087, "MST7MDT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_088, "MST7MDT,M4.1.0,M10.5.0")
PARSE_TEST_SUCCESS(str_089, "NST3:30NDT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_090, "NZST-12NZDT,M9.5.0,M4.1.0/3")
PARSE_TEST_SUCCESS(str_091, "PKT-5")
PARSE_TEST_SUCCESS(str_092, "PST-8")
PARSE_TEST_SUCCESS(str_093, "PST8PDT,M3.2.0,M11.1.0")
PARSE_TEST_SUCCESS(str_094, "SAST-2")
PARSE_TEST_SUCCESS(str_095, "SST11")
PARSE_TEST_SUCCESS(str_096, "UTC0")
PARSE_TEST_SUCCESS(str_097, "WAT-1")
PARSE_TEST_SUCCESS(str_098, "WET0WEST,M3.5.0/1,M10.5.0")
PARSE_TEST_SUCCESS(str_099, "WIB-7")
PARSE_TEST_SUCCESS(str_100, "WIT-9")
PARSE_TEST_SUCCESS(str_101, "WITA-8")
