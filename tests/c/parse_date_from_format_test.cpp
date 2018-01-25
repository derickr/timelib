#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <stdio.h>
#include <string.h>

static const timelib_format_specifier default_format_map[] = {
	{'+', TIMELIB_FORMAT_ALLOW_EXTRA_CHARACTERS},
	{'#', TIMELIB_FORMAT_ANY_SEPARATOR},
	{'j', TIMELIB_FORMAT_DAY_TWO_DIGIT},
	{'d', TIMELIB_FORMAT_DAY_TWO_DIGIT_PADDED},
	{'z', TIMELIB_FORMAT_DAY_OF_YEAR},
	{'S', TIMELIB_FORMAT_DAY_SUFFIX},
	{'U', TIMELIB_FORMAT_EPOCH_SECONDS},
	{'\\', TIMELIB_FORMAT_ESCAPE},
	{'h', TIMELIB_FORMAT_HOUR_TWO_DIGIT_12_MAX},
	{'g', TIMELIB_FORMAT_HOUR_TWO_DIGIT_12_MAX_PADDED},
	{'H', TIMELIB_FORMAT_HOUR_TWO_DIGIT_24_MAX},
	{'G', TIMELIB_FORMAT_HOUR_TWO_DIGIT_24_MAX_PADDED},
	{'a', TIMELIB_FORMAT_MERIDIAN},
	{'A', TIMELIB_FORMAT_MERIDIAN},
	{'u', TIMELIB_FORMAT_MICROSECOND_SIX_DIGIT},
	{'v', TIMELIB_FORMAT_MILLISECOND_THREE_DIGIT},
	{'i', TIMELIB_FORMAT_MINUTE_TWO_DIGIT},
	{'n', TIMELIB_FORMAT_MONTH_TWO_DIGIT},
	{'m', TIMELIB_FORMAT_MONTH_TWO_DIGIT_PADDED},
	{'?', TIMELIB_FORMAT_RANDOM_CHAR},
	{'!', TIMELIB_FORMAT_RESET_ALL},
	{'|', TIMELIB_FORMAT_RESET_ALL_WHEN_NOT_SET},
	{'s', TIMELIB_FORMAT_SECOND_TWO_DIGIT},
	{';', TIMELIB_FORMAT_SEPARATOR},
	{':', TIMELIB_FORMAT_SEPARATOR},
	{'/', TIMELIB_FORMAT_SEPARATOR},
	{'.', TIMELIB_FORMAT_SEPARATOR},
	{',', TIMELIB_FORMAT_SEPARATOR},
	{'-', TIMELIB_FORMAT_SEPARATOR},
	{'(', TIMELIB_FORMAT_SEPARATOR},
	{')', TIMELIB_FORMAT_SEPARATOR},
	{'*', TIMELIB_FORMAT_SKIP_TO_SEPARATOR},
	{'D', TIMELIB_FORMAT_TEXTUAL_DAY_3_LETTER},
	{'l', TIMELIB_FORMAT_TEXTUAL_DAY_FULL},
	{'M', TIMELIB_FORMAT_TEXTUAL_MONTH_3_LETTER},
	{'F', TIMELIB_FORMAT_TEXTUAL_MONTH_FULL},
	{'e', TIMELIB_FORMAT_TIMEZONE_OFFSET},
	{'P', TIMELIB_FORMAT_TIMEZONE_OFFSET},
	{'T', TIMELIB_FORMAT_TIMEZONE_OFFSET},
	{'O', TIMELIB_FORMAT_TIMEZONE_OFFSET},
	{' ', TIMELIB_FORMAT_WHITESPACE},
	{'y', TIMELIB_FORMAT_YEAR_TWO_DIGIT},
	{'Y', TIMELIB_FORMAT_YEAR_FOUR_DIGIT},
	{'B', TIMELIB_FORMAT_YEAR_ISO},
	{'b', TIMELIB_FORMAT_DAY_OF_WEEK_ISO},
	{'V', TIMELIB_FORMAT_WEEK_OF_YEAR_ISO},
	{'Z', TIMELIB_FORMAT_TIMEZONE_OFFSET_MINUTES},
	{'\0', TIMELIB_FORMAT_END}
};

static const timelib_format_config default_format_config = {
	default_format_map,
	'\0'
};

static const timelib_format_config format_config_prefix = {
	default_format_map,
	'%'
};

TEST_GROUP(parse_date_from_format)
{
	timelib_time *t;
	int           i;
	timelib_error_container *errors;


	void test_parse(const char *input, const char *format, const timelib_format_config *config)
	{
		if (t) {
			timelib_dump_date(t, 1);
			if (t->tz_info) {
				timelib_tzinfo_dtor(t->tz_info);
			}
			timelib_time_dtor(t);
			timelib_error_container_dtor(errors);
		}

		char *formatCopy = strdup(format);
		char *inputCopy = strdup(input);
		t = timelib_parse_from_format_with_map(formatCopy, 
											   inputCopy,
											   strlen(inputCopy), 
											   &errors, 
											   timelib_builtin_db(), 
											   timelib_parse_tzfile, 
											   config);
		free(formatCopy);
		free(inputCopy);
	}

	void test_parse(const char *input, const char *format)
	{
		test_parse(input, format, &default_format_config);
	}

	void test_parse_with_prefix(const char *input, const char *format)
	{
		test_parse(input, format, &format_config_prefix);
	}

	TEST_TEARDOWN()
	{   
		timelib_dump_date(t, 1);
		if (t->tz_info) {
			timelib_tzinfo_dtor(t->tz_info);
		}
		timelib_time_dtor(t);
		timelib_error_container_dtor(errors);
	}
};

TEST(parse_date_from_format, naturalDateWithoutPrefix)
{
	test_parse("2018/01/26", "Y/m/d");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(26, t->d);
}

TEST(parse_date_from_format, isoDateWithoutPrefix)
{
	/* Week(V).Day(b).Year(B) */
	test_parse("53.7.2017", "V.b.B");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(7, t->d);

	/* Week/Year */
	test_parse("53/2017", "V/B");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);

	test_parse("2017", "B");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2017, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(2, t->d);
}

TEST(parse_date_from_format, invalidISOWeek)
{
	test_parse("55/2017", "V/B");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(1, errors->error_count);

	test_parse("52", "V");
	LONGS_EQUAL(1, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
}

TEST(parse_date_from_format, invalidISODayOfWeek)
{
	test_parse("8", "b");
	LONGS_EQUAL(1, errors->warning_count);
	LONGS_EQUAL(1, errors->error_count);
}

TEST(parse_date_from_format, tzOffsetMinutes)
{
	test_parse("2018/01/26 +285", "Y/m/d Z");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(285 * 60, t->z);
}

TEST(parse_date_from_format, cannotMixISOWithNatural)
{
	test_parse("2018/01/26", "B/m/d");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(1, errors->error_count);
}

TEST(parse_date_from_format, naturalDateWithPrefix)
{
	test_parse_with_prefix("Year 2018 Month 01 Day 26", "Year %Y Month %m Day %d");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(26, t->d);
}

TEST(parse_date_from_format, naturalDateWithTime)
{
	test_parse_with_prefix("2018-01-26T11:56:02Z", "%Y-%m-%dT%H:%i:%sZ");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(26, t->d);
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(56, t->i);
	LONGS_EQUAL(2, t->s);
}

TEST(parse_date_from_format, isoDateWithPrefix)
{
	/* Week(V).Day(b).Year(B) */
	test_parse_with_prefix("53.7.2017", "%V.%b.%B");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(7, t->d);

	/* Week/Year */
	test_parse_with_prefix("53/2017", "%V/%B");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2018, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);

	test_parse_with_prefix("2017", "%B");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2017, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(2, t->d);
}

TEST(parse_date_from_format, missingPrefix)
{
	test_parse_with_prefix("2018/01/26", "Y/m/d");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(6, errors->error_count);
}

TEST(parse_date_from_format, prefixEscape)
{
	test_parse_with_prefix("%", "%%");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);

	test_parse_with_prefix("%Y", "%%Y");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
}

TEST(parse_date_from_format, unmatchedPrefix)
{
	test_parse_with_prefix("2018", "%Y%m");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(1, errors->error_count);
}
