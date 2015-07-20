#include "timelib.h"

int main(int argc, char *argv[])
{
	timelib_time *t, *now;
	char    *tz;
	timelib_tzinfo *tzi;

	if (argc < 4) {
		printf("Usage:\n\ttester-create-ts [t] [reference] [tz specification]\n\tExample: ./tester-create-ts \"9/11\" \"00:00:00\" \"Europe/Amsterdam\"\n\n");
		exit(-1);
	}

	t = timelib_strtotime(argv[1], strlen(argv[1]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	now = timelib_strtotime(argv[2], strlen(argv[2]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	tz = argv[3];
	tzi = timelib_parse_tzfile(tz, timelib_builtin_db());

	timelib_fill_holes(t, now, TIMELIB_OVERRIDE_TIME);
	timelib_time_dtor(now);
	timelib_update_ts(t, tzi);

	timelib_dump_date(t, 1);
	timelib_time_dtor(t);
	if (tzi) {
		timelib_tzinfo_dtor(tzi);
	}

	return 0;
}
