#include "live/live.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int livemode() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	int *graph = NULL, graph_w = 0, graph_h = 0, graph_pos = 0;
	int cpufreq_scroll = 0, ram_scroll = 0, swap_scroll = 0;
	int proc_scroll = 0, proc_selected = 0;

	int total_cores = sysconf(_SC_NPROCESSORS_ONLN);

	enum { SEL_CPU, SEL_RAM, SEL_SWAP, SEL_PROC } selected = SEL_CPU;

	while (1) {
		int h, w;
		getmaxyx(stdscr, h, w);

		int top_h = 3;
		WINDOW *os_win = newwin(top_h, w / 4, 0, 0);
		WINDOW *kernel_win = newwin(top_h, w / 4, 0, w / 4);
		WINDOW *uptime_win = newwin(top_h, w / 4, 0, w / 2);
		WINDOW *host_win = newwin(top_h, w / 4, 0, 3 * w / 4);

		int middle_h = 7, middle_w = w / 2;
		WINDOW *cpu_win = newwin(middle_h, middle_w, top_h, 0);
		WINDOW *freq_win = newwin(middle_h, middle_w, top_h, middle_w);
		WINDOW *ram_win =
		    newwin(middle_h, middle_w, top_h + middle_h, 0);
		WINDOW *swap_win =
		    newwin(middle_h, middle_w, top_h + middle_h, middle_w);

		int user_root_h = 5;
		WINDOW *me_win =
		    newwin(user_root_h, w / 2, top_h + 2 * middle_h, 0);
		WINDOW *root_win =
		    newwin(user_root_h, w / 2, top_h + 2 * middle_h, w / 2);

		int graph_start = top_h + 2 * middle_h + user_root_h;
		int bottom_h = h - graph_start;
		if (bottom_h < 4) bottom_h = 4;

		int graph_w_part = (w * 7.35) / 10;
		int proc_w_part = w - graph_w_part;

		WINDOW *graph_win =
		    newwin(bottom_h, graph_w_part, graph_start, 0);
		WINDOW *proc_win =
		    newwin(bottom_h, proc_w_part, graph_start, graph_w_part);

		if (!graph || graph_w != graph_w_part) {
			if (graph) free(graph);
			graph_w = graph_w_part;
			graph_h = bottom_h;
			graph = calloc(graph_w, sizeof(int));
			graph_pos = 0;
		}

		WINDOW *wins[] = {os_win,  kernel_win, uptime_win, host_win,
				  cpu_win, freq_win,   ram_win,	   swap_win,
				  me_win,  root_win,   graph_win,  proc_win};
		const char *titles[] = {"OS",	    "KERNEL",	"UPTIME",
					"HOSTNAME", "CPU",	"FREQ",
					"RAM",	    "SWAP",	"USER",
					"ROOT",	    "CPU LOAD", "PROCESSES"};

		for (int i = 0; i < 12; i++) draw_box(wins[i], titles[i]);

		os_box(os_win);
		kernel_box(kernel_win);
		uptime_box(uptime_win);
		hostname_box(host_win);
		cpu_box(cpu_win);
		cpufreq_box(freq_win, cpufreq_scroll);
		ram_box(ram_win, ram_scroll);
		swap_box(swap_win, swap_scroll);
		me_box(me_win);
		root_box(root_win);

		// CPU graph
		float usage = get_total_cpu_usage();
		int bar = (int)(usage * (graph_h - 2) / 100.0);
		graph[graph_pos] = bar;
		graph_pos = (graph_pos + 1) % graph_w;

		werase(graph_win);
		box(graph_win, 0, 0);
		for (int x = 0; x < graph_w; x++) {
			int idx = (graph_pos + x) % graph_w;
			for (int y = 0; y < graph[idx]; y++)
				mvwaddch(graph_win, graph_h - 2 - y, x + 1,
					 '#');
		}
		mvwprintw(graph_win, 0, 2, "CPU LOAD | Usage: %.1f%%", usage);

		proc_box(proc_win, proc_scroll, proc_selected);

		int text_margin = 2;
		char status_text[128];
		const char *sel_text = (selected == SEL_CPU)	? "CPU FREQ"
				       : (selected == SEL_RAM)	? "RAM"
				       : (selected == SEL_SWAP) ? "SWAP"
								: "PROCESSES";

		snprintf(status_text, sizeof(status_text),
			 "Selected: %s  |  LEFT/RIGHT switch  UP/DOWN scroll  "
			 "Q quit",
			 sel_text);

		int start_col = w - strlen(status_text) - text_margin;
		if (start_col < 0) start_col = 0;

		attron(A_REVERSE);
		mvhline(h - 1, 0, ' ', w);
		mvprintw(h - 1, start_col, "%s", status_text);
		attroff(A_REVERSE);

		for (int i = 0; i < 12; i++) wrefresh(wins[i]);
		for (int i = 0; i < 12; i++) delwin(wins[i]);

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
		} else if (ch == KEY_UP && selected == SEL_PROC) {
			if (proc_selected > 0) proc_selected--;
			if (proc_selected < proc_scroll)
				proc_scroll = proc_selected;
		} else if (ch == KEY_DOWN && selected == SEL_PROC) {
			proc_selected++;
			int visible_proc = bottom_h - 2;
			if (proc_selected >= proc_scroll + visible_proc)
				proc_scroll = proc_selected - visible_proc + 1;
		}

		usleep(200000);
	}

	endwin();
	if (graph) free(graph);
	return 0;
}

