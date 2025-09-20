#include <stdio.h>
#include <time.h>

#include "functions.h"

void show_date(int format) {
	time_t rawtime;
	struct tm *t;

	time(&rawtime);
	t = localtime(&rawtime);

	if (format == 1) {
		// DD-MM-YYYY
		printf("%02d-%02d-%04d\n", t->tm_mday, t->tm_mon + 1,
		       t->tm_year + 1900);
	} else if (format == 2) {
		// MM-DD-YYYY
		printf("%02d-%02d-%04d\n", t->tm_mon + 1, t->tm_mday,
		       t->tm_year + 1900);
	} else if (format == 3) {
		// YYYY-MM-DD (ISO)
		printf("%04d-%02d-%02d\n", t->tm_year + 1900, t->tm_mon + 1,
		       t->tm_mday);
	} else {
		printf("Invalid format (use 1, 2, or 3)\n");
	}
}

