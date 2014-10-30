#include <timelib_config.h>
#include "timelib.h"


int main(int argc, char *argv[])
{
	signed long long ts;
	timelib_time *t;
	char    *tz;
	timelib_tzinfo  *tzi;

	if (argc < 3) {
		printf("Usage:\n\ttester-render [t] [tz specification]\n\tExample: ./tester-render \"1114819200\" \"Europe/Amsterdam\"\n\n");
		exit(-1);
	}
	ts = atoll(argv[1]);
	tz = argv[2];
	tzi = timelib_parse_tzfile(tz, timelib_builtin_db());

	t = timelib_time_ctor();
	timelib_set_timezone(t, tzi);
	timelib_unixtime2local(t, ts);
	timelib_dump_date(t, 3);
	timelib_tzinfo_dtor(t->tz_info);
	timelib_time_dtor(t);

	return 0;
}
