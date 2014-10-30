#include <timelib_config.h>
#include "timelib.h"

int main(int argc, char *argv[])
{
	timelib_time     *b = NULL, *e = NULL;
	timelib_rel_time *p = NULL;
	int               r = 0;
	int           i;
	struct timelib_error_container *errors;

	timelib_strtointerval(argv[1], strlen(argv[1]), &b, &e, &p, &r, &errors);
	if (errors->warning_count) {
		printf("W=%d ", errors->warning_count);
	}
	if (errors->error_count) {
		printf("E=%d ", errors->error_count);
	}
	printf("\n");
	if (b) {
		printf("B ");
		timelib_dump_date(b, 1);
		timelib_time_dtor(b);
	}
	if (e) {
		printf("E ");
		timelib_dump_date(e, 1);
		timelib_time_dtor(e);
	}
	if (p) {
		printf("P ");
		timelib_dump_rel_time(p);
		timelib_rel_time_dtor(p);
	}
	if (r) {
		printf("Recurrences: %d\n", r);
	}

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
