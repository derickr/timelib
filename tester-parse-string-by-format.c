#include <timelib_config.h>
#include "timelib.h"

int main(int argc, char *argv[])
{
	timelib_time *t;
	int           i;
	struct timelib_error_container *errors;

	t = timelib_parse_from_format(argv[1], argv[2], strlen(argv[2]), &errors, timelib_builtin_db(), timelib_parse_tzfile);
	if (errors->warning_count) {
		printf("W=%d ", errors->warning_count);
	}
	if (errors->error_count) {
		printf("E=%d ", errors->error_count);
	}
	timelib_dump_date(t, 1);
	timelib_time_dtor(t);

	if (errors->warning_count) {
		printf("Warnings found while parsing '%s'\n", argv[1]);
		for (i = 0; i < errors->warning_count; i++) {
			printf("W %s @ pos %d (char=[%c])\n", errors->warning_messages[i].message, errors->warning_messages[i].position, errors->warning_messages[i].character);
		}
	}
	if (errors->error_count) {
		printf("Errors found while parsing '%s'\n", argv[1]);
		for (i = 0; i < errors->error_count; i++) {
			printf("E %s @ pos %d (char=[%c])\n", errors->error_messages[i].message, errors->error_messages[i].position, errors->error_messages[i].character);
		}
	}

	timelib_error_container_dtor(errors);
	return errors->error_count ? 1 : 0;
}
