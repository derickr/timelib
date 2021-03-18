#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <string.h>

extern timelib_tzdb *zoneinfo;

TEST_GROUP(render)
{
};

TEST_GROUP(render_zoneinfo)
{
};

#define TIMELIB_LLABS(y) (y < 0 ? (y * -1) : y)

char *render_date(timelib_time *d)
{
	char *tmp = (char*) calloc(1, 1024);
	sprintf(tmp, "%s%04lld-%02lld-%02lld %02lld:%02lld:%02lld.%06lld %s",
		d->y < 0 ? "-" : "", TIMELIB_LLABS(d->y), d->m, d->d, d->h, d->i, d->s, d->us,
		d->zone_type == TIMELIB_ZONETYPE_OFFSET
			? "GMT"
			: (d->tz_abbr ? d->tz_abbr : "??")
		);

	return tmp;
}

#define RENDER_TEST(n,s,ts,tzid) \
	TEST(render, n) { \
		int dummy_error; \
		timelib_tzinfo *tzi = timelib_parse_tzfile(tzid, timelib_builtin_db(), &dummy_error); \
		LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, dummy_error); \
		CHECK(tzi != NULL); \
		timelib_time *t = timelib_time_ctor(); \
		CHECK(t != NULL); \
		timelib_set_timezone(t, tzi); \
		timelib_unixtime2local(t, ts); \
		char *rendered = render_date(t); \
		STRCMP_EQUAL(s, rendered); \
		timelib_time_dtor(t); \
		timelib_tzinfo_dtor(tzi); \
		free(rendered); \
	} \
	\
	TEST(render_zoneinfo, n) { \
		int dummy_error; \
		timelib_tzinfo *tzi = timelib_parse_tzfile(tzid, zoneinfo, &dummy_error); \
		LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, dummy_error); \
		CHECK(tzi != NULL); \
		timelib_time *t = timelib_time_ctor(); \
		CHECK(t != NULL); \
		timelib_set_timezone(t, tzi); \
		timelib_unixtime2local(t, ts); \
		char *rendered = render_date(t); \
		STRCMP_EQUAL(s, rendered); \
		timelib_time_dtor(t); \
		timelib_tzinfo_dtor(tzi); \
		free(rendered); \
	} \

RENDER_TEST(php_bug_30532_01, "2004-10-31 00:00:00.000000 EDT", 1099195200, "America/New_York")
RENDER_TEST(php_bug_30532_02, "2004-10-31 01:00:00.000000 EDT", 1099198800, "America/New_York")
RENDER_TEST(php_bug_30532_03, "2004-10-31 02:00:00.000000 EST", 1099206000, "America/New_York")

RENDER_TEST(php_bug_32086_01, "2004-11-01 00:00:00.000000 -03", 1099278000, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_02, "2004-11-01 23:00:00.000000 -03", 1099360800, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_03, "2004-11-01 23:59:00.000000 -03", 1099364340, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_04, "2004-11-02 01:00:00.000000 -02", 1099364400, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_05, "2004-11-02 02:00:00.000000 -02", 1099368000, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_06, "2005-02-18 23:00:00.000000 -02", 1108774800, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_07, "2005-02-19 00:00:00.000000 -02", 1108778400, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_08, "2005-02-19 01:00:00.000000 -02", 1108782000, "America/Sao_Paulo")
RENDER_TEST(php_bug_32086_09, "2005-02-20 00:00:00.000000 -03", 1108868400, "America/Sao_Paulo")

RENDER_TEST(php_bug_32555_01, "2005-04-02 02:30:00.000000 EST", 1112427000, "America/New_York")
RENDER_TEST(php_bug_32555_02, "2005-04-02 00:00:00.000000 EST", 1112418000, "America/New_York")
RENDER_TEST(php_bug_32555_03, "2005-04-03 00:00:00.000000 EST", 1112504400, "America/New_York")

RENDER_TEST(php_bug_32588_01, "2005-04-02 00:00:00.000000 GMT", 1112400000, "GMT")
RENDER_TEST(php_bug_32588_02, "2005-04-03 00:00:00.000000 GMT", 1112486400, "GMT")
RENDER_TEST(php_bug_32588_03, "2005-04-04 00:00:00.000000 GMT", 1112572800, "GMT")

RENDER_TEST(php_bug_73294_01, "-1900-06-22 00:00:00.000000 UTC", -122110502400, "UTC")
RENDER_TEST(php_bug_73294_02, "-1916-06-22 00:00:00.000000 UTC", -122615337600, "UTC")

RENDER_TEST(first_transition_01, "1884-08-06 06:39:33.000000 PST", -2695022427, "America/Los_Angeles")
RENDER_TEST(first_transition_02, "1900-08-06 06:39:33.000000 PST", -2190187227, "America/Los_Angeles")
RENDER_TEST(first_transition_03, "1901-08-06 06:39:33.000000 PST", -2158651227, "America/Los_Angeles")
RENDER_TEST(first_transition_04, "1902-08-06 06:39:33.000000 PST", -2127115227, "America/Los_Angeles")
RENDER_TEST(first_transition_05, "1918-02-06 06:39:33.000000 PST", -1637832027, "America/Los_Angeles")
RENDER_TEST(first_transition_06, "1918-08-06 06:39:33.000000 PDT", -1622197227, "America/Los_Angeles")

RENDER_TEST(issue0017_render_2369_01, "2369-12-31 00:00:00.000000 UTC", 12622694400, "UTC")

RENDER_TEST(past_leap_01, "1965-01-01 00:00:00.000000 UTC", -157766400, "UTC")
RENDER_TEST(past_leap_02, "1964-12-31 00:00:00.000000 UTC", -157852800, "UTC")
RENDER_TEST(past_leap_03, "1964-01-31 00:00:00.000000 UTC", -186796800, "UTC")
RENDER_TEST(past_leap_04, "1964-01-01 00:00:00.000000 UTC", -189388800, "UTC")
RENDER_TEST(past_leap_05, "1963-12-31 00:00:00.000000 UTC", -189475200, "UTC")

RENDER_TEST(render_01, "2005-05-26 23:11:59.000000 CEST", 1117141919, "Europe/Amsterdam")
RENDER_TEST(render_02, "2005-05-26 22:11:59.000000 BST",  1117141919, "Europe/London")
RENDER_TEST(render_03, "2005-05-27 07:11:59.000000 AEST", 1117141919, "Australia/Sydney")
RENDER_TEST(render_04, "2005-10-30 00:00:00.000000 GMT",  1130630400, "GMT")
RENDER_TEST(render_05, "2005-10-30 00:59:59.000000 GMT",  1130633999, "GMT")
RENDER_TEST(render_06, "2005-10-30 01:00:00.000000 GMT",  1130634000, "GMT")
RENDER_TEST(render_07, "2005-10-30 02:00:00.000000 CEST", 1130630400, "Europe/Oslo")
RENDER_TEST(render_08, "2005-10-30 02:59:59.000000 CEST", 1130633999, "Europe/Oslo")
RENDER_TEST(render_09, "2005-10-30 02:00:00.000000 CET",  1130634000, "Europe/Oslo")
RENDER_TEST(render_10, "2005-10-30 01:00:00.000000 CEST", 1130626800, "Europe/Amsterdam")
RENDER_TEST(render_11, "2005-10-30 02:00:00.000000 CEST", 1130630400, "Europe/Amsterdam")
RENDER_TEST(render_12, "2005-10-30 02:00:00.000000 CET",  1130634000, "Europe/Amsterdam")
RENDER_TEST(render_13, "2005-10-30 03:00:00.000000 CET",  1130637600, "Europe/Amsterdam")
RENDER_TEST(render_14, "2005-10-30 04:00:00.000000 CET",  1130641200, "Europe/Amsterdam")
RENDER_TEST(render_15, "2006-06-07 19:06:44.000000 CEST", 1149700004, "Europe/Amsterdam")

RENDER_TEST(transition_01, "2008-03-30 01:00:00.000000 CET",  1206835200, "Europe/Amsterdam")
RENDER_TEST(transition_02, "2008-03-30 01:59:59.000000 CET",  1206838799, "Europe/Amsterdam")
RENDER_TEST(transition_03, "2008-03-30 03:00:00.000000 CEST", 1206838800, "Europe/Amsterdam")
RENDER_TEST(transition_04, "2008-03-30 03:59:59.000000 CEST", 1206842399, "Europe/Amsterdam")
RENDER_TEST(transition_05, "2008-03-30 03:00:00.000000 CEST", 1206838800, "Europe/Amsterdam")

RENDER_TEST(weird_timezone_01, "1970-01-04 21:47:17.000000 -0830", 368237,    "Pacific/Pitcairn")
RENDER_TEST(weird_timezone_02, "1998-04-27 08:29:59.000000 UTC",   893665799, "UTC")
RENDER_TEST(weird_timezone_03, "1998-04-27 08:30:00.000000 UTC",   893665800, "UTC")
RENDER_TEST(weird_timezone_04, "1998-04-27 08:30:01.000000 UTC",   893665801, "UTC")
RENDER_TEST(weird_timezone_05, "1998-04-26 23:59:59.000000 -0830", 893665799, "Pacific/Pitcairn")
RENDER_TEST(weird_timezone_06, "1998-04-27 00:30:00.000000 -08",   893665800, "Pacific/Pitcairn")
RENDER_TEST(weird_timezone_07, "1998-04-27 00:30:01.000000 -08",   893665801, "Pacific/Pitcairn")
