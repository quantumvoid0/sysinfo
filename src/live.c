#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define CORES 256

typedef struct {
	unsigned long long user, nice, system, idle, iowait, irq, softirq,
	    steal;
} cpu_stat;

void read_cpu_stats_live(cpu_stat stats[], int cores) {
	FILE *fp = fopen("/proc/stat", "r");
	if (!fp) return;
	char line[256];
	int idx = 0;
	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "cpu", 3) != 0) break;
		if (line[3] >= '0' && line[3] <= '9') {
			sscanf(line,
			       "cpu%*d %llu %llu %llu %llu %llu %llu %llu %llu",
			       &stats[idx].user, &stats[idx].nice,
			       &stats[idx].system, &stats[idx].idle,
			       &stats[idx].iowait, &stats[idx].irq,
			       &stats[idx].softirq, &stats[idx].steal);
			idx++;
			if (idx >= cores) break;
		}
	}
	fclose(fp);
}

float get_total_cpu_usage() {
	static cpu_stat prev[CORES] = {0};
	cpu_stat curr[CORES];
	int cores = sysconf(_SC_NPROCESSORS_ONLN);
	read_cpu_stats_live(curr, cores);
	float total = 0.0;

	for (int i = 0; i < cores; i++) {
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
		double usage = (double)((curr_total - prev_total) -
					(curr_idle - prev_idle)) /
			       (curr_total - prev_total) * 100.0;
		total += usage;
		prev[i] = curr[i];
	}

	return total / cores;
}

void draw_box(WINDOW *win, const char *title) {
	werase(win);
	box(win, 0, 0);
	mvwprintw(win, 0, 2, "%s", title);
}

void os_box(WINDOW *win) {
	char buffer[256];
	FILE *fp = popen("grep PRETTY_NAME /etc/os-release", "r");
	if (fp && fgets(buffer, sizeof(buffer), fp)) {
		char *x = strchr(buffer, '=');
		if (x) x++;
		if (x[0] == '"') x++;
		size_t len = strlen(x);
		if (x[len - 2] == '"') x[len - 2] = 0;
		mvwprintw(win, 1, 2, "%s", x);
	}
	if (fp) pclose(fp);
}

void kernel_box(WINDOW *win) {
	char buffer[256];
	FILE *fp = popen("uname -r", "r");
	if (fp && fgets(buffer, sizeof(buffer), fp))
		mvwprintw(win, 1, 2, "%s", buffer);
	if (fp) pclose(fp);
}

void hostname_box(WINDOW *win) {
	char buffer[256];
	FILE *fp = popen("uname -n", "r");
	if (fp && fgets(buffer, sizeof(buffer), fp))
		mvwprintw(win, 1, 2, "%s", buffer);
	if (fp) pclose(fp);
}

void uptime_box(WINDOW *win) {
	char buffer[256];
	FILE *fp = popen("uptime -p", "r");
	if (fp && fgets(buffer, sizeof(buffer), fp))
		mvwprintw(win, 1, 2, "%s", buffer + 3);
	if (fp) pclose(fp);
}

void cpu_box(WINDOW *win) {
	char buffer[256];
	FILE *fp = popen("grep 'model name' /proc/cpuinfo", "r");
	if (fp && fgets(buffer, sizeof(buffer), fp))
		mvwprintw(win, 1, 2, "%s", buffer + 13);
	if (fp) pclose(fp);
}

void cpufreq_box(WINDOW *win, int scroll_offset) {
	char buffer[256];
	FILE *fp = popen(
	    "cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq", "r");
	if (!fp) return;

	int row = 1;
	int i = 0;
	int max_rows, max_cols;
	getmaxyx(win, max_rows, max_cols);

	while (fgets(buffer, sizeof(buffer), fp)) {
		i++;
		if (i <= scroll_offset) continue;
		if (row >= max_rows - 1) break;
		float f = atol(buffer) / 1000.0;
		mvwprintw(win, row++, 2, "Core %d: %.2f MHz", i, f);
	}

	pclose(fp);
}

void ram_box(WINDOW *win, int scroll_offset) {
	char buffer[256];
	FILE *fp = popen("grep -E 'MemTotal|MemAvailable' /proc/meminfo", "r");
	if (!fp) return;

	float total = 0, avail = 0;
	char lines[16][128];
	int line_count = 0;

	while (fgets(buffer, sizeof(buffer), fp)) {
		if (strncmp(buffer, "MemTotal:", 9) == 0)
			total = atol(buffer + 9) / 1048576.0;
		if (strncmp(buffer, "MemAvailable:", 13) == 0)
			avail = atol(buffer + 13) / 1048576.0;
	}
	pclose(fp);

	snprintf(lines[line_count++], 128, "Total RAM   : %.2f GB", total);
	snprintf(lines[line_count++], 128, "Available   : %.2f GB", avail);
	snprintf(lines[line_count++], 128, "Used        : %.2f GB",
		 total - avail);

	int max_rows, max_cols;
	getmaxyx(win, max_rows, max_cols);
	int row = 1;
	for (int i = scroll_offset; i < line_count && row < max_rows - 1; i++)
		mvwprintw(win, row++, 2, "%s", lines[i]);
}

void swap_box(WINDOW *win, int scroll_offset) {
	char buffer[256];
	FILE *fp = popen("grep -E 'SwapTotal|SwapFree' /proc/meminfo", "r");
	if (!fp) return;

	float total = 0, free = 0;
	char lines[16][128];
	int line_count = 0;

	while (fgets(buffer, sizeof(buffer), fp)) {
		if (strncmp(buffer, "SwapTotal:", 10) == 0)
			total = atol(buffer + 10) / 1048576.0;
		if (strncmp(buffer, "SwapFree:", 9) == 0)
			free = atol(buffer + 9) / 1048576.0;
	}
	pclose(fp);

	snprintf(lines[line_count++], 128, "Total Swap  : %.2f GB", total);
	snprintf(lines[line_count++], 128, "Free Swap   : %.2f GB", free);
	snprintf(lines[line_count++], 128, "Used Swap   : %.2f GB",
		 total - free);

	int max_rows, max_cols;
	getmaxyx(win, max_rows, max_cols);
	int row = 1;
	for (int i = scroll_offset; i < line_count && row < max_rows - 1; i++)
		mvwprintw(win, row++, 2, "%s", lines[i]);
}

void root_box(WINDOW *win) {
	char buffer[256];
	FILE *fp = popen("df -h /", "r");
	int row = 1;
	while (fp && fgets(buffer, sizeof(buffer), fp))
		mvwprintw(win, row++, 2, "%s", buffer);
	if (fp) pclose(fp);
}

void me_box(WINDOW *win) {
	char buffer[256];
	FILE *fp = popen("whoami", "r");
	if (fp && fgets(buffer, sizeof(buffer), fp))
		mvwprintw(win, 1, 2, "%s", buffer);
	if (fp) pclose(fp);
}

void proc_box(WINDOW *win, int scroll, int selected_line) {
	werase(win);
	box(win, 0, 0);
	mvwprintw(win, 0, 2, "PROCESSES");

	int maxy, maxx;
	getmaxyx(win, maxy, maxx);

	FILE *fp = popen(
	    "ps -eo pid,comm,user,%cpu,%mem,tty,start_time --sort=-%cpu", "r");
	if (!fp) {
		mvwprintw(win, 1, 1, "Failed to run ps");
		return;
	}

	char buf[256];
	int row = 1;
	int index = 0;

	while (fgets(buf, sizeof(buf), fp)) {
		if (index++ < scroll) continue;
		if (row >= maxy - 1) break;

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

	pclose(fp);
}

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

