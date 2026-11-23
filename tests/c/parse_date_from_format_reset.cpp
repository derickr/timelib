#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <stdio.h>
#include <string.h>

TEST_GROUP(parse_date_from_format_reset)
{
	timelib_time            *t;
	timelib_time            *fallback;
	timelib_error_container *errors;

	/* The tz callback is a plain function pointer, so it can not capture the
	 * test: it records the tzinfo it returned here instead. The teardown frees
	 * the one a "!" dropped, as the parsed time no longer points at it. */
	static timelib_tzinfo *last_tz;

	static timelib_tzinfo *recording_tz_get(const char *name, const timelib_tzdb *db, int *error_code)
	{
		last_tz = timelib_parse_tzfile(name, db, error_code);
		return last_tz;
	}

	TEST_SETUP()
	{
		t = NULL;
		fallback = NULL;
		errors = NULL;
		last_tz = NULL;
	}

	void parse(const char *input, const char *format)
	{
		char *formatCopy = strdup(format);
		char *inputCopy = strdup(input);
		t = timelib_parse_from_format(formatCopy,
									  inputCopy,
									  strlen(inputCopy),
									  &errors,
									  timelib_builtin_db(),
									  recording_tz_get);
		free(formatCopy);
		free(inputCopy);
		LONGS_EQUAL(0, errors->error_count);
	}

	void set_fallback_abbr(const char *abbr, timelib_sll utc_offset, int dst)
	{
		timelib_abbr_info info = { utc_offset, (char *) abbr, dst };

		fallback = timelib_time_ctor();
		timelib_set_timezone_from_abbr(fallback, info);
	}

	void set_fallback_offset(timelib_sll utc_offset)
	{
		fallback = timelib_time_ctor();
		timelib_set_timezone_from_offset(fallback, utc_offset);
	}

	void set_fallback_id(const char *name)
	{
		int error_code;

		fallback = timelib_time_ctor();
		timelib_set_timezone(fallback, timelib_parse_tzfile(name, timelib_builtin_db(), &error_code));
	}

	/* timelib_fill_holes() clones the fallback tzinfo into 't' */
	void fill_holes()
	{
		timelib_fill_holes(t, fallback, 0);
	}

	void check_zone_is_fallback()
	{
		LONGS_EQUAL(fallback->zone_type, t->zone_type);
		LONGS_EQUAL(fallback->z, t->z);
		LONGS_EQUAL(fallback->dst, t->dst);
		if (fallback->tz_abbr) {
			STRCMP_EQUAL(fallback->tz_abbr, t->tz_abbr);
		} else {
			POINTERS_EQUAL(NULL, t->tz_abbr);
		}
		if (t->zone_type == TIMELIB_ZONETYPE_ID) {
			CHECK(t->tz_info != NULL);
			STRCMP_EQUAL(fallback->tz_info->name, t->tz_info->name);
		} else {
			POINTERS_EQUAL(NULL, t->tz_info);
		}
	}

	TEST_TEARDOWN()
	{
		if (fallback) {
			if (fallback->tz_info) {
				timelib_tzinfo_dtor(fallback->tz_info);
			}
			timelib_time_dtor(fallback);
		}
		/* Dropped by "!", or never attached to 't' */
		if (last_tz && (!t || last_tz != t->tz_info)) {
			timelib_tzinfo_dtor(last_tz);
		}
		if (t) {
			if (t->tz_info) {
				timelib_tzinfo_dtor(t->tz_info);
			}
			timelib_time_dtor(t);
		}
		if (errors) {
			timelib_error_container_dtor(errors);
		}
	}
};

timelib_tzinfo *TEST_GROUP_CppUTestGroupparse_date_from_format_reset::last_tz = NULL;

TEST(parse_date_from_format_reset, identifierIsDroppedByReset)
{
	parse("Europe/London 2024-10-27 01:30", "e !Y-m-d H:i");
	LONGS_EQUAL(2024, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL(0, t->zone_type);
	LONGS_EQUAL(0, t->have_zone);
	LONGS_EQUAL(0, t->is_localtime);
	LONGS_EQUAL(TIMELIB_UNSET, t->z);
	LONGS_EQUAL(TIMELIB_UNSET, t->dst);
	POINTERS_EQUAL(NULL, t->tz_abbr);
	POINTERS_EQUAL(NULL, t->tz_info);
}

TEST(parse_date_from_format_reset, abbreviationIsDroppedByReset)
{
	parse("BST 2024-10-27 01:30", "T !Y-m-d H:i");
	LONGS_EQUAL(0, t->zone_type);
	LONGS_EQUAL(0, t->have_zone);
	LONGS_EQUAL(0, t->is_localtime);
	LONGS_EQUAL(TIMELIB_UNSET, t->z);
	LONGS_EQUAL(TIMELIB_UNSET, t->dst);
	POINTERS_EQUAL(NULL, t->tz_abbr);
	POINTERS_EQUAL(NULL, t->tz_info);
}

TEST(parse_date_from_format_reset, offsetIsDroppedByReset)
{
	parse("+05:00 2024-10-27 01:30", "P !Y-m-d H:i");
	LONGS_EQUAL(0, t->zone_type);
	LONGS_EQUAL(0, t->have_zone);
	LONGS_EQUAL(0, t->is_localtime);
	LONGS_EQUAL(TIMELIB_UNSET, t->z);
	LONGS_EQUAL(TIMELIB_UNSET, t->dst);
	POINTERS_EQUAL(NULL, t->tz_abbr);
	POINTERS_EQUAL(NULL, t->tz_info);
}

TEST(parse_date_from_format_reset, identifierThenAbbreviationFallback)
{
	parse("Europe/London 2024-10-27 01:30", "e !Y-m-d H:i");
	set_fallback_abbr("EST", -5 * 3600, 0);
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_ABBR, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, identifierThenOffsetFallback)
{
	parse("Europe/London 2024-10-27 01:30", "e !Y-m-d H:i");
	set_fallback_offset(3600);
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_OFFSET, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, identifierThenIdentifierFallback)
{
	parse("Europe/London 2024-10-27 01:30", "e !Y-m-d H:i");
	set_fallback_id("America/New_York");
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_ID, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, abbreviationThenAbbreviationFallback)
{
	parse("BST 2024-10-27 01:30", "T !Y-m-d H:i");
	set_fallback_abbr("EST", -5 * 3600, 0);
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_ABBR, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, abbreviationThenOffsetFallback)
{
	parse("BST 2024-10-27 01:30", "T !Y-m-d H:i");
	set_fallback_offset(3600);
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_OFFSET, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, abbreviationThenIdentifierFallback)
{
	parse("BST 2024-10-27 01:30", "T !Y-m-d H:i");
	set_fallback_id("America/New_York");
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_ID, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, offsetThenAbbreviationFallback)
{
	parse("+05:00 2024-10-27 01:30", "P !Y-m-d H:i");
	set_fallback_abbr("EST", -5 * 3600, 0);
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_ABBR, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, offsetThenIdentifierFallback)
{
	parse("+05:00 2024-10-27 01:30", "P !Y-m-d H:i");
	set_fallback_id("America/New_York");
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_ID, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, offsetThenOffsetFallback)
{
	parse("+05:00 2024-10-27 01:30", "P !Y-m-d H:i");
	set_fallback_offset(3600);
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_OFFSET, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, trailingResetAfterZone)
{
	parse("2024-10-27 01:30 Europe/Paris", "Y-m-d H:i e!");
	set_fallback_abbr("EST", -5 * 3600, 0);
	fill_holes();
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL(TIMELIB_ZONETYPE_ABBR, t->zone_type);
	check_zone_is_fallback();
}

TEST(parse_date_from_format_reset, zoneAfterResetIsKept)
{
	parse("2024-10-27 01:30 Europe/Paris", "!Y-m-d H:i e");
	set_fallback_abbr("EST", -5 * 3600, 0);
	fill_holes();
	LONGS_EQUAL(TIMELIB_ZONETYPE_ID, t->zone_type);
	STRCMP_EQUAL("Europe/Paris", t->tz_info->name);
}
