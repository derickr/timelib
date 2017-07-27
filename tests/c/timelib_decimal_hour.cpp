#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(decimal_hour)
{
};

TEST(decimal_hour, convertPositiveHMSToDecimal)
{
	int hour = 2, min = 19, sec = 48;
	double d;

	timelib_hms_to_decimal_hour(hour, min, sec, &d);

	DOUBLES_EQUAL(2.33, d, 0.00001);
}

TEST(decimal_hour, convertZeroHMSToDecimal)
{
	int hour = 0, min = 0, sec = 0;
	double d;

	timelib_hms_to_decimal_hour(hour, min, sec, &d);

	DOUBLES_EQUAL(0, d, 0.00001);
}

TEST(decimal_hour, convertNegativeHMSToDecimal)
{
	int hour = -2, min = 20, sec = 0;
	double d;

	timelib_hms_to_decimal_hour(hour, min, sec, &d);

	DOUBLES_EQUAL(-2.333333, d, 0.000001);
}

TEST(decimal_hour, convertNegativeZeroHMSToDecimal)
{
	int hour = -0, min = 0, sec = 0;
	double d;

	timelib_hms_to_decimal_hour(hour, min, sec, &d);

	DOUBLES_EQUAL(0, d, 0.00001);
}

TEST(decimal_hour, convertPositiveDecimalToHMS)
{
	double d = 2.33;
	int hour, min, sec;

	timelib_decimal_hour_to_hms(d, &hour, &min, &sec);

	LONGS_EQUAL(2, hour);
	LONGS_EQUAL(19, min);
	LONGS_EQUAL(48, sec);
}

TEST(decimal_hour, convertZeroDecimalToHMS)
{
	double d = 0;
	int hour, min, sec;

	timelib_decimal_hour_to_hms(d, &hour, &min, &sec);

	LONGS_EQUAL(0, hour);
	LONGS_EQUAL(0, min);
	LONGS_EQUAL(0, sec);
}

TEST(decimal_hour, convertNegativeDecimalToHMS)
{
	double d = -2.33;
	int hour, min, sec;

	timelib_decimal_hour_to_hms(d, &hour, &min, &sec);

	LONGS_EQUAL(-2, hour);
	LONGS_EQUAL(19, min);
	LONGS_EQUAL(48, sec);
}

TEST(decimal_hour, convertNegativeZeroDecimalToHMS)
{
	double d = -0;
	int hour, min, sec;

	timelib_decimal_hour_to_hms(d, &hour, &min, &sec);

	LONGS_EQUAL(-0, hour);
	LONGS_EQUAL(0, min);
	LONGS_EQUAL(0, sec);
}
