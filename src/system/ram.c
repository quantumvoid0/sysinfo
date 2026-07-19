#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

static void ram_vals(float *total_gb, float *avail_gb) {
	long total = meminfo_value("MemTotal:");
	long avail = meminfo_value("MemAvailable:");
	*total_gb = total / MB;
	*avail_gb = avail / MB;
}

int ram(char *out, size_t sz, int argc, char **argv) {
	float x_h, y_h;
	ram_vals(&x_h, &y_h);
	snprintf(out, sz,
			"total memory in ram : %0.3f GB\n"
			"memory left in ram : %0.3f GB\n"
			"memory used in ram : %0.3f GB\n",
			x_h, y_h, x_h - y_h);
	return 0;
}

int ram_total(char *out, size_t sz, int argc, char **argv) {
	float x_h, y_h;
	ram_vals(&x_h, &y_h);
	snprintf(out, sz, "total memory in ram : %0.3f GB\n", x_h);
	return 0;
}

int ram_total_val(char *out, size_t sz, int argc, char **argv) {
	float x_h, y_h;
	ram_vals(&x_h, &y_h);
	snprintf(out, sz, "%0.3f GB\n", x_h);
	return 0;
}

int ram_left(char *out, size_t sz, int argc, char **argv) {
	float x_h, y_h;
	ram_vals(&x_h, &y_h);
	snprintf(out, sz, "memory left in ram : %0.3f GB\n", y_h);
	return 0;
}

int ram_left_val(char *out, size_t sz, int argc, char **argv) {
	float x_h, y_h;
	ram_vals(&x_h, &y_h);
	snprintf(out, sz, "%0.3f GB\n", y_h);
	return 0;
}

int ram_used(char *out, size_t sz, int argc, char **argv) {
	float x_h, y_h;
	ram_vals(&x_h, &y_h);
	snprintf(out, sz, "memory used in ram : %0.3f GB\n", x_h - y_h);
	return 0;
}

int ram_used_val(char *out, size_t sz, int argc, char **argv) {
	float x_h, y_h;
	ram_vals(&x_h, &y_h);
	snprintf(out, sz, "%0.3f GB\n", x_h - y_h);
	return 0;
}
