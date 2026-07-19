#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

int cpu(char *out, size_t sz, int argc, char **argv) {
	FILE *fp = fopen("/proc/cpuinfo", "r");
	if (!fp) {
		snprintf(out, sz, "fopen failed\n");
		return 1;
	}
	char buffer[256];
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "model name", 10) == 0) {
			char *x = strchr(buffer, ':');
			if (!x) continue;
			x++;
			while (*x == ' ' || *x == '\t') x++;
			size_t len = strlen(x);
			if (len > 0 && x[len - 1] == '\n') x[len - 1] = '\0';
			snprintf(out, sz, "%s\n", x);
			break;
		}
	}
	fclose(fp);
	return 0;
}

int cpucores(char *out, size_t sz, int argc, char **argv) {
	snprintf(out, sz, "%ld\n", sysconf(_SC_NPROCESSORS_ONLN));
	return 0;
}

int cpufreq(char *out, size_t sz, int argc, char **argv) {
	int cores = sysconf(_SC_NPROCESSORS_ONLN);
	size_t off = 0;
	for (int i = 0; i < cores; i++) {
		float freq = get_core_freq(i);
		if (freq > 0)
			off = out_append(out, off, sz,
					"frequency of core %d : %0.3f Mhz\n", i,
					freq);
		else
			off = out_append(out, off, sz,
					"frequency of core %d : unavailable\n",
					i);
	}
	return 0;
}

int load(char *out, size_t sz, int argc, char **argv) {
	cpu_stat prev[CORES], curr[CORES];
	int cores = sysconf(_SC_NPROCESSORS_ONLN);

	read_cpu_stats(prev, cores);
	usleep(100000);
	read_cpu_stats(curr, cores);

	size_t off = 0;
	for (int i = 0; i < cores; i++)
		off = out_append(out, off, sz, "Core %d: %.2f%%\n", i,
				cpu_usage_percent(&prev[i], &curr[i]));
	return 0;
}

int load_core(char *out, size_t sz, int argc, char **argv) {
	cpu_stat prev[CORES], curr[CORES];
	int cores = sysconf(_SC_NPROCESSORS_ONLN);

	if (argc < 4) {
		snprintf(out, sz,
				"usage : cpu load (core number)\n");
		return 1;
	}

	int core_num = atoi(argv[3]);
	if (core_num >= cores || core_num < 0) {
		snprintf(out, sz, "Invalid core number %d\n", core_num);
		return 1;
	}

	read_cpu_stats(prev, cores);
	usleep(100000);
	read_cpu_stats(curr, cores);

	snprintf(out, sz, "Core %d: %.2f%%\n", core_num,
			cpu_usage_percent(&prev[core_num], &curr[core_num]));
	return 0;
}

float get_core_freq(int core_num) {
	char path[128];
	snprintf(path, sizeof(path),
			"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq",
			core_num);

	FILE *fp = fopen(path, "r");
	if (!fp) {
		return -1.0f;
	}

	long freq_khz;
	if (fscanf(fp, "%ld", &freq_khz) != 1) {
		fclose(fp);
		return -1.0f;
	}
	fclose(fp);

	return freq_khz / 1000.0f;
}
