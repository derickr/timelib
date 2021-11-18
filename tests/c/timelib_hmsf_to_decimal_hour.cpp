#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <stdio.h>
#include <string.h>


TEST_GROUP(hmsf_to_decimal_hour)
{
	double result;

	void test_convert(int hour, int min, int sec, int us)
	{
		timelib_hmsf_to_decimal_hour(hour, min, sec, us, &result);
	}
};

TEST(hmsf_to_decimal_hour, zero)
{
	test_convert(0, 0, 0, 0);
	DOUBLES_EQUAL(0, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, smallest_positive)
{
	test_convert(0, 0, 0, 1);
	DOUBLES_EQUAL(2.777778e-10, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, one_second_positive)
{
	test_convert(0, 0, 1, 0);
	DOUBLES_EQUAL(0.00027777778, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, six_minute_positive)
{
	test_convert(0, 6, 0, 0);
	DOUBLES_EQUAL(0.1, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, three_hours_positive)
{
	test_convert(3, 0, 0, 0);
	DOUBLES_EQUAL(3, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, three_hours_fifteen_minutes_positive)
{
	test_convert(3, 15, 0, 0);
	DOUBLES_EQUAL(3.25, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, smallest_negative)
{
	test_convert(0, 0, 0, 1);
	DOUBLES_EQUAL(2.777778e-10, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, one_second_negative)
{
	test_convert(0, 0, -1, 0);
	DOUBLES_EQUAL(-0.00027777778, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, six_minute_negative)
{
	test_convert(0, -6, 0, 0);
	DOUBLES_EQUAL(-0.1, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, three_hours_negative)
{
	test_convert(-3, 0, 0, 0);
	DOUBLES_EQUAL(-3, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, three_hours_fifteen_minutes_negative)
{
	test_convert(-3, 15, 0, 0);
	DOUBLES_EQUAL(-3.25, result, 0.00000000001);
}

TEST(hmsf_to_decimal_hour, three_hours_negative_fifteen_minutes_negative)
{
	test_convert(-3, -15, 0, 0);
	DOUBLES_EQUAL(-2.75, result, 0.00000000001);
}
