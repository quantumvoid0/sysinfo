#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {
	unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} cpu_stat;

void swap();
void swap_total();
void swap_used();
void swap_left();
void swap_total_val();
void swap_used_val();
void swap_left_val();

int drives();
void ram();
void ram_used();
void ram_total();
void ram_left();
void ram_used_val();
void ram_total_val();
void ram_left_val();

void cpu();
void cpucores();
void cpufreq();
void load();
void load_core(int core_num);
void read_cpu_stats(cpu_stat stats[], int cores);
float get_core_freq(int core_num);

void os();
void kernel();
void uptime();
void root();
void me();
void hostname();

void show_time(int mode);
void show_date(int format);

int help();
int livemode();
int fetch(int argc, char **argv);

#endif
