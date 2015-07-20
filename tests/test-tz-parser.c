#include "timelib.h"

int main(int argc, char *argv[])
{
	signed long long ts;
	timelib_time *local_time;
	timelib_tzinfo *tz;

	ts = 368237; /*Monday 1970-01-01 17:17:17 */

	tz = timelib_parse_tzfile(argv[1], timelib_builtin_db());
	timelib_dump_tzinfo(tz);

	local_time = timelib_time_ctor();
	timelib_set_timezone(local_time, tz);
	timelib_unixtime2gmt(local_time, ts);
	timelib_dump_date(local_time, 1);
	timelib_unixtime2local(local_time, ts);
	timelib_dump_date(local_time, 1);

	timelib_update_ts(local_time, tz);

	timelib_tzinfo_dtor(local_time->tz_info);
	timelib_time_dtor(local_time);

	return 0;
}
