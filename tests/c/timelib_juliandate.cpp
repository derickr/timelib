#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(j2000)
{
};

TEST(j2000, J2000Epoch)
{
	double d = 946728000;
	double js = timelib_ts_to_j2000(d);

	DOUBLES_EQUAL(0, js, 0.0000001);
}

TEST(j2000, August2017)
{
	double d = 1502755200;
	double js = timelib_ts_to_j2000(d);

	DOUBLES_EQUAL(6435.5, js, 0.0000001);
}

TEST_GROUP(julian_day)
{
};

TEST(julian_day, JulianDayEpoch)
{
	double d = -210866760000;
	double js = timelib_ts_to_julianday(d);

	DOUBLES_EQUAL(0, js, 0.0000001);
}

TEST(j2000, JulianDateExampleFromWikipedia)
{
	double d = 1357000200;
	double js = timelib_ts_to_julianday(d);

	DOUBLES_EQUAL(2456293.520833, js, 0.000001);
}

TEST(julian_day, August2017)
{
	double d = 1502755200;
	double js = timelib_ts_to_julianday(d);

	DOUBLES_EQUAL(2457980.5, js, 0.0000001);
}
