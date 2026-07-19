#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

struct cmd_entry {
	const char *name;
	const char *const *aliases;
	cmd_fn fn;
	int direct; 
};

static int cmd_cpu(char *out, size_t sz, int argc, char **argv) {
	if (argc == 2) return cpu(out, sz, argc, argv);
	if (argc >= 3 && strcmp(argv[2], "load") == 0) {
		if (argc == 3) return load(out, sz, argc, argv);
		if (argc == 4) return load_core(out, sz, argc, argv);
	}
	snprintf(out, sz,
			"usage : cpu\n"
			"usage : cpu load\n"
			"usage : cpu load <core>\n");
	return 1;
}

static int cmd_ram(char *out, size_t sz, int argc, char **argv) {
	if (argc == 2) return ram(out, sz, argc, argv);
	if (argc < 3) {
		snprintf(out, sz, "usage : ram [total|left|used] [val]\n");
		return 1;
	}
	int val = (argc >= 4 && strcmp(argv[3], "val") == 0);
	if (strcmp(argv[2], "total") == 0)
		return val ? ram_total_val(out, sz, argc, argv)
			: ram_total(out, sz, argc, argv);
	if (strcmp(argv[2], "left") == 0)
		return val ? ram_left_val(out, sz, argc, argv)
			: ram_left(out, sz, argc, argv);
	if (strcmp(argv[2], "used") == 0)
		return val ? ram_used_val(out, sz, argc, argv)
			: ram_used(out, sz, argc, argv);
	snprintf(out, sz, "usage : ram [total|left|used] [val]\n");
	return 1;
}

static int cmd_swap(char *out, size_t sz, int argc, char **argv) {
	if (argc == 2) return swap(out, sz, argc, argv);
	if (argc < 3) {
		snprintf(out, sz, "usage : swap [total|left|used] [val]\n");
		return 1;
	}
	int val = (argc >= 4 && strcmp(argv[3], "val") == 0);
	if (strcmp(argv[2], "total") == 0)
		return val ? swap_total_val(out, sz, argc, argv)
			: swap_total(out, sz, argc, argv);
	if (strcmp(argv[2], "left") == 0)
		return val ? swap_left_val(out, sz, argc, argv)
			: swap_left(out, sz, argc, argv);
	if (strcmp(argv[2], "used") == 0)
		return val ? swap_used_val(out, sz, argc, argv)
			: swap_used(out, sz, argc, argv);
	snprintf(out, sz, "usage : swap [total|left|used] [val]\n");
	return 1;
}

static int cmd_cpufreq(char *out, size_t sz, int argc, char **argv) {
	if (argc == 2)
		return cpufreq(out, sz, argc, argv);
	if (argc == 3) {
		int core = atoi(argv[2]);
		float f = get_core_freq(core);
		if (f > 0)
			snprintf(out, sz, "frequency of core %d : %0.3f Mhz\n",
					core, f);
		else
			snprintf(out, sz,
					"frequency of core %d : unavailable\n",
					core);
		return 0;
	}
	snprintf(out, sz, "usage : cpufreq [<core>]\n");
	return 1;
}

static int cmd_help(char *out, size_t sz, int argc, char **argv) {
	(void)out; (void)sz; (void)argc; (void)argv;
	help();
	return 0;
}

static int cmd_version(char *out, size_t sz, int argc, char **argv) {
	(void)out; (void)sz; (void)argc; (void)argv;
	printf("sysinfo v3\n");
	return 0;
}

static int cmd_fetch(char *out, size_t sz, int argc, char **argv) {
	(void)out; (void)sz;
	fetch(argc, argv);
	return 0;
}

static int cmd_live(char *out, size_t sz, int argc, char **argv) {
	(void)out; (void)sz; (void)argc; (void)argv;
	livemode();
	return 0;
}

static const char *aliases_host[] = {"host", "hostname", "name", NULL};
static const char *aliases_uptime[] = {"uptime", "up", NULL};
static const char *aliases_cores[] = {"cpucores", "cores", "core",
	"cpucore", NULL};
static const char *aliases_freq[] = {"cpufreq", "freq", "cpufrequency",
	"frequency", NULL};
static const char *aliases_mem[] = {"ram", "mem", "memory", NULL};
static const char *aliases_proc[] = {"proc", "ps", NULL};
static const char *aliases_sys[] = {"system", "sys", NULL};

static const struct cmd_entry table[] = {
	{"editor", NULL, editor, 0},
	{"language", NULL, language, 0},
	{"region", NULL, region, 0},
	{"encoding", NULL, encoding, 0},
	{"locale", NULL, locale, 0},
	{"os", NULL, os, 0},
	{"kernel", NULL, kernel, 0},
	{"host", aliases_host, hostname, 0},
	{"uptime", aliases_uptime, uptime, 0},
	{"cpu", NULL, cmd_cpu, 0},
	{"cpucores", aliases_cores, cpucores, 0},
	{"cpufreq", aliases_freq, cmd_cpufreq, 0},
	{"ram", aliases_mem, cmd_ram, 0},
	{"swap", NULL, cmd_swap, 0},
	{"root", NULL, root, 0},
	{"me", NULL, me, 0},
	{"time", NULL, show_time, 0},
	{"date", NULL, show_date, 0},
	{"drives", NULL, drives, 0},
	{"shell", NULL, shell, 0},
	{"term", NULL, term, 0},
	{"ip", NULL, ip, 0},
	{"ping", NULL, ping, 0},
	{"bios", NULL, bios, 0},
	{"system", aliases_sys, system_info, 0},
	{"board", NULL, board, 0},
	{"init", NULL, init, 0},
	{"proc", aliases_proc, proc, 0},
	{"help", NULL, cmd_help, 1},
	{"version", NULL, cmd_version, 1},
	{"fetch", NULL, cmd_fetch, 1},
	{"live", NULL, cmd_live, 1},
};

#define NCMD (int)(sizeof(table) / sizeof(table[0]))

static const struct cmd_entry *lookup(const char *name) {
	for (int i = 0; i < NCMD; i++) {
		if (strcmp(table[i].name, name) == 0)
			return &table[i];
		const char *const *a = table[i].aliases;
		for (; a && *a; a++)
			if (strcmp(*a, name) == 0)
				return &table[i];
	}
	return NULL;
}

int dispatch_command(int argc, char **argv, char *out, size_t sz) {
	out[0] = '\0';
	if (argc < 2) {
		help();
		return 0;
	}

	const struct cmd_entry *e = lookup(argv[1]);
	if (!e) {
		fprintf(stderr, "sys: unknown command '%s'\n", argv[1]);
		fprintf(stderr, "run 'sys help' for a list of commands\n");
		return 2;
	}

	int rc = e->fn(out, sz, argc, argv);
	if (rc != 0 && !e->direct && out[0] != '\0')
		fprintf(stderr, "sys: %s", out);
	return rc;
}
