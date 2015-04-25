#include "timelib.h"

int main(int argc, char *argv[])
{
	timelib_time *t1, *t2, *now;
	timelib_rel_time *rt;

	if (argc < 3) {
		printf("Usage:\n\ttester-diff [t1] [t2]\n\tExample: ./tester-create-ts \"9/11\" \"2008-03-26\"\n\n");
		exit(-1);
	}

	t1 = timelib_strtotime(argv[1], strlen(argv[1]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	t2 = timelib_strtotime(argv[2], strlen(argv[2]), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	now = timelib_strtotime("1970-01-01 00:00:00 UTC", strlen("1970-01-01 00:00:00 UTC"), NULL, timelib_builtin_db(), timelib_parse_tzfile);
	timelib_fill_holes(t1, now, TIMELIB_OVERRIDE_TIME);
	timelib_fill_holes(t2, now, TIMELIB_OVERRIDE_TIME);

	timelib_update_ts(t1, t1->tz_info);
	timelib_update_ts(t2, t2->tz_info);

	timelib_dump_date(t1, 1);
	timelib_dump_date(t2, 1);
	rt = timelib_diff(t1, t2);

	timelib_dump_rel_time(rt);

	timelib_time_dtor(t1);
	timelib_time_dtor(t2);
	timelib_time_dtor(now);
	timelib_rel_time_dtor(rt);

	return 0;
}
