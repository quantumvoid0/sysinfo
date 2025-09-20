#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

void cpu() {
	char buffer[256];
	FILE *fp = popen("cat /proc/cpuinfo", "r");

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "model name	: ", 13) == 0) {
			char *x = buffer + 13;
			printf("%s", x);
			break;
		};
	};
	pclose(fp);
};

void cpucores() {
	char buffer[256];
	FILE *fp = popen("nproc", "r");
	fgets(buffer, sizeof(buffer), fp);
	printf("%s", buffer);
	pclose(fp);
};

void cpufreq() {
	char buffer[256];
	FILE *fp = popen(
	    "cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq", "r");
	int i = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		char core[256];
		FILE *cr = popen("nproc", "r");
		fgets(core, sizeof(core), cr);
		float buffer_h = atol(buffer);
		buffer_h = buffer_h / 1000;
		printf("frequncy of core %d : %0.3f Mhz\n", i, buffer_h);
		i++;
	};
	pclose(fp);
};

#define CORES 256

typedef struct {
	unsigned long long user, nice, system, idle, iowait, irq, softirq,
	    steal;
} cpu_stat;

void read_cpu_stats(cpu_stat stats[], int cores) {
	FILE *fp = fopen("/proc/stat", "r");
	if (!fp) return;

	char line[256];
	int core_index = 0;

	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "cpu", 3) != 0) break;

		if (line[3] >= '0' && line[3] <= '9') {
			sscanf(
			    line,
			    "cpu%*d %llu %llu %llu %llu %llu %llu %llu %llu",
			    &stats[core_index].user, &stats[core_index].nice,
			    &stats[core_index].system, &stats[core_index].idle,
			    &stats[core_index].iowait, &stats[core_index].irq,
			    &stats[core_index].softirq,
			    &stats[core_index].steal);
			core_index++;
			if (core_index >= cores) break;
		}
	}
	fclose(fp);
}

void load() {
	cpu_stat prev[CORES], curr[CORES];
	int i, cores = sysconf(_SC_NPROCESSORS_ONLN);

	read_cpu_stats(prev, cores);
	usleep(100000);
	read_cpu_stats(curr, cores);

	for (i = 0; i < cores; i++) {
		unsigned long long prev_idle = prev[i].idle + prev[i].iowait;
		unsigned long long curr_idle = curr[i].idle + curr[i].iowait;

		unsigned long long prev_total = prev[i].user + prev[i].nice +
						prev[i].system + prev[i].idle +
						prev[i].iowait + prev[i].irq +
						prev[i].softirq + prev[i].steal;
		unsigned long long curr_total = curr[i].user + curr[i].nice +
						curr[i].system + curr[i].idle +
						curr[i].iowait + curr[i].irq +
						curr[i].softirq + curr[i].steal;

		unsigned long long total_diff = curr_total - prev_total;
		unsigned long long idle_diff = curr_idle - prev_idle;

		double usage =
		    (double)(total_diff - idle_diff) / total_diff * 100.0;
		printf("Core %d: %.2f%%\n", i, usage);
	}
}

void load_core(int core_num) {
	cpu_stat prev[CORES], curr[CORES];
	int i, cores = sysconf(_SC_NPROCESSORS_ONLN);

	if (core_num >= cores || core_num < 0) {
		printf("Invalid core number %d\n", core_num);
		return;
	}

	read_cpu_stats(prev, cores);
	usleep(100000);
	read_cpu_stats(curr, cores);

	unsigned long long prev_idle =
	    prev[core_num].idle + prev[core_num].iowait;
	unsigned long long curr_idle =
	    curr[core_num].idle + curr[core_num].iowait;

	unsigned long long prev_total =
	    prev[core_num].user + prev[core_num].nice + prev[core_num].system +
	    prev[core_num].idle + prev[core_num].iowait + prev[core_num].irq +
	    prev[core_num].softirq + prev[core_num].steal;

	unsigned long long curr_total =
	    curr[core_num].user + curr[core_num].nice + curr[core_num].system +
	    curr[core_num].idle + curr[core_num].iowait + curr[core_num].irq +
	    curr[core_num].softirq + curr[core_num].steal;

	unsigned long long total_diff = curr_total - prev_total;
	unsigned long long idle_diff = curr_idle - prev_idle;

	double usage = (double)(total_diff - idle_diff) / total_diff * 100.0;
	printf("Core %d: %.2f%%\n", core_num, usage);
}

float get_core_freq(int core_num) {
	char path[128];
	snprintf(path, sizeof(path),
		 "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq",
		 core_num);

	FILE *fp = fopen(path, "r");
	if (!fp) {
		perror("fopen");
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
