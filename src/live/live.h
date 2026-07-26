#ifndef LIVE_H
#define LIVE_H

#include <ncurses.h>

#include "functions.h"

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
void battery_box(WINDOW *win);

#endif
