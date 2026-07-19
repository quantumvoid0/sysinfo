#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
	unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} cpu_stat;

#define CORES 256

#define MB 1048576.0f

#define OUT_SZ 32768

static inline size_t out_append(char *out, size_t off, size_t sz,
		const char *fmt, ...) {
	if (off >= sz)
		return off;
	va_list ap;
	va_start(ap, fmt);
	int n = vsnprintf(out + off, sz - off, fmt, ap);
	va_end(ap);
	if (n > 0)
		off += (size_t)n;
	return off;
}

#define CMD_SIG char *out, size_t sz, int argc, char **argv
typedef int (*cmd_fn)(CMD_SIG);

int cpu(CMD_SIG);
int cpucores(CMD_SIG);
int cpufreq(CMD_SIG);
int load(CMD_SIG);
int load_core(CMD_SIG);
float get_core_freq(int core_num);

int ram(CMD_SIG);
int ram_used(CMD_SIG);
int ram_total(CMD_SIG);
int ram_left(CMD_SIG);
int ram_used_val(CMD_SIG);
int ram_total_val(CMD_SIG);
int ram_left_val(CMD_SIG);

int swap(CMD_SIG);
int swap_total(CMD_SIG);
int swap_used(CMD_SIG);
int swap_left(CMD_SIG);
int swap_total_val(CMD_SIG);
int swap_used_val(CMD_SIG);
int swap_left_val(CMD_SIG);
int swap_available();

int os(CMD_SIG);
int kernel(CMD_SIG);
int uptime(CMD_SIG);
int root(CMD_SIG);
int me(CMD_SIG);
int hostname(CMD_SIG);

int show_time(CMD_SIG);
int show_date(CMD_SIG);

int drives(CMD_SIG);

int editor(CMD_SIG);
int locale(CMD_SIG);
int language(CMD_SIG);
int region(CMD_SIG);
int encoding(CMD_SIG);
int shell(CMD_SIG);
int term(CMD_SIG);
int bios(CMD_SIG);
int system_info(CMD_SIG);
int board(CMD_SIG);
int init(CMD_SIG);
int proc(CMD_SIG);
int ping(CMD_SIG);
int ip(CMD_SIG);

int help();
int livemode();
int fetch(int argc, char **argv);

long meminfo_value(const char *key);
int read_dmi(const char *file, char *out, size_t outsz);
int cpu_model_name(char *out, size_t outsz);
void read_cpu_stats(cpu_stat stats[], int cores);
double cpu_usage_percent(const cpu_stat *prev, const cpu_stat *curr);

int dispatch_command(int argc, char **argv, char *out, size_t sz);


#endif
