#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

int swap_available() {
	long total = meminfo_value("SwapTotal:");
	return (total > 0);
}

static void swap_vals(float *total_gb, float *free_gb) {
	long total = meminfo_value("SwapTotal:");
	long free = meminfo_value("SwapFree:");
	*total_gb = total / MB;
	*free_gb = free / MB;
}

int swap(char *out, size_t sz, int argc, char **argv) {
	if (!swap_available()) {
		snprintf(out, sz, "You dont have a swap partition\n\n");
		return 1;
	}
	float x_h, y_h;
	swap_vals(&x_h, &y_h);
	snprintf(out, sz,
			"total memory is swap : %0.3f GB\n"
			"memory left in swap : %0.3f GB\n"
			"memory used in swap : %0.3f GB\n",
			x_h, y_h, x_h - y_h);
	return 0;
}

int swap_total(char *out, size_t sz, int argc, char **argv) {
	if (!swap_available()) {
		snprintf(out, sz, "You dont have a swap partition\n\n");
		return 1;
	}
	float x_h, y_h;
	swap_vals(&x_h, &y_h);
	snprintf(out, sz, "total memory in swap : %0.3f GB\n", x_h);
	return 0;
}

int swap_left(char *out, size_t sz, int argc, char **argv) {
	if (!swap_available()) {
		snprintf(out, sz, "You dont have a swap partition\n\n");
		return 1;
	}
	float x_h, y_h;
	swap_vals(&x_h, &y_h);
	snprintf(out, sz, "memory left in swap : %0.3f GB\n", y_h);
	return 0;
}

int swap_used(char *out, size_t sz, int argc, char **argv) {
	if (!swap_available()) {
		snprintf(out, sz, "You dont have a swap partition\n\n");
		return 1;
	}
	float x_h, y_h;
	swap_vals(&x_h, &y_h);
	snprintf(out, sz, "memory used in swap : %0.3f GB\n", x_h - y_h);
	return 0;
}

int swap_total_val(char *out, size_t sz, int argc, char **argv) {
	if (!swap_available()) {
		snprintf(out, sz, "You dont have a swap partition\n\n");
		return 1;
	}
	float x_h, y_h;
	swap_vals(&x_h, &y_h);
	snprintf(out, sz, "%0.3f GB\n", x_h);
	return 0;
}

int swap_left_val(char *out, size_t sz, int argc, char **argv) {
	if (!swap_available()) {
		snprintf(out, sz, "You dont have a swap partition\n\n");
		return 1;
	}
	float x_h, y_h;
	swap_vals(&x_h, &y_h);
	snprintf(out, sz, "%0.3f GB\n", y_h);
	return 0;
}

int swap_used_val(char *out, size_t sz, int argc, char **argv) {
	if (!swap_available()) {
		snprintf(out, sz, "You dont have a swap partition\n\n");
		return 1;
	}
	float x_h, y_h;
	swap_vals(&x_h, &y_h);
	snprintf(out, sz, "%0.3f GB\n", x_h - y_h);
	return 0;
}
