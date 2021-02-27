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
PARSE_TEST_SUCCESS(str_058, "ChST-10")
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


/* Test whether all the used ones can be parsed */
#define PARSE_FUZZED(n,p) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		CHECK(NULL == s); \
	}

PARSE_FUZZED(fuzzed_001, "+-00>0")
PARSE_FUZZED(fuzzed_002, "<+00>0<+02>-2,M3.5.0/1sM10.5.0/3")
PARSE_FUZZED(fuzzed_003, "<+00>0<+02>-2,M3.5.0/1,t10.5.0/3")
PARSE_FUZZED(fuzzed_006, "<-01>1<+00>,M3.5.0/0,M1o.5.0/1")
PARSE_FUZZED(fuzzed_008, "<-01Q1<+00>,M3.5.0/0,M10.5.0/1")
PARSE_FUZZED(fuzzed_009, "<+02>-i")
PARSE_FUZZED(fuzzed_010, "<+02>I2")
PARSE_FUZZED(fuzzed_011, "<-03>+")
PARSE_FUZZED(fuzzed_013, "<-03>3<-02>,M3.5.0/-2,M10.5x0/-1")
PARSE_FUZZED(fuzzed_014, "<-03>3<-02>pM3.5.0/-2,M10.5.0/-1")
PARSE_FUZZED(fuzzed_015, "<+0330>-3:30<+0430>,J79/24lJ263/24")
PARSE_FUZZED(fuzzed_016, "<+0330>-3:3u<+0430>,J79/24,J263/24")
PARSE_FUZZED(fuzzed_017, "<-0343")
PARSE_FUZZED(fuzzed_019, "<-0373<-02>,M3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_021, "<+03>a3")
PARSE_FUZZED(fuzzed_022, "<-03o3<-02>,M3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_023, "<+03U-3")
PARSE_FUZZED(fuzzed_025, "<+0430>-4:3.")
PARSE_FUZZED(fuzzed_027, "<+0430>q4:30")
PARSE_FUZZED(fuzzed_028, "<-04>4<-03>,M10.1.0/0zM3.4.0/0")
PARSE_FUZZED(fuzzed_029, "<-04>4<-03>,M1n.1.0/0,M3.4.0/0")
PARSE_FUZZED(fuzzed_030, "<-04>4<-03>,M9.1.6/n4,M4.1.6/24")
PARSE_FUZZED(fuzzed_031, "<-04>4<-03>,M9.1.6/z4,M4.1.6/24")
PARSE_FUZZED(fuzzed_032, "<-04>c")
PARSE_FUZZED(fuzzed_033, "<-04>c<-03>,M10.1.0/0,M3.4.0/0")
PARSE_FUZZED(fuzzed_035, "<+0530>-5:3G")
PARSE_FUZZED(fuzzed_036, "<+0530>C5:30")
PARSE_FUZZED(fuzzed_037, "<+0545>-5d45")
PARSE_FUZZED(fuzzed_038, "<+05457-5:45")
PARSE_FUZZED(fuzzed_040, "<+05>-a")
PARSE_FUZZED(fuzzed_041, "<-05x5")
PARSE_FUZZED(fuzzed_045, "<-06>6<-05>,M9.1.6/22,M4.N.6/22")
PARSE_FUZZED(fuzzed_047, "<+06B-6")
PARSE_FUZZED(fuzzed_048, "<-06>P")
PARSE_FUZZED(fuzzed_049, "<-06>q")
PARSE_FUZZED(fuzzed_050, "<-06T6<-05>,M9.1.6/22,M4.1.6/22")
PARSE_FUZZED(fuzzed_051, "<-06w6<-05>,M9.1.6/22,M4.1.6/22")
PARSE_FUZZED(fuzzed_052, "<+07>-a")
PARSE_FUZZED(fuzzed_053, "<-07k7")
PARSE_FUZZED(fuzzed_054, "<-07>Y")
PARSE_FUZZED(fuzzed_055, "<+083-8")
PARSE_FUZZED(fuzzed_056, "<+0845>-k:45")
PARSE_FUZZED(fuzzed_057, "<+0845t-8:45")
PARSE_FUZZED(fuzzed_060, "<-0930>9i30")
PARSE_FUZZED(fuzzed_061, "--09>9")
PARSE_FUZZED(fuzzed_062, "<-09>L")
PARSE_FUZZED(fuzzed_063, "<+09n-9")
PARSE_FUZZED(fuzzed_076, "<+10>-1j")
PARSE_FUZZED(fuzzed_077, "<+1030>-10:30<+11>-11,L10.1.0,M4.1.0")
PARSE_FUZZED(fuzzed_078, "<+1030>-10:30<+11>-11,M10.V.0,M4.1.0")
PARSE_FUZZED(fuzzed_079, "<+1030>-10O30<+11>-11,M10.1.0,M4.1.0")
PARSE_FUZZED(fuzzed_080, "<-10E10")
PARSE_FUZZED(fuzzed_081, "<-10n10")
PARSE_FUZZED(fuzzed_082, "<+10>-t0")
PARSE_FUZZED(fuzzed_083, "<-10V10")
PARSE_FUZZED(fuzzed_085, "<+11>-11<+12>,M10.1.0,MI.1.0/3")
PARSE_FUZZED(fuzzed_086, "<+11>-11<+12>,M10.1.0,o4.1.0/3")
PARSE_FUZZED(fuzzed_087, "<+11>-11<+12>,M10.180,M4.1.0/3")
PARSE_FUZZED(fuzzed_088, "<-11>1c")
PARSE_FUZZED(fuzzed_089, "<-11>o1")
PARSE_FUZZED(fuzzed_090, "<-12>1/")
PARSE_FUZZED(fuzzed_092, "<+12>-12<+13>,M11.2.0,M1g2.3/99")
PARSE_FUZZED(fuzzed_093, "<-12412")
PARSE_FUZZED(fuzzed_094, "<+1245>-12:45<+1345>,M9.5.0/2:45,M+.1.0/3:45")
PARSE_FUZZED(fuzzed_095, "<+1245>-12:45<+1345>,M9.5.0/U:45,M4.1.0/3:45")
PARSE_FUZZED(fuzzed_096, "<+1245>-12:45<+1345>,M9.w.0/2:45,M4.1.0/3:45")
PARSE_FUZZED(fuzzed_097, "<+12>r12<+13>,M11.2.0,M1.2.3/99")
PARSE_FUZZED(fuzzed_098, "<-12X12")
PARSE_FUZZED(fuzzed_099, "<+13>-1+")
PARSE_FUZZED(fuzzed_100, "<+13>-13<+14-,M9.5.0/3,M4.1.0/4")
PARSE_FUZZED(fuzzed_101, "<+13>-1v")
PARSE_FUZZED(fuzzed_102, "<+14>-1i")
PARSE_FUZZED(fuzzed_103, "<+14>-1O")
PARSE_FUZZED(fuzzed_104, "<+14>-.4")
PARSE_FUZZED(fuzzed_111, "4+07>-7")
PARSE_FUZZED(fuzzed_113, "8WST-8")
PARSE_FUZZED(fuzzed_117, "A5ST-10")
PARSE_FUZZED(fuzzed_120, "ACST-9:30ACDT,,10.1.0,M4.1.0/3")
PARSE_FUZZED(fuzzed_124, "AES,-10")
PARSE_FUZZED(fuzzed_125, "AEST-10AEDT,M10.1.0,M:.1.0/3")
PARSE_FUZZED(fuzzed_126, "AEST-10AEDT,M10.1.0PM4.1.0/3")
PARSE_FUZZED(fuzzed_127, "AEST-10AEDT,M1-.1.0,M4.1.0/3")
PARSE_FUZZED(fuzzed_130, "AKST9AKDT, 3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_135, "AST4ADT,M3.2.0,L11.1.0")
PARSE_FUZZED(fuzzed_137, "AST4ADT,Me.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_142, "CE5-1CEST,M3.5.0,M10.5.0/3")
PARSE_FUZZED(fuzzed_143, "CET-1CEST,M3.5.0,M10.5.0n3")
PARSE_FUZZED(fuzzed_144, "CET-1CEST,M3.5.0,M10.T.0/3")
PARSE_FUZZED(fuzzed_145, "CET-F")
PARSE_FUZZED(fuzzed_146, "CET-l")
PARSE_FUZZED(fuzzed_149, "CST5CD ,M3.2.0/0,M11.1.0/1")
PARSE_FUZZED(fuzzed_150, "CST5CDTCM3.2.0/0,M11.1.0/1")
PARSE_FUZZED(fuzzed_152, "CST6CDT,-3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_153, "CST6CDTlM4.1.0,M10.5.0")
PARSE_FUZZED(fuzzed_154, "CST6CDT,M3.2.0,MB1.1.0")
PARSE_FUZZED(fuzzed_155, "CST6CDT,M3.2.0,Mk1.1.0")
PARSE_FUZZED(fuzzed_156, "CST6CDT,M4.1.06M10.5.0")
PARSE_FUZZED(fuzzed_157, "CST6C T,M4.1.0,M10.5.0")
PARSE_FUZZED(fuzzed_158, "CSTa")
PARSE_FUZZED(fuzzed_159, "CST-g")
PARSE_FUZZED(fuzzed_165, "EAT-:")
PARSE_FUZZED(fuzzed_169, "EE+-2")
PARSE_FUZZED(fuzzed_171, "EET-2E1ST,M3.5.5/0,M10.5.5/0")
PARSE_FUZZED(fuzzed_174, "EET-2EEST,M3.4a4/48,M10.4.4/49")
PARSE_FUZZED(fuzzed_175, "EET-2EEST,M3.5.0,M10.5F0/3")
PARSE_FUZZED(fuzzed_176, "EET-2EEST,M3.5.0,M10.y.0/3")
PARSE_FUZZED(fuzzed_177, "EET-2EEST,M3.5.0/W,M10.5.0/4")
PARSE_FUZZED(fuzzed_178, "EET-2EEST,M3.5.4/24,Mf0.5.5/1")
PARSE_FUZZED(fuzzed_180, "EET-2EEST,M3.5.5/0,M10.5.5O0")
PARSE_FUZZED(fuzzed_181, "EET-2EEST,M3.5.R/3,M10.5.0/4")
PARSE_FUZZED(fuzzed_182, "EET-2EEST,M3.i.0/0,M10.5.0/0")
PARSE_FUZZED(fuzzed_183, "EET-2EEST,M3.N.0/3,M10.5.0/4")
PARSE_FUZZED(fuzzed_184, "EET-aEEST,M3.5.0/0,M10.5.0/0")
PARSE_FUZZED(fuzzed_186, "EET-nEEST,M3.5.0,M10.5.0/3")
PARSE_FUZZED(fuzzed_191, "EST5EDT,M3.2.0,M11.1z0")
PARSE_FUZZED(fuzzed_192, "EST5EDT,M3.2.0,M11a1.0")
PARSE_FUZZED(fuzzed_197, "GMT0BST,M3.5.0/1,M10.a.0")
PARSE_FUZZED(fuzzed_198, "GMT0BST,M3.5.G/1,M10.5.0")
PARSE_FUZZED(fuzzed_199, "GMTc")
PARSE_FUZZED(fuzzed_200, "GMTO")
PARSE_FUZZED(fuzzed_201, "GMTU")
PARSE_FUZZED(fuzzed_207, "HST10HDT,M-.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_208, "HST10HDT,M3.2.0,M11.1.K")
PARSE_FUZZED(fuzzed_211, "H T-8")
PARSE_FUZZED(fuzzed_213, "i+11>-11")
PARSE_FUZZED(fuzzed_216, "IST-1GMT0,M10.5.0,M3.E.0/1")
PARSE_FUZZED(fuzzed_217, "IST-1GMT0,o10.5.0,M3.5.0/1")
PARSE_FUZZED(fuzzed_219, "IST-2IDT,M3.4.4/26,M10f5.0")
PARSE_FUZZED(fuzzed_220, "IST-2IDT,M3.4.4/26,U10.5.0")
PARSE_FUZZED(fuzzed_221, "IST-5::0")
PARSE_FUZZED(fuzzed_222, "IST-5:a0")
PARSE_FUZZED(fuzzed_225, "J3T-9")
PARSE_FUZZED(fuzzed_230, "L-00>0")
PARSE_FUZZED(fuzzed_232, "L+12>-12")
PARSE_FUZZED(fuzzed_234, "MET-1MEST,M3.5.0,d10.5.0/3")
PARSE_FUZZED(fuzzed_235, "MET-1MEST,M3.5.0,M;0.5.0/3")
PARSE_FUZZED(fuzzed_236, "MET-1MEST,r3.5.0,M10.5.0/3")
PARSE_FUZZED(fuzzed_237, "MSK.3")
PARSE_FUZZED(fuzzed_238, "MSK;3")
PARSE_FUZZED(fuzzed_239, "MST7MDT,M3.2.0,M1s.1.0")
PARSE_FUZZED(fuzzed_240, "MST7MDT,M4.1.0eM10.5.0")
PARSE_FUZZED(fuzzed_241, "MST7MDT,M4.1.0,M10.5.n")
PARSE_FUZZED(fuzzed_242, "MST7MDT,M4.1.0,M1i.5.0")
PARSE_FUZZED(fuzzed_243, "MST7MDT,v3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_249, "NS,3:30NDT,M3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_250, "NST3:30NDT,M3.2.0,M11.o.0")
PARSE_FUZZED(fuzzed_251, "NZST-12NZDT,M9.5.0,MC.1.0/3")
PARSE_FUZZED(fuzzed_252, "NZST-12NZDTmM9.5.0,M4.1.0/3")
PARSE_FUZZED(fuzzed_253, "NZST-12NZDTMM9.5.0,M4.1.0/3")
PARSE_FUZZED(fuzzed_254, "O+0630>-6:30")
PARSE_FUZZED(fuzzed_255, "P-01>1<+00>,M3.5.0/0,M10.5.0/1")
PARSE_FUZZED(fuzzed_256, "p-02>2")
PARSE_FUZZED(fuzzed_257, "PK8-5")
PARSE_FUZZED(fuzzed_260, "PS6-8")
PARSE_FUZZED(fuzzed_262, "PST8PDT9M3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_263, "PST8PDT,M3.2.0,M11.1.p")
PARSE_FUZZED(fuzzed_264, "PST8PDT,M3.2.0,M11.S.0")
PARSE_FUZZED(fuzzed_265, "P-T-8")
PARSE_FUZZED(fuzzed_272, "SAS1-2")
PARSE_FUZZED(fuzzed_275, "SST 1")
PARSE_FUZZED(fuzzed_277, ":ST10")
PARSE_FUZZED(fuzzed_278, "+ST7MDT,M3.2.0,M11.1.0")
PARSE_FUZZED(fuzzed_280, "T+04>-4")
PARSE_FUZZED(fuzzed_281, "T-05>5")
PARSE_FUZZED(fuzzed_284, "UTCp")
PARSE_FUZZED(fuzzed_285, "UTCu")
PARSE_FUZZED(fuzzed_287, "W-01>1")
PARSE_FUZZED(fuzzed_290, "W6T-1")
PARSE_FUZZED(fuzzed_291, "WAT-f")
PARSE_FUZZED(fuzzed_293, "WET0WEST,M3.5.0/1HM10.5.0")
PARSE_FUZZED(fuzzed_294, "WET0WEST,M3.5.0/1,M10.5.o")
PARSE_FUZZED(fuzzed_295, "WET0WESTnM3.5.0/1,M10.5.0")
PARSE_FUZZED(fuzzed_296, "WIB-,")
PARSE_FUZZED(fuzzed_297, "WIB-t")
PARSE_FUZZED(fuzzed_300, "WIT2-8")
PARSE_FUZZED(fuzzed_302, "y-00>0")
PARSE_FUZZED(fuzzed_304, "<-5>>5")


/* Test whether all the used ones can be parsed */
#define PARSE_FUZZED_OK(n,p) \
	TEST(posix, n) { \
		timelib_posix_str *s = timelib_parse_posix_str(p); \
		CHECK(NULL != s); \
		timelib_posix_str_dtor(s); \
	}

PARSE_FUZZED_OK(fuzzed_004, "<+00>0<+0X>-2,M3.5.0/1,M10.5.0/3")
PARSE_FUZZED_OK(fuzzed_005, "</01>-1")
PARSE_FUZZED_OK(fuzzed_007, "<013>-13")
PARSE_FUZZED_OK(fuzzed_012, "<;03>3")
PARSE_FUZZED_OK(fuzzed_018, "<+03>63")
PARSE_FUZZED_OK(fuzzed_020, "<-03>9<-02>,M3.5.0/-2,M10.5.0/-1")
PARSE_FUZZED_OK(fuzzed_024, "<-04>1<-03>,M9.1.6/24,M4.1.6/24")
PARSE_FUZZED_OK(fuzzed_026, "<+0430>-4:35")
PARSE_FUZZED_OK(fuzzed_034, "<+0;>-5")
PARSE_FUZZED_OK(fuzzed_039, "<+054x>-5:45")
PARSE_FUZZED_OK(fuzzed_042, "<+0630>-6:30")
PARSE_FUZZED_OK(fuzzed_043, "<+063s>-6:30")
PARSE_FUZZED_OK(fuzzed_044, "<:06>6")
PARSE_FUZZED_OK(fuzzed_046, "<+06>-9")
PARSE_FUZZED_OK(fuzzed_058, "<-09>1")
PARSE_FUZZED_OK(fuzzed_059, "<-0930>9:35")
PARSE_FUZZED_OK(fuzzed_064, "<-0A>8")
PARSE_FUZZED_OK(fuzzed_065, "<-0F>4")
PARSE_FUZZED_OK(fuzzed_066, "<+0g>-9")
PARSE_FUZZED_OK(fuzzed_067, "<+0i30>-3:30<+0430>,J79/24,J263/24")
PARSE_FUZZED_OK(fuzzed_068, "<+0I>-4")
PARSE_FUZZED_OK(fuzzed_069, "<+0I>-5")
PARSE_FUZZED_OK(fuzzed_070, "<-0k>4")
PARSE_FUZZED_OK(fuzzed_071, "<+0K>-8")
PARSE_FUZZED_OK(fuzzed_072, "<-0m30>9:30")
PARSE_FUZZED_OK(fuzzed_073, "<+0O>-6")
PARSE_FUZZED_OK(fuzzed_074, "<-0P>8")
PARSE_FUZZED_OK(fuzzed_075, "<+0Q30>-5:30")
PARSE_FUZZED_OK(fuzzed_084, "<-11>11")
PARSE_FUZZED_OK(fuzzed_091, "<+12>-12<+13>,M11.2.0,M1.2.3/99")
PARSE_FUZZED_OK(fuzzed_105, "<+1j>-13<+14>,M9.5.0/3,M4.1.0/4")
PARSE_FUZZED_OK(fuzzed_106, "<+1o>-12")
PARSE_FUZZED_OK(fuzzed_107, "<+1Q>-12")
PARSE_FUZZED_OK(fuzzed_108, "<+1s>-10")
PARSE_FUZZED_OK(fuzzed_109, "<202>2")
PARSE_FUZZED_OK(fuzzed_110, "<+38>-8")
PARSE_FUZZED_OK(fuzzed_112, "<603>3<-02>,M3.2.0,M11.1.0")
PARSE_FUZZED_OK(fuzzed_114, "<+92>-2")
PARSE_FUZZED_OK(fuzzed_115, "<a07>7")
PARSE_FUZZED_OK(fuzzed_116, "<-A1>1")
PARSE_FUZZED_OK(fuzzed_118, "ACSs-9:30ACDT,M10.1.0,M4.1.0/3")
PARSE_FUZZED_OK(fuzzed_119, "ACST-1:30")
PARSE_FUZZED_OK(fuzzed_121, "ACSTA9:30")
PARSE_FUZZED_OK(fuzzed_122, "ACSTZ9:30ACDT,M10.1.0,M4.1.0/3")
PARSE_FUZZED_OK(fuzzed_123, "ACSW-9:30")
PARSE_FUZZED_OK(fuzzed_128, "AgST-8")
PARSE_FUZZED_OK(fuzzed_129, "AKS39AKDT,M3.2.0,M11.1.0")
PARSE_FUZZED_OK(fuzzed_131, "AKST9hKDT,M3.2.0,M11.1.0")
PARSE_FUZZED_OK(fuzzed_132, "ASD4")
PARSE_FUZZED_OK(fuzzed_133, "ASR4")
PARSE_FUZZED_OK(fuzzed_134, "aST3:30NDT,M3.2.0,M11.1.0")
PARSE_FUZZED_OK(fuzzed_136, "AST4ADT,M3.2.0,M11.1.7")
PARSE_FUZZED_OK(fuzzed_138, "<+b4>-4")
PARSE_FUZZED_OK(fuzzed_139, "BST5")
PARSE_FUZZED_OK(fuzzed_140, "CAD-2")
PARSE_FUZZED_OK(fuzzed_141, "CATv2")
PARSE_FUZZED_OK(fuzzed_147, "ChfT-10")
PARSE_FUZZED_OK(fuzzed_148, "CSM6")
PARSE_FUZZED_OK(fuzzed_151, "CST5nDT,M3.2.0/0,M11.1.0/1")
PARSE_FUZZED_OK(fuzzed_160, "CSTp8")
PARSE_FUZZED_OK(fuzzed_161, "<d11>-11")
PARSE_FUZZED_OK(fuzzed_162, "dST11")
PARSE_FUZZED_OK(fuzzed_163, "<E01>-1")
PARSE_FUZZED_OK(fuzzed_164, "<-e2>2")
PARSE_FUZZED_OK(fuzzed_166, "EAT-2")
PARSE_FUZZED_OK(fuzzed_167, "EdT-2EEST,M3.5.4/24,M10.5.5/1")
PARSE_FUZZED_OK(fuzzed_168, "EDT-3")
PARSE_FUZZED_OK(fuzzed_170, "EED-2EEST,M3.5.4/24,M10.5.5/1")
PARSE_FUZZED_OK(fuzzed_172, "EET-2EEDT,M3.4.4/48,M10.4.4/49")
PARSE_FUZZED_OK(fuzzed_173, "EET-2EESd,M3.5.0/0,M10.5.0/0")
PARSE_FUZZED_OK(fuzzed_179, "EET-2EEST,M3.5.5/0,M10.5.5/0")
PARSE_FUZZED_OK(fuzzed_185, "EETk2EEST,M3.4.4/48,M10.4.4/49")
PARSE_FUZZED_OK(fuzzed_187, "EfT5")
PARSE_FUZZED_OK(fuzzed_188, "EPT-2")
PARSE_FUZZED_OK(fuzzed_189, "ESm5EDT,M3.2.0,M11.1.0")
PARSE_FUZZED_OK(fuzzed_190, "ESN5")
PARSE_FUZZED_OK(fuzzed_193, "EZT-3")
PARSE_FUZZED_OK(fuzzed_194, "FET-1")
PARSE_FUZZED_OK(fuzzed_195, "fIT-9")
PARSE_FUZZED_OK(fuzzed_196, "FST-9")
PARSE_FUZZED_OK(fuzzed_202, "gST-9")
PARSE_FUZZED_OK(fuzzed_203, "GtT0BST,M3.5.0/1,M10.5.0")
PARSE_FUZZED_OK(fuzzed_204, "<-H1>1")
PARSE_FUZZED_OK(fuzzed_205, "HKT08")
PARSE_FUZZED_OK(fuzzed_206, "HS910")
PARSE_FUZZED_OK(fuzzed_209, "HST10HDT,M3.2.0,M18.1.0")
PARSE_FUZZED_OK(fuzzed_210, "hST-9")
PARSE_FUZZED_OK(fuzzed_212, "HWST-8")
PARSE_FUZZED_OK(fuzzed_214, "IAT-1")
PARSE_FUZZED_OK(fuzzed_215, "ISO-2IDT,M3.4.4/26,M10.5.0")
PARSE_FUZZED_OK(fuzzed_218, "IST-1GMTi,M10.5.0,M3.5.0/1")
PARSE_FUZZED_OK(fuzzed_223, "ISTK5:30")
PARSE_FUZZED_OK(fuzzed_224, "<j01>-1")
PARSE_FUZZED_OK(fuzzed_226, "KST7")
PARSE_FUZZED_OK(fuzzed_227, "KST-8")
PARSE_FUZZED_OK(fuzzed_228, "KSTh9")
PARSE_FUZZED_OK(fuzzed_229, "KSTJ9")
PARSE_FUZZED_OK(fuzzed_231, "<L05>5")
PARSE_FUZZED_OK(fuzzed_233, "LET-2")
PARSE_FUZZED_OK(fuzzed_244, "MSV-3")
PARSE_FUZZED_OK(fuzzed_245, "MvT7")
PARSE_FUZZED_OK(fuzzed_246, "<+N3>-13<+14>,M9.5.0/3,M4.1.0/4")
PARSE_FUZZED_OK(fuzzed_247, "<+n6>-6")
PARSE_FUZZED_OK(fuzzed_248, "NKT-8")
PARSE_FUZZED_OK(fuzzed_258, "PKTa5")
PARSE_FUZZED_OK(fuzzed_259, "PKTH5")
PARSE_FUZZED_OK(fuzzed_261, "pST-8")
PARSE_FUZZED_OK(fuzzed_266, "<+Q845>-8:45")
PARSE_FUZZED_OK(fuzzed_267, "QhST-10")
PARSE_FUZZED_OK(fuzzed_268, "qIT-9")
PARSE_FUZZED_OK(fuzzed_269, "QST7")
PARSE_FUZZED_OK(fuzzed_270, "REST-10")
PARSE_FUZZED_OK(fuzzed_271, "SABT-2")
PARSE_FUZZED_OK(fuzzed_273, "SASY-2")
PARSE_FUZZED_OK(fuzzed_274, "ShST-10")
PARSE_FUZZED_OK(fuzzed_276, "SST11")
PARSE_FUZZED_OK(fuzzed_279, "sTC0")
PARSE_FUZZED_OK(fuzzed_282, "<t11>-11")
PARSE_FUZZED_OK(fuzzed_283, "uST10")
PARSE_FUZZED_OK(fuzzed_286, "VST4")
PARSE_FUZZED_OK(fuzzed_288, "<w07>-7")
PARSE_FUZZED_OK(fuzzed_289, "<W08>8")
PARSE_FUZZED_OK(fuzzed_292, "WDTA-8")
PARSE_FUZZED_OK(fuzzed_298, "WID-9")
PARSE_FUZZED_OK(fuzzed_299, "WIs-7")
PARSE_FUZZED_OK(fuzzed_301, "WvTA-8")
PARSE_FUZZED_OK(fuzzed_303, "yST6")
