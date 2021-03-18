#include "CppUTest/CommandLineTestRunner.h"
#include "timelib.h"

timelib_tzdb *zoneinfo;

int main(int ac, char **av)
{
	int return_value;

	zoneinfo = timelib_zoneinfo("/usr/share/zoneinfo");
	return_value = CommandLineTestRunner::RunAllTests(ac, av);
	timelib_zoneinfo_dtor(zoneinfo);

	return return_value;
}
