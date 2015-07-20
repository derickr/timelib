#include "timelib.h"

int main(int argc, char *argv[])
{
	timelib_sll y, m, d;
	timelib_sll week, year;

	y = atoi(argv[1]);
	m = atoi(argv[2]);
	d = atoi(argv[3]);

	timelib_isoweek_from_date(y, m, d, &week, &year);

	printf("%llu-%llu\n", week, year);

	return 0;
}
