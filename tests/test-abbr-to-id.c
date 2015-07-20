#include "timelib.h"


int main(void)
{
	char *id;

	id = timelib_timezone_id_from_abbr("cest", 10800, 1);  printf("%s\n", id);
	id = timelib_timezone_id_from_abbr("cest", 7200, 1);   printf("%s\n", id);
	id = timelib_timezone_id_from_abbr("cest", 7200, 0);   printf("%s\n", id);
	id = timelib_timezone_id_from_abbr("foobar", 7200, 0); printf("%s\n", id);
	id = timelib_timezone_id_from_abbr("foobar", -7 * 3600, 0); printf("%s\n", id);
	id = timelib_timezone_id_from_abbr("foobar", -5 * 3600, 1); printf("%s\n", id);
	id = timelib_timezone_id_from_abbr("foobar", 7201, 1); printf("%s\n", id);

	return 0;
}
