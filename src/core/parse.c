#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

long meminfo_value(const char *key) {
	FILE *fp = fopen("/proc/meminfo", "r");
	if (!fp) return -1;
	char buffer[256];
	size_t klen = strlen(key);
	long val = -1;
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, key, klen) == 0) {
			char *x = strchr(buffer, ':');
			if (!x) continue;
			x++;
			while (*x == ' ' || *x == '\t') x++;
			val = atol(x);
			break;
		}
	}
	fclose(fp);
	return val;
}

int read_dmi(const char *file, char *out, size_t outsz) {
	char path[256];
	snprintf(path, sizeof(path), "/sys/devices/virtual/dmi/id/%s", file);
	FILE *fp = fopen(path, "r");
	if (!fp) return 0;
	if (!fgets(out, outsz, fp)) {
		fclose(fp);
		return 0;
	}
	size_t len = strlen(out);
	if (len > 0 && out[len - 1] == '\n') out[len - 1] = '\0';
	fclose(fp);
	return 1;
}

int cpu_model_name(char *out, size_t outsz) {
	FILE *fp = fopen("/proc/cpuinfo", "r");
	if (!fp) return 0;
	char buffer[256];
	int found = 0;
	while (fgets(buffer, sizeof(buffer), fp)) {
		if (strncmp(buffer, "model name", 10) == 0) {
			char *x = strchr(buffer, ':');
			if (!x) continue;
			x++;
			while (*x == ' ' || *x == '\t') x++;
			size_t len = strlen(x);
			if (len > 0 && x[len - 1] == '\n') x[len - 1] = '\0';
			snprintf(out, outsz, "%s", x);
			found = 1;
			break;
		}
	}
	fclose(fp);
	return found;
}

void read_cpu_stats(cpu_stat stats[], int cores) {
	FILE *fp = fopen("/proc/stat", "r");
	if (!fp) return;

	char line[256];
	int idx = 0;

	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "cpu", 3) != 0) break;
		if (line[3] >= '0' && line[3] <= '9') {
			sscanf(line,
					"cpu%*d %llu %llu %llu %llu %llu %llu %llu %llu",
					&stats[idx].user, &stats[idx].nice,
					&stats[idx].system, &stats[idx].idle,
					&stats[idx].iowait, &stats[idx].irq,
					&stats[idx].softirq, &stats[idx].steal);
			idx++;
			if (idx >= cores) break;
		}
	}
	fclose(fp);
}

double cpu_usage_percent(const cpu_stat *prev, const cpu_stat *curr) {
	unsigned long long prev_idle = prev->idle + prev->iowait;
	unsigned long long curr_idle = curr->idle + curr->iowait;

	unsigned long long prev_total = prev->user + prev->nice +
		prev->system + prev->idle + prev->iowait +
		prev->irq + prev->softirq + prev->steal;
	unsigned long long curr_total = curr->user + curr->nice +
		curr->system + curr->idle + curr->iowait +
		curr->irq + curr->softirq + curr->steal;

	unsigned long long total_diff = curr_total - prev_total;
	if (total_diff == 0) return 0.0;

	unsigned long long idle_diff = curr_idle - prev_idle;
	return (double)(total_diff - idle_diff) / total_diff * 100.0;
}
