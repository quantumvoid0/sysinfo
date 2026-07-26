#ifndef LIVE_DATA_H
#define LIVE_DATA_H

#include <stddef.h>

#include "functions.h"

void live_os(char *out, size_t sz);
void live_kernel(char *out, size_t sz);
void live_hostname(char *out, size_t sz);
void live_uptime(char *out, size_t sz);
void live_cpu_model(char *out, size_t sz);
void live_me(char *out, size_t sz);

#define LIVE_LINE_SZ 128
#define LIVE_MAX_LINES 16
void live_ram(char lines[][LIVE_LINE_SZ], int *n);
void live_swap(char lines[][LIVE_LINE_SZ], int *n);
void live_root(char lines[][LIVE_LINE_SZ], int *n);

int live_cpufreq_count(void);
void live_cpufreq_core(char *out, size_t sz, int core);

float live_total_cpu_usage(void);

void live_proc_line(char *out, size_t sz, int pid, const char *comm);

void live_battery(char *out, size_t sz);
void live_battery_full(char lines[][LIVE_LINE_SZ], int *n);

#endif
