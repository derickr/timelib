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
	{'p', TIMELIB_FORMAT_TIMEZONE_OFFSET},
	{'T', TIMELIB_FORMAT_TIMEZONE_OFFSET},
	{'O', TIMELIB_FORMAT_TIMEZONE_OFFSET},
	{' ', TIMELIB_FORMAT_WHITESPACE},
	{'y', TIMELIB_FORMAT_YEAR_TWO_DIGIT},
	{'Y', TIMELIB_FORMAT_YEAR_FOUR_DIGIT},
	{'x', TIMELIB_FORMAT_YEAR_EXPANDED},
	{'X', TIMELIB_FORMAT_YEAR_EXPANDED},
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

TEST(parse_date_from_format, tzOffsetHours)
{
	test_parse("2018/01/26 +02:00", "Y/m/d P");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2 * 60 * 60, t->z);

	test_parse("2018/01/26 +02:00", "Y/m/d p");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2 * 60 * 60, t->z);
}

TEST(parse_date_from_format, tzUtc)
{
	test_parse("2018/01/26 13:20:00Z", "Y/m/d H:i:sp");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date_from_format, cannotMixISOWithNatural)
{
	test_parse("2018/01/26", "B/m/d");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(1, errors->error_count);
}

TEST(parse_date_from_format, cannotHaveMeridianBeforeHour)
{
	test_parse("11 Mar 2013 PM 3:34", "d M Y A h:i");
	LONGS_EQUAL(1, errors->error_count);
	LONGS_EQUAL(3, t->h);
}

TEST(parse_date_from_format, cannotHaveMeridianWithoutHour)
{
	test_parse("11 Mar 2013 PM", "d M Y A");
	LONGS_EQUAL(1, errors->error_count);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
}

TEST(parse_date_from_format, cannotHaveDOYBeforeYear)
{
	test_parse("60 2020", "z Y");
	LONGS_EQUAL(1, errors->error_count);
	LONGS_EQUAL(2020, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
}

TEST(parse_date_from_format, DOYAfterLeapYear)
{
	test_parse("2020 60", "Y z");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2020, t->y);
	LONGS_EQUAL(3, t->m);
	LONGS_EQUAL(1, t->d);
}

TEST(parse_date_from_format, DOYAfterYear)
{
	test_parse("2021 60", "Y z");
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(2021, t->y);
	LONGS_EQUAL(3, t->m);
	LONGS_EQUAL(2, t->d);
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

TEST(parse_date_from_format, american00)
{
	test_parse("9/11", "m/d");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(9, t->m);
	LONGS_EQUAL(11, t->d);
}

TEST(parse_date_from_format, american01)
{
	test_parse("09/11", "m/d");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(9, t->m);
	LONGS_EQUAL(11, t->d);
}

TEST(parse_date_from_format, american02)
{
	test_parse("12/22/69", "m/d/y");
	LONGS_EQUAL(2069, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, american03)
{
	test_parse("12/22/70", "m/d/y");
	LONGS_EQUAL(1970, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, american04)
{
	test_parse("12/22/78", "m/d/y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, american05)
{
	test_parse("12/22/1978", "m/d/Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, american06)
{
	test_parse("12/22/2078", "m/d/Y");
	LONGS_EQUAL(2078, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, bug41523_00)
{
	test_parse("0000-00-00", "Y-m-d");
	LONGS_EQUAL(0000, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}

TEST(parse_date_from_format, bug41523_01)
{
	test_parse("0001-00-00", "Y-m-d");
	LONGS_EQUAL(0001, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}

TEST(parse_date_from_format, bug41523_02)
{
	test_parse("0002-00-00", "Y-m-d");
	LONGS_EQUAL(0002, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}

TEST(parse_date_from_format, bug41523_03)
{
	test_parse("0003-00-00", "Y-m-d");
	LONGS_EQUAL(0003, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}

TEST(parse_date_from_format, bug41523_04)
{
	test_parse("00-00-00", "y-m-d");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}

TEST(parse_date_from_format, bug41523_05)
{
	test_parse("01-00-00", "y-m-d");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}

TEST(parse_date_from_format, bug41523_06)
{
	test_parse("02-00-00", "y-m-d");
	LONGS_EQUAL(2002, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}

TEST(parse_date_from_format, bug41523_07)
{
	test_parse("03-00-00", "y-m-d");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(0, t->m);
	LONGS_EQUAL(0, t->d);
}


TEST(parse_date_from_format, bug41842_00)
{
	test_parse("-0001-06-28", "-Y-m-d");
	LONGS_EQUAL(0001, t->y);
	LONGS_EQUAL(6, t->m);
	LONGS_EQUAL(28, t->d);
}

TEST(parse_date_from_format, bug41842_01)
{
	test_parse("-2007-06-28", "-Y-m-d");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL(6, t->m);
	LONGS_EQUAL(28, t->d);
}


TEST(parse_date_from_format, bug55240_00)
{
	test_parse("11.11.2009 800", "d.m.Y Gi");
	LONGS_EQUAL(2009, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(11, t->d);
	LONGS_EQUAL(80, t->h);
	LONGS_EQUAL(0, t->i);
	LONGS_EQUAL(0, t->s);
}

TEST(parse_date_from_format, datefull_00)
{
	test_parse("22 dec 1978", "d F Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_01)
{
	test_parse("22-dec-78", "d-F-y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_02)
{
	test_parse("22 Dec 1978", "d F Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_03)
{
	test_parse("22DEC78", "dFy  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_04)
{
	test_parse("22 december 1978", "d F Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_05)
{
	test_parse("22-december-78", "d-F-y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_06)
{
	test_parse("22 December 1978", "d F Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_07)
{
	test_parse("22DECEMBER78", "dFy  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_08)
{
	test_parse("22	dec	1978", "d?F?Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_09)
{
	test_parse("22	Dec	1978", "d?F?Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_10)
{
	test_parse("22	december	1978", "d?F?Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, datefull_11)
{
	test_parse("22	December	1978", "d?F?Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}


TEST(parse_date_from_format, datenocolon_00)
{
	test_parse("19781222", "Ymd");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}


TEST(parse_date_from_format, date_00)
{
	test_parse("31.01.2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date_from_format, date_01)
{
	test_parse("32.01.2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(32, t->d);
}

TEST(parse_date_from_format, date_02)
{
	test_parse("28.01.2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(28, t->d);
}

TEST(parse_date_from_format, date_03)
{
	test_parse("29.01.2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date_from_format, date_04)
{
	test_parse("30.01.2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(30, t->d);
}

TEST(parse_date_from_format, date_05)
{
	test_parse("31.01.2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date_from_format, date_06)
{
	test_parse("32.01.2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(32, t->d);
}

TEST(parse_date_from_format, date_07)
{
	test_parse("31-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date_from_format, date_08)
{
	test_parse("32-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(32, t->d);
}

TEST(parse_date_from_format, date_09)
{
	test_parse("28-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(28, t->d);
}

TEST(parse_date_from_format, date_10)
{
	test_parse("29-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date_from_format, date_11)
{
	test_parse("30-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(30, t->d);
}

TEST(parse_date_from_format, date_12)
{
	test_parse("31-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date_from_format, date_13)
{
	test_parse("32-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(32, t->d);
}

TEST(parse_date_from_format, date_14)
{
	test_parse("29-02-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(2, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date_from_format, date_15)
{
	test_parse("30-02-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(2, t->m);
	LONGS_EQUAL(30, t->d);
}

TEST(parse_date_from_format, date_16)
{
	test_parse("31-02-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(2, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date_from_format, date_17)
{
	test_parse("01-01-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);
}

TEST(parse_date_from_format, date_18)
{
	test_parse("31-12-2006", "d-m-Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date_from_format, date_19)
{
	test_parse("31-13-2006", "d.m.Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(13, t->m);
	LONGS_EQUAL(31, t->d);
}

TEST(parse_date_from_format, date_20)
{
	test_parse("11/10/2006", "m/d/Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(10, t->d);
}

TEST(parse_date_from_format, date_21)
{
	test_parse("12/10/2006", "m/d/Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(10, t->d);
}

TEST(parse_date_from_format, date_22)
{
	test_parse("13/10/2006", "m/d/Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(13, t->m);
	LONGS_EQUAL(10, t->d);
}

TEST(parse_date_from_format, date_23)
{
	test_parse("14/10/2006", "m/d/Y");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL(14, t->m);
	LONGS_EQUAL(10, t->d);
}


TEST(parse_date_from_format, dateroman_00)
{
	test_parse("22 I 1978", "d F Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_01)
{
	test_parse("22. II 1978", "d. F Y ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(2, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_02)
{
	test_parse("22 III. 1978", "d F. Y ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(3, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_03)
{
	test_parse("22- IV- 1978", "d- F- Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(4, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_04)
{
	test_parse("22 -V -1978", "d -F -Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(5, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_05)
{
	test_parse("22-VI-1978", "d-F-Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(6, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_06)
{
	test_parse("22.VII.1978", "d.F.Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(7, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_07)
{
	test_parse("22 VIII 1978", "d F Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(8, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_08)
{
	test_parse("22 IX 1978", "d F Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(9, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_09)
{
	test_parse("22 X 1978", "d F Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_10)
{
	test_parse("22 XI 1978", "d F Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, dateroman_11)
{
	test_parse("22	XII	1978", "d?F?Y  ");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}


TEST(parse_date_from_format, dateslash_00)
{
	test_parse("2005/8/12", "Y/n/d");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL(8, t->m);
	LONGS_EQUAL(12, t->d);
}

TEST(parse_date_from_format, dateslash_01)
{
	test_parse("2005/01/02", "Y/m/d");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(2, t->d);
}

TEST(parse_date_from_format, dateslash_02)
{
	test_parse("2005/01/2", "Y/m/j");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(2, t->d);
}

TEST(parse_date_from_format, dateslash_03)
{
	test_parse("2005/1/02", "Y/n/d");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(2, t->d);
}

TEST(parse_date_from_format, dateslash_04)
{
	test_parse("2005/1/2", "Y/n/j");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(2, t->d);
}


TEST(parse_date_from_format, lenient_00)
{
	test_parse("2001-11-29 13:20:01", "Y-m-d");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, lenient_01)
{
	test_parse("2001-11-29 13:20:01", "Y-m-d+");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, lenient_02)
{
	test_parse("2001-11-29 13:20:01", "Y-m-d +");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, lenient_03)
{
	test_parse("2001-11-29", "Y-m-d+");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date_from_format, lenient_04)
{
	test_parse("2001-11-29", "Y-m-d +");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
}

TEST(parse_date_from_format, lenient_05)
{
	test_parse("2001-11-29", "Y-m-d +");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
}



TEST(parse_date_from_format, mysql_00)
{
	test_parse("19970523091528", "YmdHis");
	LONGS_EQUAL(1997, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(23, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL(15, t->i);
	LONGS_EQUAL(28, t->s);
}

TEST(parse_date_from_format, mysql_01)
{
	test_parse("20001231185859", "YmdHis");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(58, t->i);
	LONGS_EQUAL(59, t->s);
}

TEST(parse_date_from_format, mysql_02)
{
	test_parse("20500410101010", "YmdHis");
	LONGS_EQUAL(2050, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(10, t->h);
	LONGS_EQUAL(10, t->i);
	LONGS_EQUAL(10, t->s);
}

TEST(parse_date_from_format, mysql_03)
{
	test_parse("20050620091407", "YmdHis");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 6, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 7, t->s);
}


TEST(parse_date_from_format, pgsql_00)
{
	test_parse("January 8, 1999", "F d, Y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_01)
{
	test_parse("January	8,	1999", "F?d,?Y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_02)
{
	test_parse("1999-01-08", "Y-m-d");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_03)
{
	test_parse("1/8/1999", "m/d/Y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_04)
{
	test_parse("1/18/1999", "m/d/Y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(18, t->d);
}

TEST(parse_date_from_format, pgsql_05)
{
	test_parse("01/02/03", "m/d/y");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date_from_format, pgsql_06)
{
	test_parse("1999-Jan-08", "Y-M-d");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_07)
{
	test_parse("Jan-08-1999", "M-d-Y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_08)
{
	test_parse("08-Jan-1999", "d-M-Y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_09)
{
	test_parse("99-Jan-08", "y-M-d");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_10)
{
	test_parse("08-Jan-99", "d-M-y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_11)
{
	test_parse("Jan-08-99", "M-d-y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_12)
{
	test_parse("19990108", "Ymd");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_13)
{
	test_parse("1999.008", "Y.z");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 9, t->d);
}

TEST(parse_date_from_format, pgsql_14)
{
	test_parse("1999.038", "Y.z");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_15)
{
	test_parse("1999.238", "Y.z");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
}

TEST(parse_date_from_format, pgsql_16)
{
	test_parse("1999.366", "Y.z");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}

TEST(parse_date_from_format, pgsql_17)
{
	test_parse("1999008", "Yz");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 9, t->d);
}

TEST(parse_date_from_format, pgsql_18)
{
	test_parse("1999038", "Yz");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 2, t->m);
	LONGS_EQUAL( 8, t->d);
}

TEST(parse_date_from_format, pgsql_19)
{
	test_parse("1999238", "Yz");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
}

TEST(parse_date_from_format, pgsql_20)
{
	test_parse("1999366", "Yz");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 2, t->d);
}


TEST(parse_date_from_format, pointeddate_00)
{
	test_parse("22.12.1978", "d.m.Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, pointeddate_01)
{
	test_parse("22.7.1978", "d.m.Y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, pointeddate_02)
{
	test_parse("22.12.78", "d.m.y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(22, t->d);
}

TEST(parse_date_from_format, pointeddate_03)
{
	test_parse("22.7.78", "d.m.y");
	LONGS_EQUAL(1978, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(22, t->d);
}


TEST(parse_date_from_format, timelong12_00)
{
	test_parse("01:00:03am", "g:i:sa");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
}

TEST(parse_date_from_format, timelong12_01)
{
	test_parse("01:03:12pm", "g:i:sa");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date_from_format, timelong12_02)
{
	test_parse("12:31:13 A.M.", "g:i:s A");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(31, t->i);
	LONGS_EQUAL(13, t->s);
}

TEST(parse_date_from_format, timelong12_03)
{
	test_parse("08:13:14 P.M.", "g:i:s A");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(13, t->i);
	LONGS_EQUAL(14, t->s);
}

TEST(parse_date_from_format, timelong12_04)
{
	test_parse("11:59:15 AM", "g:i:s A");
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(15, t->s);
}

TEST(parse_date_from_format, timelong12_05)
{
	test_parse("06:12:16 PM", "g:i:s A");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(12, t->i);
	LONGS_EQUAL(16, t->s);
}

TEST(parse_date_from_format, timelong12_06)
{
	test_parse("07:08:17 am", "g:i:s a");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(17, t->s);
}

TEST(parse_date_from_format, timelong12_07)
{
	test_parse("08:09:18 p.m.", "g:i:s a");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL(18, t->s);
}

TEST(parse_date_from_format, timelong12_08)
{
	test_parse("01.00.03am", "h:i:sa");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
}

TEST(parse_date_from_format, timelong12_09)
{
	test_parse("01.03.12pm", "h:i:sa");
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date_from_format, timelong12_10)
{
	test_parse("12.31.13 A.M.", "h:i:s A");
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(31, t->i);
	LONGS_EQUAL(13, t->s);
}

TEST(parse_date_from_format, timelong12_11)
{
	test_parse("08.13.14 P.M.", "h:i:s A");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(13, t->i);
	LONGS_EQUAL(14, t->s);
}

TEST(parse_date_from_format, timelong12_12)
{
	test_parse("11.59.15 AM", "h:i:s A");
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(15, t->s);
}

TEST(parse_date_from_format, timelong12_13)
{
	test_parse("06.12.16 PM", "h:i:s A");
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(12, t->i);
	LONGS_EQUAL(16, t->s);
}

TEST(parse_date_from_format, timelong12_14)
{
	test_parse("07.08.17 am", "h:i:s a");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(17, t->s);
}

TEST(parse_date_from_format, timelong12_15)
{
	test_parse("08.09.18 p.m.", "h:i:s a");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL(18, t->s);
}

TEST(parse_date_from_format, timelong12_16)
{
	test_parse("07.08.17	am", "h:i:s a");
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(17, t->s);
}

TEST(parse_date_from_format, timelong12_17)
{
	test_parse("08.09.18	p.m.", "h:i:s a");
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL( 9, t->i);
	LONGS_EQUAL(18, t->s);
}



TEST(parse_date_from_format, bug44426_00)
{
	test_parse("Aug 27 2007 12:00:00:000AM", "M d Y h:i:s:uA");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bug44426_01)
{
	test_parse("Aug 27 2007 12:00:00.000AM", "M d Y h:i:s.uA");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bug44426_02)
{
	test_parse("Aug 27 2007 12:00:00:000", "M d Y h:i:s:u");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bug44426_03)
{
	test_parse("Aug 27 2007 12:00:00.000", "M d Y h:i:s.u");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bug44426_04)
{
	test_parse("Aug 27 2007 12:00:00AM", "M d Y h:i:sA");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bug44426_05)
{
	test_parse("Aug 27 2007", "M d Y h:i:s:uA");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bug44426_06)
{
	test_parse("Aug 27 2007 12:00AM", "M d Y h:iA");
	LONGS_EQUAL(2007, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(27, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}


TEST(parse_date_from_format, bug50392_00)
{
	test_parse("2010-03-06 16:07:25", "Y-m-d H:i:s");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(000000, t->us);
}

TEST(parse_date_from_format, bug50392_01)
{
	test_parse("2010-03-06 16:07:25.", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(000000, t->us);
}

TEST(parse_date_from_format, bug50392_02)
{
	test_parse("2010-03-06 16:07:25.1", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(100000, t->us);
}

TEST(parse_date_from_format, bug50392_03)
{
	test_parse("2010-03-06 16:07:25.12", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(120000, t->us);
}

TEST(parse_date_from_format, bug50392_04)
{
	test_parse("2010-03-06 16:07:25.123", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123000, t->us);
}

TEST(parse_date_from_format, bug50392_05)
{
	test_parse("2010-03-06 16:07:25.1234", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
}

TEST(parse_date_from_format, bug50392_06)
{
	test_parse("2010-03-06 16:07:25.12345", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123450, t->us);
}

TEST(parse_date_from_format, bug50392_07)
{
	test_parse("2010-03-06 16:07:25.123456", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date_from_format, bug50392_08)
{
	test_parse("2010-03-06 16:07:25.1234567", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date_from_format, bug50392_09)
{
	test_parse("2010-03-06 16:07:25.12345678", "Y-m-d H:i:s.u");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123456, t->us);
}


TEST(parse_date_from_format, bug75577_00)
{
	test_parse("2010-03-06 16:07:25.123", "Y-m-d H:i:s.v");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123000, t->us);
}

TEST(parse_date_from_format, bug75577_01)
{
	test_parse("2010-03-06 16:07:25.1234", "Y-m-d H:i:s.v");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123000, t->us);
}

TEST(parse_date_from_format, bug75577_02)
{
	test_parse("2010-03-06 16:07:25.12345", "Y-m-d H:i:s.v");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123000, t->us);
}

TEST(parse_date_from_format, bug75577_03)
{
	test_parse("2010-03-06 16:07:25.123456", "Y-m-d H:i:s.v");
	LONGS_EQUAL(2010, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL( 6, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL( 7, t->i);
	LONGS_EQUAL(25, t->s);
	LONGS_EQUAL(123000, t->us);
}


TEST(parse_date_from_format, bugs_00)
{
	test_parse("04/05/06 0045", "m/d/y Hi");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL( 5, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bugs_01)
{
	test_parse("17:00 2004-01-03", "H:i Y-m-d");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bugs_02)
{
	test_parse("2004-03-10 16:33:17.11403+01", "Y-m-d H:i:s.ue");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL(33, t->i);
	LONGS_EQUAL(17, t->s);
	LONGS_EQUAL(114030, t->us);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, bugs_03)
{
	test_parse("2004-03-10 16:33:17+01", "Y-m-d H:i:se");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL(33, t->i);
	LONGS_EQUAL(17, t->s);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, bugs_04)
{
	test_parse("2003-11-19 08:00:00 T", "Y-m-d H:i:s e");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(19, t->d);
	LONGS_EQUAL( 8, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("T", t->tz_abbr);
	LONGS_EQUAL(-25200, t->z);
}

TEST(parse_date_from_format, bugs_05)
{
	test_parse("01-MAY-1982 00:00:00", "d-M-Y H:i:s");
	LONGS_EQUAL(1982, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL( 1, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bugs_06)
{
	test_parse("2040-06-12T04:32:12", "Y-m-d?H:i:s");
	LONGS_EQUAL(2040, t->y);
	LONGS_EQUAL( 6, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL(32, t->i);
	LONGS_EQUAL(12, t->s);
}

TEST(parse_date_from_format, bugs_07)
{
	test_parse("july 14th", "F dS");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_08)
{
	test_parse("july 14tH", "F d?e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
	STRCMP_EQUAL("H", t->tz_abbr);
	LONGS_EQUAL(28800, t->z);
}

TEST(parse_date_from_format, bugs_09)
{
	test_parse("11Oct", "dF");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(11, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_10)
{
	test_parse("11 Oct", "d F");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(11, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_11)
{
	test_parse("Jan14, 2004", "Fd, Y");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_12)
{
	test_parse("Jan 14, 2004", "F d, Y");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_13)
{
	test_parse("Jan.14, 2004", "F.d, Y");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_14)
{
	test_parse("1999-10-13", "Y-m-d");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(13, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_15)
{
	test_parse("Oct 13  1999", "F d  Y");
	LONGS_EQUAL(1999, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(13, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_16)
{
	test_parse("2000-01-19", "Y-m-d");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(19, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_17)
{
	test_parse("Jan 19  2000", "F d  Y");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL( 1, t->m);
	LONGS_EQUAL(19, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_18)
{
	test_parse("2001-12-21", "Y-m-d");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, bugs_19)
{
	test_parse("Dec 21  12:16", "F d  H:is");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(16, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bugs_20)
{
	test_parse("2001-12-21 12:16", "Y-m-d H:i");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(16, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bugs_21)
{
	test_parse("Dec 21 2001 12:16", "F d Y H:i");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(16, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bugs_22)
{
	test_parse("Dec 21  12:16", "F d  H:is");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(21, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(16, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, bugs_23)
{
	test_parse("2001-10-22 21:19:58", "Y-m-d H:i:s");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
}

TEST(parse_date_from_format, bugs_24)
{
	test_parse("2001-10-22 21:19:58-02", "Y-m-d H:i:se");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-7200, t->z);
}

TEST(parse_date_from_format, bugs_25)
{
	test_parse("2001-10-22 21:19:58-0213", "Y-m-d H:i:se");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-7980, t->z);
}

TEST(parse_date_from_format, bugs_26)
{
	test_parse("2001-10-22 21:19:58+02", "Y-m-d H:i:se");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(7200, t->z);
}

TEST(parse_date_from_format, bugs_27)
{
	test_parse("2001-10-22 21:19:58+0213", "Y-m-d H:i:se");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(7980, t->z);
}

TEST(parse_date_from_format, bugs_28)
{
	test_parse("2001-10-22T21:20:58-03:40", "Y-m-d?H:i:se");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-13200, t->z);
}

TEST(parse_date_from_format, bugs_29)
{
	test_parse("2001-10-22T211958-2", "Y-m-d?Hise");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(-7200, t->z);
}

TEST(parse_date_from_format, bugs_30)
{
	test_parse("20011022T211958+0213", "Ymd?Hise");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(19, t->i);
	LONGS_EQUAL(58, t->s);
	LONGS_EQUAL(7980, t->z);
}

TEST(parse_date_from_format, bugs_31)
{
	test_parse("20011022T21:20+0215", "Ymd?H:ie");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(22, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(8100, t->z);
}


TEST(parse_date_from_format, datenoday_00)
{
	test_parse("Oct 2003", "M Y");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, datenoday_01)
{
	test_parse("20 October 2003", "d F Y");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, datenoday_02)
{
	test_parse("Oct 03", "M d");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL(TIMELIB_UNSET, t->h);
	LONGS_EQUAL(TIMELIB_UNSET, t->i);
	LONGS_EQUAL(TIMELIB_UNSET, t->s);
}

TEST(parse_date_from_format, datenoday_03)
{
	test_parse("Oct 2003 2045", "M Y Hi");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, datenoday_04)
{
	test_parse("Oct 2003 20:45", "M Y H:i");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, datenoday_05)
{
	test_parse("Oct 2003 20:45:37", "M Y H:i:s");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL(37, t->s);
}

TEST(parse_date_from_format, datenoday_06)
{
	test_parse("20 October 2003 00:00 CEST", "d F Y H:i e");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL( 0, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, datenoday_07)
{
	test_parse("Oct 03 21:46m", "M d h:ie");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(46, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("M", t->tz_abbr);
	LONGS_EQUAL(43200, t->z);
}

TEST(parse_date_from_format, datenoday_08)
{
	test_parse("Oct	2003	20:45", "M?Y?H:i");
	LONGS_EQUAL(2003, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(20, t->h);
	LONGS_EQUAL(45, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, datenoday_09)
{
	test_parse("Oct	03	21:46m", "M?d?H:ie");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL( 3, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(46, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("M", t->tz_abbr);
	LONGS_EQUAL(43200, t->z);
}


TEST(parse_date_from_format, epoch_seconds_00)
{
	test_parse("-12219146756", "U");
	LONGS_EQUAL(1582, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(16, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL(34, t->i);
	LONGS_EQUAL( 4, t->s);
}

TEST(parse_date_from_format, epoch_seconds_01)
{
	test_parse("12219146756", "U");
	LONGS_EQUAL(2357, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL(25, t->i);
	LONGS_EQUAL(56, t->s);
}

TEST(parse_date_from_format, epoch_seconds_02)
{
	test_parse("-12219146756 123456", "U u");
	LONGS_EQUAL(1582, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(16, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL(34, t->i);
	LONGS_EQUAL( 4, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date_from_format, epoch_seconds_03)
{
	test_parse("12219146756 123456", "U u");
	LONGS_EQUAL(2357, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL(25, t->i);
	LONGS_EQUAL(56, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date_from_format, epoch_seconds_04)
{
	test_parse("123456 -12219146756", "u U");
	LONGS_EQUAL(1582, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(16, t->d);
	LONGS_EQUAL(16, t->h);
	LONGS_EQUAL(34, t->i);
	LONGS_EQUAL( 4, t->s);
	LONGS_EQUAL(123456, t->us);
}

TEST(parse_date_from_format, epoch_seconds_05)
{
	test_parse("123456 12219146756", "u U");
	LONGS_EQUAL(2357, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 7, t->h);
	LONGS_EQUAL(25, t->i);
	LONGS_EQUAL(56, t->s);
	LONGS_EQUAL(123456, t->us);
}


TEST(parse_date_from_format, iso8601long_00)
{
	test_parse("01:00:03.12345", "H:i:s.u");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
}

TEST(parse_date_from_format, iso8601long_01)
{
	test_parse("13:03:12.45678", "H:i:s.u");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
}


TEST(parse_date_from_format, iso8601longtz_00)
{
	test_parse("01:00:03.12345 CET", "H:i:s.u e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, iso8601longtz_01)
{
	test_parse("13:03:12.45678 CEST", "H:i:s.u e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, iso8601longtz_02)
{
	test_parse("15:57:41.0GMT", "H:i:s.ue");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date_from_format, iso8601longtz_03)
{
	test_parse("15:57:41.0 pdt", "H:i:s.u e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("PDT", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, iso8601longtz_04)
{
	test_parse("23:41:00.0Z", "H:i:s.ue");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("Z", t->tz_abbr);
}

TEST(parse_date_from_format, iso8601longtz_05)
{
	test_parse("23:41:00.0 k", "H:i:s.u e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("K", t->tz_abbr);
	LONGS_EQUAL(36000, t->z);
}

TEST(parse_date_from_format, iso8601longtz_06)
{
	test_parse("04:05:07.789cast", "H:i:s.ue");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 7, t->s);
	LONGS_EQUAL(789000, t->us);
	STRCMP_EQUAL("CAST", t->tz_abbr);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date_from_format, iso8601longtz_07)
{
	test_parse("01:00:03.12345  +1", "H:i:s.u  e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, iso8601longtz_08)
{
	test_parse("13:03:12.45678 +0100", "H:i:s.u e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, iso8601longtz_09)
{
	test_parse("15:57:41.0-0", "H:i:s.ue");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date_from_format, iso8601longtz_10)
{
	test_parse("15:57:41.0-8", "H:i:s.ue");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(15, t->h);
	LONGS_EQUAL(57, t->i);
	LONGS_EQUAL(41, t->s);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date_from_format, iso8601longtz_11)
{
	test_parse("23:41:00.0 -0000", "H:i:s.u e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(41, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, iso8601longtz_12)
{
	test_parse("04:05:07.789 +0930", "H:i:s.u e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 4, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 7, t->s);
	LONGS_EQUAL(789000, t->us);
	LONGS_EQUAL(34200, t->z);
}

TEST(parse_date_from_format, iso8601longtz_13)
{
	test_parse("01:00:03.12345 (CET)", "H:i:s.u (e)");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, iso8601longtz_14)
{
	test_parse("13:03:12.45678 (CEST)", "H:i:s.u (e)");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, iso8601longtz_15)
{
	test_parse("(CET) 01:00:03.12345", "(e) H:i:s.u");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, iso8601longtz_16)
{
	test_parse("(CEST) 13:03:12.45678", "(e) H:i:s.ue");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, iso8601longtz_17)
{
	test_parse("13:03:12.45678	(CEST)", "H:i:s.u?(e)");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, iso8601longtz_18)
{
	test_parse("(CEST)	13:03:12.45678", "(e)?H:i:s.u");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 3, t->i);
	LONGS_EQUAL(12, t->s);
	LONGS_EQUAL(456780, t->us);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}


TEST(parse_date_from_format, iso8601nocolon_00)
{
	test_parse("2314", "Hi");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, iso8601nocolon_01)
{
	test_parse("2314 2314", "Hi Y");
	LONGS_EQUAL(2314, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
}

TEST(parse_date_from_format, iso8601nocolon_02)
{
	test_parse("2314 PST", "Hi e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("PST", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date_from_format, iso8601nocolon_03)
{
	test_parse("231431 CEST", "His e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, iso8601nocolon_04)
{
	test_parse("231431 CET", "His e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, iso8601nocolon_05)
{
	test_parse("231431", "His");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
}

TEST(parse_date_from_format, iso8601nocolon_06)
{
	test_parse("231431 2314", "His Y");
	LONGS_EQUAL(2314, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
}

TEST(parse_date_from_format, iso8601nocolon_07)
{
	test_parse("2314	PST", "Hi?e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("PST", t->tz_abbr);
	LONGS_EQUAL(-28800, t->z);
}

TEST(parse_date_from_format, iso8601nocolon_08)
{
	test_parse("231431	CEST", "His?e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, iso8601nocolon_09)
{
	test_parse("231431	CET", "His?e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
	STRCMP_EQUAL("CET", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, iso8601nocolon_10)
{
	test_parse("231431	2314", "His?Y");
	LONGS_EQUAL(2314, t->y);
	LONGS_EQUAL(TIMELIB_UNSET, t->m);
	LONGS_EQUAL(TIMELIB_UNSET, t->d);
	LONGS_EQUAL(23, t->h);
	LONGS_EQUAL(14, t->i);
	LONGS_EQUAL(31, t->s);
}


TEST(parse_date_from_format, special_00)
{
	test_parse("1998-9-15T09:05:32+4:0", "Y-m-d?H:i:se");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date_from_format, special_01)
{
	test_parse("1998-09-15T09:05:32+04:00", "Y-m-d?H:i:se");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date_from_format, special_02)
{
	test_parse("1998-09-15T09:05:32.912+04:00", "Y-m-d?H:i:s.ue");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(912000, t->us);
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date_from_format, special_03)
{
	test_parse("1998-09-15T09:05:32", "Y-m-d?H:i:s");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
}

TEST(parse_date_from_format, special_04)
{
	test_parse("19980915T09:05:32", "Ymd?H:i:s");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
}

TEST(parse_date_from_format, special_05)
{
	test_parse("19980915t090532", "Ymd?His");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
}

TEST(parse_date_from_format, special_06)
{
	test_parse("1998-09-15T09:05:32+4:9", "Y-m-d?H:i:se");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14940, t->z);
}

TEST(parse_date_from_format, special_07)
{
	test_parse("1998-9-15T09:05:32+4:30", "Y-m-d?H:i:se");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date_from_format, special_08)
{
	test_parse("1998-09-15T09:05:32+04:9", "Y-m-d?H:i:se");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(14940, t->z);
}

TEST(parse_date_from_format, special_09)
{
	test_parse("1998-9-15T09:05:32+04:30", "Y-m-d?H:i:se");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 9, t->m);
	LONGS_EQUAL(15, t->d);
	LONGS_EQUAL( 9, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(32, t->s);
	LONGS_EQUAL(16200, t->z);
}


TEST(parse_date_from_format, tzcorrection_00)
{
	test_parse("+4:30", "e");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date_from_format, tzcorrection_01)
{
	test_parse("+4", "e");
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date_from_format, tzcorrection_02)
{
	test_parse("+1", "e");
	LONGS_EQUAL(3600, t->z);
}

TEST(parse_date_from_format, tzcorrection_03)
{
	test_parse("+14", "P");
	LONGS_EQUAL(50400, t->z);
}

TEST(parse_date_from_format, tzcorrection_04)
{
	test_parse("+42", "P");
	LONGS_EQUAL(151200, t->z);
}

TEST(parse_date_from_format, tzcorrection_05)
{
	test_parse("+4:0", "P");
	LONGS_EQUAL(14400, t->z);
}

TEST(parse_date_from_format, tzcorrection_06)
{
	test_parse("+4:01", "P");
	LONGS_EQUAL(14460, t->z);
}

TEST(parse_date_from_format, tzcorrection_07)
{
	test_parse("+4:30", "T");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date_from_format, tzcorrection_08)
{
	test_parse("+401", "T");
	LONGS_EQUAL(14460, t->z);
}

TEST(parse_date_from_format, tzcorrection_09)
{
	test_parse("+402", "T");
	LONGS_EQUAL(14520, t->z);
}

TEST(parse_date_from_format, tzcorrection_10)
{
	test_parse("+430", "T");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date_from_format, tzcorrection_11)
{
	test_parse("+0430", "O");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date_from_format, tzcorrection_12)
{
	test_parse("+04:30", "O");
	LONGS_EQUAL(16200, t->z);
}

TEST(parse_date_from_format, tzcorrection_13)
{
	test_parse("+04:9", "O");
	LONGS_EQUAL(14940, t->z);
}

TEST(parse_date_from_format, tzcorrection_14)
{
	test_parse("+04:09", "O");
	LONGS_EQUAL(14940, t->z);
}


TEST(parse_date_from_format, tz_identifier_00)
{
	test_parse("01:00:03.12345 Europe/Amsterdam", "H:i:s.u e");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("Europe/Amsterdam", t->tz_info->name);
}

TEST(parse_date_from_format, tz_identifier_01)
{
	test_parse("01:00:03.12345 America/Indiana/Knox", "H:i:s.u e");
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 3, t->s);
	LONGS_EQUAL(123450, t->us);
	STRCMP_EQUAL("America/Indiana/Knox", t->tz_info->name);
}

TEST(parse_date_from_format, tz_identifier_02)
{
	test_parse("2005-07-14 22:30:41 America/Los_Angeles", "Y-m-d H:i:s e");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(22, t->h);
	LONGS_EQUAL(30, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("America/Los_Angeles", t->tz_info->name);
}

TEST(parse_date_from_format, tz_identifier_03)
{
	test_parse("2005-07-14	22:30:41	America/Los_Angeles", "Y-m-d?H:i:s?e");
	LONGS_EQUAL(2005, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(14, t->d);
	LONGS_EQUAL(22, t->h);
	LONGS_EQUAL(30, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("America/Los_Angeles", t->tz_info->name);
}


TEST(parse_date_from_format, bug37017_00)
{
	test_parse("2006-05-12 12:59:59 America/New_York", "Y-m-d H:i:s e");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	STRCMP_EQUAL("America/New_York", t->tz_info->name);
}

TEST(parse_date_from_format, bug37017_01)
{
	test_parse("2006-05-12 13:00:00 America/New_York", "Y-m-d H:i:s e");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("America/New_York", t->tz_info->name);
}

TEST(parse_date_from_format, bug37017_02)
{
	test_parse("2006-05-12 13:00:01 America/New_York", "Y-m-d H:i:s e");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL( 0, t->i);
	LONGS_EQUAL( 1, t->s);
	STRCMP_EQUAL("America/New_York", t->tz_info->name);
}

TEST(parse_date_from_format, bug37017_03)
{
	test_parse("2006-05-12 12:59:59 GMT", "Y-m-d H:i:s e");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(12, t->d);
	LONGS_EQUAL(12, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
}


TEST(parse_date_from_format, bug51393_00)
{
	test_parse("[13/Mar/1969:23:40:00 +0100]", "[d/M/Y:H:i:s O]");
	LONGS_EQUAL(1969, t->y);
	LONGS_EQUAL( 3, t->m);
	LONGS_EQUAL(13, t->d);
	LONGS_EQUAL(23, t->h);	LONGS_EQUAL(40, t->i);
	LONGS_EQUAL( 0, t->s);
	LONGS_EQUAL(3600, t->z);
}


TEST(parse_date_from_format, combined_00)
{
	test_parse("Sat, 24 Apr 2004 21:48:40 +0200", "D, d F Y H:i:s e");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(24, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(48, t->i);
	LONGS_EQUAL(40, t->s);
	LONGS_EQUAL(7200, t->z);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, combined_01)
{
	test_parse("Sun Apr 25 01:05:41 CEST 2004", "D F d H:i:s e Y");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, combined_02)
{
	test_parse("Sun Apr 18 18:36:57 2004", "D F d H:i:s Y");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(18, t->h);
	LONGS_EQUAL(36, t->i);
	LONGS_EQUAL(57, t->s);
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, combined_03)
{
	test_parse("Sat, 24 Apr 2004	21:48:40	+0200", "D, d F Y?H:i:s?e");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(24, t->d);
	LONGS_EQUAL(21, t->h);
	LONGS_EQUAL(48, t->i);
	LONGS_EQUAL(40, t->s);
	LONGS_EQUAL(7200, t->z);
	LONGS_EQUAL(6, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, combined_04)
{
	test_parse("20040425010541 CEST", "YmdHis e");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(41, t->s);
	STRCMP_EQUAL("CEST", t->tz_abbr);
	LONGS_EQUAL(3600, t->z);
	LONGS_EQUAL(1, t->dst);
}

TEST(parse_date_from_format, combined_05)
{
	test_parse("20040425010541", "YmdHis");
	LONGS_EQUAL(2004, t->y);
	LONGS_EQUAL( 4, t->m);
	LONGS_EQUAL(25, t->d);
	LONGS_EQUAL( 1, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL(41, t->s);
}

TEST(parse_date_from_format, combined_06)
{
	test_parse("19980717T14:08:55", "Ymd?H:i:s");
	LONGS_EQUAL(1998, t->y);
	LONGS_EQUAL( 7, t->m);
	LONGS_EQUAL(17, t->d);
	LONGS_EQUAL(14, t->h);
	LONGS_EQUAL( 8, t->i);
	LONGS_EQUAL(55, t->s);
}

TEST(parse_date_from_format, combined_07)
{
	test_parse("10/Oct/2000:13:55:36 -0700", "d/F/Y:H:i:s e");
	LONGS_EQUAL(2000, t->y);
	LONGS_EQUAL(10, t->m);
	LONGS_EQUAL(10, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(55, t->i);
	LONGS_EQUAL(36, t->s);
	LONGS_EQUAL(-25200, t->z);
}

TEST(parse_date_from_format, combined_08)
{
	test_parse("2001-11-29T13:20:01.123", "Y-m-d?H:i:s.u");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL( 1, t->s);
	LONGS_EQUAL(123000, t->us);
}

TEST(parse_date_from_format, combined_09)
{
	test_parse("2001-11-29T13:20:01.123-05:00", "Y-m-d?H:i:s.ue");
	LONGS_EQUAL(2001, t->y);
	LONGS_EQUAL(11, t->m);
	LONGS_EQUAL(29, t->d);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(20, t->i);
	LONGS_EQUAL( 1, t->s);
	LONGS_EQUAL(123000, t->us);
	LONGS_EQUAL(-18000, t->z);
}

TEST(parse_date_from_format, combined_10)
{
	test_parse("Fri Aug 20 11:59:59 1993 GMT", "D F d H:i:s Y e");
	LONGS_EQUAL(1993, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, combined_11)
{
	test_parse("Fri Aug 20 11:59:59 1993 UTC", "D F d H:i:s Y e");
	LONGS_EQUAL(1993, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, combined_12)
{
	test_parse("Fri	Aug	20	 11:59:59	 1993	UTC", "D?F?d?H:i:s?Y?e");
	LONGS_EQUAL(1993, t->y);
	LONGS_EQUAL( 8, t->m);
	LONGS_EQUAL(20, t->d);
	LONGS_EQUAL(11, t->h);
	LONGS_EQUAL(59, t->i);
	LONGS_EQUAL(59, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, combined_13)
{
	test_parse("May 18th 5:05 UTC", "F dS g:i e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 5, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date_from_format, combined_14)
{
	test_parse("May 18th 5:05pm UTC", "F dS g:ia e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date_from_format, combined_15)
{
	test_parse("May 18th 5:05 pm UTC", "F dS g:i a e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date_from_format, combined_16)
{
	test_parse("May 18th 5:05am UTC", "F dS g:ia e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 5, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date_from_format, combined_17)
{
	test_parse("May 18th 5:05 am UTC", "F dS g:i a e");
	LONGS_EQUAL(TIMELIB_UNSET, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL( 5, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}

TEST(parse_date_from_format, combined_18)
{
	test_parse("May 18th 2006 5:05pm UTC", "F dS Y g:ia e");
	LONGS_EQUAL(2006, t->y);
	LONGS_EQUAL( 5, t->m);
	LONGS_EQUAL(18, t->d);
	LONGS_EQUAL(17, t->h);
	LONGS_EQUAL( 5, t->i);
	LONGS_EQUAL( 0, t->s);
	STRCMP_EQUAL("UTC", t->tz_abbr);
	LONGS_EQUAL(0, t->z);
}


TEST(parse_date_from_format, day_00)
{
	test_parse("Monday", "D");
	LONGS_EQUAL(1, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, day_01)
{
	test_parse("Wed", "D");
	LONGS_EQUAL(3, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, day_02)
{
	test_parse("friday", "D");
	LONGS_EQUAL(5, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, day_03)
{
	test_parse("SUNDAY", "D");
	LONGS_EQUAL(0, t->relative.weekday);
	LONGS_EQUAL(1, t->relative.weekday_behavior);
}

TEST(parse_date_from_format, year_expanded_01)
{
	test_parse("20-02", "x-m");
	LONGS_EQUAL(20, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_expanded_02)
{
	test_parse("2002", "x");
	LONGS_EQUAL(2002, t->y);
}

TEST(parse_date_from_format, year_expanded_03)
{
	test_parse("-2022-02", "x-m");
	LONGS_EQUAL(-2022, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_expanded_04)
{
	test_parse("-81120-02", "x-m");
	LONGS_EQUAL(-81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_expanded_05)
{
	test_parse("81120-02", "x-m");
	LONGS_EQUAL(81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_expanded_06)
{
	test_parse("+82120-02", "x-m");
	LONGS_EQUAL(82120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_expanded_07)
{
	test_parse("-81120-02", "-x-m");
	LONGS_EQUAL(81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_expanded_08)
{
	test_parse("02-81120", "m-x");
	LONGS_EQUAL(81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_expanded_09)
{
	test_parse("02--81120", "m-x");
	LONGS_EQUAL(-81120, t->y);
	LONGS_EQUAL( 2, t->m);
}


TEST(parse_date_from_format, year_eXpanded_01)
{
	test_parse("20-02", "X-m");
	LONGS_EQUAL(20, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_eXpanded_02)
{
	test_parse("2002", "X");
	LONGS_EQUAL(2002, t->y);
}

TEST(parse_date_from_format, year_eXpanded_03)
{
	test_parse("-2022-02", "X-m");
	LONGS_EQUAL(-2022, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_eXpanded_04)
{
	test_parse("-81120-02", "X-m");
	LONGS_EQUAL(-81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_eXpanded_05)
{
	test_parse("81120-02", "X-m");
	LONGS_EQUAL(81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_eXpanded_06)
{
	test_parse("+82120-02", "X-m");
	LONGS_EQUAL(82120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_eXpanded_07)
{
	test_parse("-81120-02", "-X-m");
	LONGS_EQUAL(81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_eXpanded_08)
{
	test_parse("02-81120", "m-X");
	LONGS_EQUAL(81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, year_eXpanded_09)
{
	test_parse("02--81120", "m-X");
	LONGS_EQUAL(-81120, t->y);
	LONGS_EQUAL( 2, t->m);
}

TEST(parse_date_from_format, bug_gh9700)
{
	test_parse("2022-02-18T00:00:00+01:00[Europe/Berlin]", "Y-m-d\\TH:i:sP[e]");
	STRCMP_EQUAL("Europe/Berlin", t->tz_info->name);
}

TEST(parse_date_from_format, trailing_with_plus)
{
	test_parse("01:31:PM - 03:00:PM", "h:i:A+");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(13, t->h);
	LONGS_EQUAL(31, t->i);
	LONGS_EQUAL( 0, t->s);
}
