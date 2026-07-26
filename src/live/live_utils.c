#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "live.h"
#include "live_data.h"

void draw_box(WINDOW *win, const char *title) {
	werase(win);
	box(win, 0, 0);
	mvwprintw(win, 0, 2, "%s", title);
}

void os_box(WINDOW *win) {
	char buf[LIVE_LINE_SZ];
	live_os(buf, sizeof(buf));
	mvwprintw(win, 1, 2, "%s", buf);
}

void kernel_box(WINDOW *win) {
	char buf[LIVE_LINE_SZ];
	live_kernel(buf, sizeof(buf));
	mvwprintw(win, 1, 2, "%s", buf);
}

void hostname_box(WINDOW *win) {
	char buf[LIVE_LINE_SZ];
	live_hostname(buf, sizeof(buf));
	mvwprintw(win, 1, 2, "%s", buf);
}

void uptime_box(WINDOW *win) {
	char buf[LIVE_LINE_SZ];
	live_uptime(buf, sizeof(buf));
	mvwprintw(win, 1, 2, "%s", buf);
}

void cpu_box(WINDOW *win) {
	char buf[LIVE_LINE_SZ];
	live_cpu_model(buf, sizeof(buf));
	mvwprintw(win, 1, 2, "%s", buf);
}

void cpufreq_box(WINDOW *win, int scroll_offset) {
	int max_rows, max_cols;
	getmaxyx(win, max_rows, max_cols);
	(void)max_cols;

	int cores = live_cpufreq_count();
	int row = 1;
	int i = 0;
	for (int c = 0; c < cores; c++) {
		char line[LIVE_LINE_SZ];
		live_cpufreq_core(line, sizeof(line), c);
		i++;
		if (i <= scroll_offset) continue;
		if (row >= max_rows - 1) break;
		mvwprintw(win, row++, 2, "%s", line);
	}
}

void ram_box(WINDOW *win, int scroll_offset) {
	char lines[LIVE_MAX_LINES][LIVE_LINE_SZ];
	int n = 0;
	live_ram(lines, &n);

	int max_rows, max_cols;
	getmaxyx(win, max_rows, max_cols);
	(void)max_cols;
	int row = 1;
	for (int i = scroll_offset; i < n && row < max_rows - 1; i++)
		mvwprintw(win, row++, 2, "%s", lines[i]);
}

void swap_box(WINDOW *win, int scroll_offset) {
	char lines[LIVE_MAX_LINES][LIVE_LINE_SZ];
	int n = 0;
	live_swap(lines, &n);

	int max_rows, max_cols;
	getmaxyx(win, max_rows, max_cols);
	(void)max_cols;
	int row = 1;
	for (int i = scroll_offset; i < n && row < max_rows - 1; i++)
		mvwprintw(win, row++, 2, "%s", lines[i]);
}

void root_box(WINDOW *win) {
	char lines[LIVE_MAX_LINES][LIVE_LINE_SZ];
	int n = 0;
	live_root(lines, &n);

	int row = 1;
	for (int i = 0; i < n; i++)
		mvwprintw(win, row++, 2, "%s", lines[i]);
}

void me_box(WINDOW *win) {
	char buf[LIVE_LINE_SZ];
	live_me(buf, sizeof(buf));
	mvwprintw(win, 1, 2, "%s", buf);
}

void battery_box(WINDOW *win) {
	char lines[LIVE_MAX_LINES][LIVE_LINE_SZ];
	int n = 0;
	live_battery_full(lines, &n);

	int max_rows, max_cols;
	getmaxyx(win, max_rows, max_cols);
	(void)max_cols;
	int row = 1;
	for (int i = 0; i < n && row < max_rows - 1; i++)
		mvwprintw(win, row++, 2, "%s", lines[i]);
}

void proc_box(WINDOW *win, int scroll, int selected_line) {
	int maxy, maxx;
	getmaxyx(win, maxy, maxx);

	DIR *proc = opendir("/proc");
	if (!proc) {
		mvwprintw(win, 1, 1, "Failed to open /proc");
		return;
	}

	int row = 1;
	int index = 0;
	struct dirent *de;
	char buf[256];
	while ((de = readdir(proc)) != NULL) {
		if (de->d_type != DT_DIR) continue;
		int pid = atoi(de->d_name);
		if (pid == 0) continue;

		char path[32];
		snprintf(path, sizeof(path), "/proc/%d/comm", pid);
		FILE *fp = fopen(path, "r");
		if (!fp) continue;
		char comm[256];
		if (!fgets(comm, sizeof(comm), fp)) {
			fclose(fp);
			continue;
		}
		fclose(fp);
		size_t len = strlen(comm);
		if (len > 0 && comm[len - 1] == '\n') comm[len - 1] = 0;

		if (index++ < scroll) continue;
		if (row >= maxy - 1) break;

		live_proc_line(buf, sizeof(buf), pid, comm);
		buf[maxx - 2] = '\0';

		if (scroll + row - 1 == selected_line) {
			wattron(win, A_REVERSE);
			mvwprintw(win, row, 1, "%s", buf);
			wattroff(win, A_REVERSE);
		} else {
			mvwprintw(win, row, 1, "%s", buf);
		}
		row++;
	}
	closedir(proc);
}
