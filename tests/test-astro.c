#include "timelib.h"
#include "astro.h"
#include "math.h"

int main(int argc, char *argv[])
{
	timelib_time *t;
	timelib_sll   rise, set, transit;
	int           rs, h, m, s;
	double        h_rise, h_set;

	if (argc != 4) {
		printf("Usage: ./test-astro time longitude latitude\n       ./test-astro \"2005-10-17 00:00:00\" 9.627 59.186\n");
		return -1;
	}

	t = timelib_strtotime(argv[1] /*"2005-10-17 00:00:00"*/, strlen(argv[1]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_dump_date(t, 1);
	rs = timelib_astro_rise_set_altitude(t, atof(argv[2]) /*9.627*/, atof(argv[3]) /*59.186*/, 0, 0, &h_rise, &h_set, &rise, &set, &transit);
	
	switch (rs) {
		case 0:
			break;
		case +1:
			printf( "Sun always above horizon\n");
			break;
		case -1:
			printf( "Sun always below horizon\n");
			break;
	}
	timelib_unixtime2local(t, rise);
	timelib_dump_date(t, 1);
	timelib_decimal_hour_to_hms(h_rise, &h, &m, &s);

	timelib_unixtime2local(t, set);
	timelib_dump_date(t, 1);
	timelib_decimal_hour_to_hms(h_set, &h, &m, &s);

	if (t->tz_info) {
		timelib_tzinfo_dtor(t->tz_info);
	}
	timelib_time_dtor(t);

	return 0;
}
