#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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
void load_core();
void read_cpu_stats();
float get_core_freq(int core_num);

void os();
void kernel();
void uptime();
void root();
void me();
void hostname();

void show_time();
void show_date();

int help();
int livemode();
int fetch();

#endif

