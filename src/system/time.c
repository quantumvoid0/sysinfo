#include <stdio.h>
#include <string.h>
#include <time.h>

#include "functions.h"

int show_time(char *out, size_t sz, int argc, char **argv) {
	time_t rawtime;
	struct tm *t;

	time(&rawtime);
	t = localtime(&rawtime);

	int mode = 24;
	if (argc >= 3) {
		if (strcmp(argv[2], "12") == 0)
			mode = 12;
		else if (strcmp(argv[2], "24") == 0)
			mode = 24;
		else {
			snprintf(out, sz, "Usage: %s time [12|24]\n",
					argv[0]);
			return 1;
		}
	}

	if (mode == 12) {
		int hour = t->tm_hour % 12;
		if (hour == 0) hour = 12;
		snprintf(out, sz, "%02d:%02d:%02d %s\n", hour, t->tm_min,
				t->tm_sec, (t->tm_hour < 12) ? "AM" : "PM");
	} else {
		snprintf(out, sz, "%02d:%02d:%02d\n", t->tm_hour, t->tm_min,
				t->tm_sec);
	}
	return 0;
}
