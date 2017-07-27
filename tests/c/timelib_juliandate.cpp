#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(julian_date)
{
};

TEST(julian_date, Epoch)
{
	double d = 946728000;
	double js = timelib_ts_to_juliandate(d);

	DOUBLES_EQUAL(0, js, 0.0000001);
}
