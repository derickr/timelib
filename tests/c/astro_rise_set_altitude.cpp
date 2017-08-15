#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(sun_rise_set)
{
};

TEST(sun_rise_set, PHPSunInfo001)
{
	double h_rise, h_set;
	timelib_sll ts_rise, ts_set, ts_transit;
	timelib_time *t = timelib_time_ctor();

	t->y = 2006;
	t->m = 12;
	t->d = 12;
	t->h = 00;
	t->i = 00;
	t->s = 00;
	timelib_update_ts(t, NULL);

	timelib_astro_rise_set_altitude(t, 31.7667, 35.2333, -35.0/60.0, 1,
		&h_rise, &h_set, &ts_rise, &ts_set, &ts_transit);

	DOUBLES_EQUAL( 4.86, h_rise, 0.01);
	DOUBLES_EQUAL(14.69, h_set,  0.01);
	LONGS_EQUAL(1165899111, ts_rise);
	LONGS_EQUAL(1165934475, ts_set);
	LONGS_EQUAL((1165899111LL + 1165934475LL) / 2, ts_transit);

	timelib_time_dtor(t);
}

TEST(sun_rise_set, PHPSunInfo002)
{
	double h_rise, h_set;
	timelib_sll ts_rise, ts_set, ts_transit;
	timelib_time *t = timelib_time_ctor();

	t->y = 2007;
	t->m = 4;
	t->d = 13;
	t->h = 11;
	t->i = 10;
	t->s = 54;
	timelib_update_ts(t, NULL);

	timelib_astro_rise_set_altitude(t, 9.61, 59.21, -35.0/60.0, 1,
		&h_rise, &h_set, &ts_rise, &ts_set, &ts_transit);

	DOUBLES_EQUAL( 4.23, h_rise, 0.01);
	DOUBLES_EQUAL(18.51, h_set,  0.01);
	LONGS_EQUAL(1176437611, ts_rise);
	LONGS_EQUAL(1176489051, ts_set);
	LONGS_EQUAL((1176489051LL + 1176437611LL) / 2, ts_transit);

	timelib_time_dtor(t);
}
