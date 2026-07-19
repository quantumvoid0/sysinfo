
#include <stdio.h>
#include <string.h>

#include "live_data.h"

static int check(const char *name, const char *val) {
	if (val && strlen(val) > 0) {
		printf("  ok   %s -> %s\n", name, val);
		return 0;
	}
	printf("  FAIL %s -> empty\n", name);
	return 1;
}

int main(void) {
	int fail = 0;
	char buf[LIVE_LINE_SZ];

	live_os(buf, sizeof(buf));
	fail |= check("live_os", buf);

	live_kernel(buf, sizeof(buf));
	fail |= check("live_kernel", buf);

	live_hostname(buf, sizeof(buf));
	fail |= check("live_hostname", buf);

	live_uptime(buf, sizeof(buf));
	fail |= check("live_uptime", buf);

	live_cpu_model(buf, sizeof(buf));
	fail |= check("live_cpu_model", buf);

	live_me(buf, sizeof(buf));
	fail |= check("live_me", buf);

	char lines[LIVE_MAX_LINES][LIVE_LINE_SZ];
	int n = 0;
	live_ram(lines, &n);
	fail |= (n == 3) ? 0 : (printf("  FAIL live_ram count=%d\n", n), 1);
	for (int i = 0; i < n; i++)
		fail |= check("live_ram[]", lines[i]);

	live_swap(lines, &n);
	fail |= (n == 3) ? 0 : (printf("  FAIL live_swap count=%d\n", n), 1);
	for (int i = 0; i < n; i++)
		fail |= check("live_swap[]", lines[i]);

	live_root(lines, &n);
	fail |= (n >= 3) ? 0 : (printf("  FAIL live_root count=%d\n", n), 1);
	for (int i = 0; i < n; i++)
		fail |= check("live_root[]", lines[i]);

	int cores = live_cpufreq_count();
	fail |= (cores > 0) ? 0 : (printf("  FAIL cpufreq_count=0\n"), 1);
	for (int c = 0; c < cores; c++) {
		live_cpufreq_core(buf, sizeof(buf), c);
		fail |= check("live_cpufreq_core", buf);
	}


	float u1 = live_total_cpu_usage();
	float u2 = live_total_cpu_usage();
	fail |= (u2 >= 0.0f && u2 <= 100.0f)
		? 0
		: (printf("  FAIL cpu_usage=%f\n", u2), 1);
	printf("  ok   live_total_cpu_usage -> %.1f%% (sample %.1f%%)\n", u2,
			u1);

	// systemd evil
	live_proc_line(buf, sizeof(buf), 1, "systemd");
	fail |= (strstr(buf, "systemd") != NULL)
		? 0
		: (printf("  FAIL live_proc_line '%s'\n", buf), 1);
	printf("  ok   live_proc_line -> %s\n", buf);

	if (fail == 0) {
		printf("DATA LAYER TESTS PASSED\n");
		return 0;
	}
	printf("DATA LAYER TESTS FAILED\n");
	return 1;
}
