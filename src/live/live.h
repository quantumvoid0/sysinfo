#ifndef LIVE_H
#define LIVE_H

#include <ncurses.h>

typedef struct {
	unsigned long long user;
	unsigned long long nice;
	unsigned long long system;
	unsigned long long idle;
	unsigned long long iowait;
	unsigned long long irq;
	unsigned long long softirq;
	unsigned long long steal;
} cpu_stat;

#define CORES 256

void read_cpu_stats_live(cpu_stat stats[], int cores);
float get_total_cpu_usage();

// Boxes section

void draw_box(WINDOW *win, const char *title);
void os_box(WINDOW *win);
void kernel_box(WINDOW *win);
void hostname_box(WINDOW *win);
void uptime_box(WINDOW *win);
void cpu_box(WINDOW *win);
void cpufreq_box(WINDOW *win, int scroll_offset);
void ram_box(WINDOW *win, int scroll_offset);
void swap_box(WINDOW *win, int scroll_offset);
void root_box(WINDOW *win);
void me_box(WINDOW *win);
void proc_box(WINDOW *win, int scroll, int selected_line);

#endif
