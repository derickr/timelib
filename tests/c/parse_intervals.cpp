#include "CppUTest/TestHarness.h"
#include "timelib.h"
#include <stdio.h>
#include <string.h>

TEST_GROUP(parse_intervals)
{
	timelib_time     *b = NULL, *e = NULL;
	timelib_rel_time *p = NULL;
	int               r = 0;
	timelib_error_container *errors;


	void test_parse(const char *input)
	{
		if (p) {
			timelib_rel_time_dtor(p);
			timelib_error_container_dtor(errors);
		}

		char *inputCopy = strdup(input);
		timelib_strtointerval(inputCopy, strlen(inputCopy), &b, &e, &p, &r, &errors);
		free(inputCopy);
	}

	TEST_TEARDOWN()
	{
		timelib_rel_time_dtor(p);
		timelib_error_container_dtor(errors);
	}
};

TEST(parse_intervals, weeksOnly)
{
	test_parse("P2W");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(0, p->y);
	LONGS_EQUAL(0, p->m);
	LONGS_EQUAL(14, p->d);
}

TEST(parse_intervals, combiningWeeksAndDays)
{
	test_parse("P2W3D");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(0, p->y);
	LONGS_EQUAL(0, p->m);
	LONGS_EQUAL(17, p->d);

	test_parse("P1Y3M1W5DT2H");
	LONGS_EQUAL(0, errors->warning_count);
	LONGS_EQUAL(0, errors->error_count);
	LONGS_EQUAL(1, p->y);
	LONGS_EQUAL(3, p->m);
	LONGS_EQUAL(12, p->d);
	LONGS_EQUAL(2, p->h);
}
