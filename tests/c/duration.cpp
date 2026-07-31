#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <stdio.h>
#include <string.h>


TEST_GROUP(duration)
{
	timelib_duration *d, *d2, *d_mod;
	int error_code;

	TEST_SETUP()
	{
		d = NULL;
		d2 = NULL;
		d_mod = NULL;
	}

	TEST_TEARDOWN()
	{
		if (d) {
			timelib_duration_dtor(d);
		}
		if (d2) {
			timelib_duration_dtor(d2);
		}
		if (d_mod) {
			timelib_duration_dtor(d_mod);
		}
	}
};

TEST(duration, construct_with_negative_seconds)
{
	d = timelib_duration_ctor(-1 , 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_SECONDS_OUT_OF_RANGE, error_code);
	POINTERS_EQUAL(NULL, d);
}

TEST(duration, construct_with_max_seconds)
{
	d = timelib_duration_ctor(9223372035, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	LONGS_EQUAL(9223372035, d->seconds);
	LONGS_EQUAL(0, d->nanoseconds);
	LONGS_EQUAL(false, d->negative);
}

TEST(duration, construct_with_out_of_range_seconds)
{
	d = timelib_duration_ctor(9223372035 + 1, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_SECONDS_OUT_OF_RANGE, error_code);
	POINTERS_EQUAL(NULL, d);
}

TEST(duration, construct_with_negative_nanoseconds)
{
	d = timelib_duration_ctor(0, -1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NANOSECONDS_OUT_OF_RANGE, error_code);
	POINTERS_EQUAL(NULL, d);
}

TEST(duration, construct_with_out_of_range_nanoseconds)
{
	d = timelib_duration_ctor(0, 1000000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NANOSECONDS_OUT_OF_RANGE, error_code);
	POINTERS_EQUAL(NULL, d);
}

TEST(duration, add_0ns_to_0ns)
{
	d = timelib_duration_ctor(0, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_1ns_to_0ns)
{
	d = timelib_duration_ctor(0, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(1, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_0ns_to_1ns)
{
	d = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(1, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_1ns_to_999999999ns)
{
	d = timelib_duration_ctor(0, 999999999, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(1, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_minus1ns_to_1ns)
{
	d = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 1, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_minus1ns_to_0ns)
{
	d = timelib_duration_ctor(0, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 1, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(1, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_minus2ns_to_1ns)
{
	d = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 2, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(1, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_2ns_to_minus1ns)
{
	d = timelib_duration_ctor(0, 1, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 2, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(1, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_1s_to_minus999999999ns)
{
	d = timelib_duration_ctor(0, 999999999, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(1, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(1, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_minus1s_to_999999999ns)
{
	d = timelib_duration_ctor(0, 999999999, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(1, 0, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(1, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, add_1ns_to_9223372035s999999999ns)
{
	d = timelib_duration_ctor(9223372035, 999999999, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_SECONDS_OUT_OF_RANGE, error_code);
	POINTERS_EQUAL(NULL, d_mod);
}

TEST(duration, add_999999999ns_to_999999999ns)
{
	d = timelib_duration_ctor(0, 999999999, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d2 = timelib_duration_ctor(0, 999999999, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_add(d, d2, &error_code);
	LONGS_EQUAL(1, d_mod->seconds);
	LONGS_EQUAL(999999998, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

/** TODO: SUB **/

TEST(duration, multiply_by_0)
{
	d = timelib_duration_ctor(2, 500000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 0, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negative_multiply_by_0)
{
	d = timelib_duration_ctor(2, 500000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 0, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, multiply_by_1)
{
	d = timelib_duration_ctor(2, 500000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 1, &error_code);
	LONGS_EQUAL(2, d_mod->seconds);
	LONGS_EQUAL(500000000, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negative_multiply_by_1)
{
	d = timelib_duration_ctor(2, 500000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 1, &error_code);
	LONGS_EQUAL(2, d_mod->seconds);
	LONGS_EQUAL(500000000, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, multiply_by_4)
{
	d = timelib_duration_ctor(2, 500000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 4, &error_code);
	LONGS_EQUAL(10, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negative_multiply_by_4)
{
	d = timelib_duration_ctor(2, 500000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 4, &error_code);
	LONGS_EQUAL(10, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, multiply_1_nanosecond_by_uintmax)
{
	d = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, UINT64_MAX, &error_code);
	LONGS_EQUAL(18446744073, d_mod->seconds);
	LONGS_EQUAL(709551615, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, multiply_2_nanoseconds_by_uintmax)
{
	d = timelib_duration_ctor(0, 2, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, UINT64_MAX, &error_code);
	LONGS_EQUAL(36893488147, d_mod->seconds);
	LONGS_EQUAL(419103230, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, multiply_uint32max_minus_1_seconds_by_uint32max)
{
	d = timelib_duration_ctor(4294967295, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 4294967296, &error_code);
	UNSIGNED_LONGS_EQUAL(18446744069414584320, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, multiply_uint32max_seconds_by_uint32max)
{
	d = timelib_duration_ctor(4294967296, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_mul(d, 4294967296, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_OVERFLOW, error_code);
}

TEST(duration, divide_by_0)
{
	d = timelib_duration_ctor(2, 500000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_div(d, 0, &error_code);
	POINTERS_EQUAL(NULL, d_mod);
	LONGS_EQUAL(TIMELIB_ERROR_DIVISION_BY_ZERO, error_code);
}

TEST(duration, divide_by_1)
{
	d = timelib_duration_ctor(2, 500000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_div(d, 1, &error_code);
	LONGS_EQUAL(2, d_mod->seconds);
	LONGS_EQUAL(500000000, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negative_divide_by_1)
{
	d = timelib_duration_ctor(2, 500000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_div(d, 1, &error_code);
	LONGS_EQUAL(2, d_mod->seconds);
	LONGS_EQUAL(500000000, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, divide_small_by_2)
{
	d = timelib_duration_ctor(0, 1, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_div(d, 2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negative_divide_small_by_2)
{
	d = timelib_duration_ctor(0, 1, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_div(d, 2, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, divide_by_3)
{
	d = timelib_duration_ctor(2, 500000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_div(d, 3, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(833333333, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negative_divide_by_7)
{
	d = timelib_duration_ctor(10, 0, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_div(d, 7, &error_code);
	LONGS_EQUAL(1, d_mod->seconds);
	LONGS_EQUAL(428571428, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negate_positive)
{
	d = timelib_duration_ctor(19, 78, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_negate(d, &error_code);
	LONGS_EQUAL(19, d_mod->seconds);
	LONGS_EQUAL(78, d_mod->nanoseconds);
	LONGS_EQUAL(true, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negate_0)
{
	d = timelib_duration_ctor(0, 0, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_negate(d, &error_code);
	LONGS_EQUAL(0, d_mod->seconds);
	LONGS_EQUAL(0, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}

TEST(duration, negate_negative)
{
	d = timelib_duration_ctor(19, 78, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	d_mod = timelib_duration_negate(d, &error_code);
	LONGS_EQUAL(19, d_mod->seconds);
	LONGS_EQUAL(78, d_mod->nanoseconds);
	LONGS_EQUAL(false, d_mod->negative);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
}


TEST(duration, abs_compare_two_negative_1)
{
	d = timelib_duration_ctor(10, 900000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_two_negative_2)
{
	d = timelib_duration_ctor(10, 700000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_two_negative_3)
{
	d = timelib_duration_ctor(7, 123456789, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_two_positive_1)
{
	d = timelib_duration_ctor(10, 900000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_two_positive_2)
{
	d = timelib_duration_ctor(10, 700000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_two_positive_3)
{
	d = timelib_duration_ctor(7, 123456789, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_positive_and_negative_1)
{
	d = timelib_duration_ctor(10, 900000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_positive_and_negative_2)
{
	d = timelib_duration_ctor(10, 700000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_positive_and_negative_3)
{
	d = timelib_duration_ctor(7, 123456789, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_negative_and_positive_1)
{
	d = timelib_duration_ctor(10, 900000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_negative_and_positive_2)
{
	d = timelib_duration_ctor(10, 700000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_abs_compare(d, d_mod));
}

TEST(duration, abs_compare_negative_and_positive_3)
{
	d = timelib_duration_ctor(7, 123456789, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_abs_compare(d, d_mod));
}


TEST(duration, compare_two_negative_1)
{
	d = timelib_duration_ctor(10, 900000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_two_negative_2)
{
	d = timelib_duration_ctor(10, 700000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_two_negative_3)
{
	d = timelib_duration_ctor(7, 123456789, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_two_positive_1)
{
	d = timelib_duration_ctor(10, 900000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_two_positive_2)
{
	d = timelib_duration_ctor(10, 700000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_two_positive_3)
{
	d = timelib_duration_ctor(7, 123456789, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_positive_and_negative_1)
{
	d = timelib_duration_ctor(10, 900000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_positive_and_negative_2)
{
	d = timelib_duration_ctor(10, 700000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_positive_and_negative_3)
{
	d = timelib_duration_ctor(7, 123456789, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_negative_and_positive_1)
{
	d = timelib_duration_ctor(10, 900000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_negative_and_positive_2)
{
	d = timelib_duration_ctor(10, 700000000, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_compare(d, d_mod));
}

TEST(duration, compare_negative_and_positive_3)
{
	d = timelib_duration_ctor(7, 123456789, true, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);
	d_mod = timelib_duration_ctor(9, 800000000, false, &error_code);
	LONGS_EQUAL(TIMELIB_ERROR_NO_ERROR, error_code);

	LONGS_EQUAL(-1, timelib_duration_compare(d, d_mod));
}
