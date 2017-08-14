#include "CppUTest/TestHarness.h"
#include "timelib.h"

TEST_GROUP(issues)
{
};

TEST(issues, issue0017_test1)
{
	timelib_sll   ts = 12622608000;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(2369, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(30, t->d);
	
	timelib_time_dtor(t);
}

TEST(issues, issue0017_test2)
{
	timelib_sll   ts = 12622694400;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(2369, t->y);
	LONGS_EQUAL(12, t->m);
	LONGS_EQUAL(31, t->d);
	
	timelib_time_dtor(t);
}

TEST(issues, issue0017_test3)
{
	timelib_sll   ts = 12622780800;
	timelib_time *t  = timelib_time_ctor();

	timelib_set_timezone_from_offset(t, 0);
	timelib_unixtime2local(t, ts);
	LONGS_EQUAL(2370, t->y);
	LONGS_EQUAL(1, t->m);
	LONGS_EQUAL(1, t->d);
	
	timelib_time_dtor(t);
}
