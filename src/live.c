#include "live/live.h"
#include "live/live_data.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int livemode(void) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	int colors = has_colors();
	if (colors) {
		start_color();
		init_pair(1, COLOR_CYAN, COLOR_BLACK);
		init_pair(2, COLOR_WHITE, COLOR_BLUE);
		init_pair(3, COLOR_GREEN, COLOR_BLACK);
		init_pair(4, COLOR_YELLOW, COLOR_BLACK);
		init_pair(5, COLOR_RED, COLOR_BLACK);
		init_pair(6, COLOR_BLACK, COLOR_WHITE);
		init_pair(7, COLOR_YELLOW, COLOR_BLACK);
	}

	int *graph = NULL, graph_w = 0, graph_h = 0, graph_pos = 0;
	int cpufreq_scroll = 0, ram_scroll = 0, swap_scroll = 0;
	int proc_scroll = 0, proc_selected = 0;

	enum section { SEL_CPU, SEL_RAM, SEL_SWAP, SEL_PROC } selected = SEL_CPU;

	while (1) {
		int h, w;
		getmaxyx(stdscr, h, w);

		int top_h = 1;
		int cpu_h = 5;
		int mem_h = 5;
		int info_h = 5;
		int content_h = top_h + cpu_h + mem_h + info_h;
		int bottom_h = h - content_h - 1;
		if (bottom_h < 4) bottom_h = 4;

		int half_w = w / 2;
		int third_w = w / 3;
		int bat_w = w - 2 * third_w;
		int graph_w_part = (w * 7) / 10;
		int proc_w_part = w - graph_w_part;

		WINDOW *header_win = newwin(top_h, w, 0, 0);

		int y = top_h;
		WINDOW *cpu_win = newwin(cpu_h, half_w, y, 0);
		WINDOW *freq_win = newwin(cpu_h, half_w, y, half_w);

		y += cpu_h;
		WINDOW *ram_win = newwin(mem_h, half_w, y, 0);
		WINDOW *swap_win = newwin(mem_h, half_w, y, half_w);

		y += mem_h;
		WINDOW *me_win = newwin(info_h, third_w, y, 0);
		WINDOW *root_win = newwin(info_h, third_w, y, third_w);
		WINDOW *bat_win = newwin(info_h, bat_w, y, 2 * third_w);

		y += info_h;
		WINDOW *graph_win = newwin(bottom_h, graph_w_part, y, 0);
		WINDOW *proc_win = newwin(bottom_h, proc_w_part, y, graph_w_part);

		WINDOW *panel_wins[] = {cpu_win, freq_win, ram_win, swap_win,
					me_win, root_win, bat_win, proc_win};
		const char *panel_titles[] = {"CPU", "CPU FREQUENCY", "RAM", "SWAP",
						"USER", "ROOT", "SYSTEM BATTERY",
						"PROCESSES"};
		int num_panels = 8;

		if (!graph || graph_w != graph_w_part || graph_h != bottom_h) {
			if (graph) free(graph);
			graph_w = graph_w_part;
			graph_h = bottom_h;
			graph = calloc(graph_w, sizeof(int));
			graph_pos = 0;
		}

		{
			char os_str[LIVE_LINE_SZ], kernel_str[LIVE_LINE_SZ];
			char uptime_str[LIVE_LINE_SZ], host_str[LIVE_LINE_SZ],
				bat_str[LIVE_LINE_SZ];
			live_os(os_str, sizeof(os_str));
			live_kernel(kernel_str, sizeof(kernel_str));
			live_uptime(uptime_str, sizeof(uptime_str));
			live_hostname(host_str, sizeof(host_str));
			live_battery(bat_str, sizeof(bat_str));

			if (colors) wattron(header_win, COLOR_PAIR(2));
			whline(header_win, ' ', w);
			char line[512];
			snprintf(line, sizeof(line), " %s | %s | %s | %s | %s",
					os_str, kernel_str, uptime_str, host_str,
					bat_str);
			line[w - 1] = '\0';
			mvwprintw(header_win, 0, 0, "%s", line);
			if (colors) wattroff(header_win, COLOR_PAIR(2));
		}

		for (int i = 0; i < num_panels; i++) {
			enum section sel = -1;
			if (panel_wins[i] == cpu_win || panel_wins[i] == freq_win)
				sel = SEL_CPU;
			else if (panel_wins[i] == ram_win)
				sel = SEL_RAM;
			else if (panel_wins[i] == swap_win)
				sel = SEL_SWAP;
			else if (panel_wins[i] == proc_win)
				sel = SEL_PROC;

			WINDOW *win = panel_wins[i];
			if (colors) {
				if (sel == selected)
					wattron(win, COLOR_PAIR(7));
				else
					wattron(win, COLOR_PAIR(1));
			}
			draw_box(win, panel_titles[i]);
			if (colors) {
				if (sel == selected)
					wattroff(win, COLOR_PAIR(7));
				else
					wattroff(win, COLOR_PAIR(1));
			}
		}

		cpu_box(cpu_win);
		cpufreq_box(freq_win, cpufreq_scroll);
		ram_box(ram_win, ram_scroll);
		swap_box(swap_win, swap_scroll);
		me_box(me_win);
		root_box(root_win);
		battery_box(bat_win);

		{
			float usage = live_total_cpu_usage();
			int bar = (int)(usage * (graph_h - 2) / 100.0);
			graph[graph_pos] = bar;
			graph_pos = (graph_pos + 1) % graph_w;

			werase(graph_win);
			box(graph_win, 0, 0);
			for (int x = 0; x < graph_w; x++) {
				int idx = (graph_pos + x) % graph_w;
				for (int by = 0; by < graph[idx]; by++)
					mvwaddch(graph_win, graph_h - 2 - by,
							x + 1, '#');
			}
			mvwprintw(graph_win, 0, 2, "CPU LOAD | Usage: %.1f%%",
					usage);
		}

		proc_box(proc_win, proc_scroll, proc_selected);

		{
			char status_text[128];
			const char *sel_text = (selected == SEL_CPU)   ? "CPU"
				: (selected == SEL_RAM)  ? "RAM"
				: (selected == SEL_SWAP) ? "SWAP"
				: "PROCESSES";
			snprintf(status_text, sizeof(status_text),
					"Selected: %s  |  LEFT / RIGHT switch  "
					"UP / DOWN scroll  Q quit",
					sel_text);

			if (colors) wattron(stdscr, COLOR_PAIR(6));
			mvhline(h - 1, 0, ' ', w);
			mvwprintw(stdscr, h - 1, 1, "%s", status_text);
			if (colors) wattroff(stdscr, COLOR_PAIR(6));
		}

		wrefresh(header_win);
		for (int i = 0; i < num_panels; i++) wrefresh(panel_wins[i]);
		wrefresh(graph_win);
		wrefresh(proc_win);

		delwin(header_win);
		for (int i = 0; i < num_panels; i++) delwin(panel_wins[i]);
		delwin(graph_win);
		delwin(proc_win);

		int ch = getch();
		if (ch == 'q')
			break;
		else if (ch == KEY_LEFT) {
			if (selected == SEL_CPU)
				selected = SEL_PROC;
			else
				selected = (selected - 1);
		} else if (ch == KEY_RIGHT) {
			if (selected == SEL_PROC)
				selected = SEL_CPU;
			else
				selected = (selected + 1);
		} else if (ch == KEY_UP) {
			if (selected == SEL_CPU) {
				if (cpufreq_scroll > 0) cpufreq_scroll--;
			} else if (selected == SEL_RAM) {
				if (ram_scroll > 0) ram_scroll--;
			} else if (selected == SEL_SWAP) {
				if (swap_scroll > 0) swap_scroll--;
			} else if (selected == SEL_PROC) {
				if (proc_selected > 0) proc_selected--;
				if (proc_selected < proc_scroll)
					proc_scroll = proc_selected;
			}
		} else if (ch == KEY_DOWN) {
			if (selected == SEL_CPU) {
				int cores = live_cpufreq_count();
				int vis = cpu_h - 2;
				if (vis < 1) vis = 1;
				if (cpufreq_scroll < cores - vis)
					cpufreq_scroll++;
			} else if (selected == SEL_RAM) {
				char lines[LIVE_MAX_LINES][LIVE_LINE_SZ];
				int n = 0;
				live_ram(lines, &n);
				int vis = mem_h - 2;
				if (vis < 1) vis = 1;
				if (ram_scroll < n - vis) ram_scroll++;
			} else if (selected == SEL_SWAP) {
				char lines[LIVE_MAX_LINES][LIVE_LINE_SZ];
				int n = 0;
				live_swap(lines, &n);
				int vis = mem_h - 2;
				if (vis < 1) vis = 1;
				if (swap_scroll < n - vis) swap_scroll++;
			} else if (selected == SEL_PROC) {
				proc_selected++;
				int visible_proc = bottom_h - 2;
				if (proc_selected >= proc_scroll + visible_proc)
					proc_scroll =
						proc_selected - visible_proc + 1;
			}
		}

		usleep(200000);
	}

	endwin();
	if (graph) free(graph);
	return 0;
}
