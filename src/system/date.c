#include <stdio.h>
#include <string.h>
#include <time.h>

#include "functions.h"

int show_date(char *out, size_t sz, int argc, char **argv) {
	time_t rawtime;
	struct tm *t;

	time(&rawtime);
	t = localtime(&rawtime);

	int format = 1;	 
	if (argc >= 3) {
		if (strcmp(argv[2], "1") == 0)
			format = 1;
		else if (strcmp(argv[2], "2") == 0)
			format = 2;
		else if (strcmp(argv[2], "3") == 0)
			format = 3;
		else {
			snprintf(out, sz, "Usage: %s date [1|2|3]\n",
					argv[0]);
			return 1;
		}
	}

	if (format == 1)
		snprintf(out, sz, "%02d-%02d-%04d\n", t->tm_mday,
				t->tm_mon + 1, t->tm_year + 1900);
	else if (format == 2)
		snprintf(out, sz, "%02d-%02d-%04d\n", t->tm_mon + 1,
				t->tm_mday, t->tm_year + 1900);
	else
		snprintf(out, sz, "%04d-%02d-%02d\n", t->tm_year + 1900,
				t->tm_mon + 1, t->tm_mday);
	return 0;
}
