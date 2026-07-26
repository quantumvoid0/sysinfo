#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "live.h"
#include "live_data.h"

void live_os(char *out, size_t sz) {
	FILE *fp = fopen("/etc/os-release", "r");
	if (!fp) {
		snprintf(out, sz, "unknown");
		return;
	}
	char buffer[256];
	while (fgets(buffer, sizeof(buffer), fp)) {
		if (strncmp(buffer, "PRETTY_NAME", 11) == 0) {
			char *x = strchr(buffer, '=');
			if (!x) break;
			x++;
			while (*x == ' ' || *x == '"' || *x == '\'') x++;
			size_t len = strlen(x);
			while (len > 0 && (x[len - 1] == '"' ||
						x[len - 1] == '\'' ||
						x[len - 1] == '\n'))
				x[--len] = 0;
			snprintf(out, sz, "%s", x);
			break;
		}
	}
	fclose(fp);
}

void live_kernel(char *out, size_t sz) {
	struct utsname u;
	if (uname(&u) == 0)
		snprintf(out, sz, "%s", u.release);
	else
		snprintf(out, sz, "uname failed");
}

void live_hostname(char *out, size_t sz) {
	char buf[256];
	if (gethostname(buf, sizeof(buf)) == 0)
		snprintf(out, sz, "%s", buf);
	else
		snprintf(out, sz, "gethostname failed");
}

void live_uptime(char *out, size_t sz) {
	struct sysinfo info;
	if (sysinfo(&info) != 0) {
		snprintf(out, sz, "sysinfo failed");
		return;
	}
	long secs = info.uptime;
	long days = secs / 86400;
	secs %= 86400;
	long hours = secs / 3600;
	secs %= 3600;
	long mins = secs / 60;

	if (days > 0)
		snprintf(out, sz, "%ld days, %ld hours, %ld minutes", days,
				hours, mins);
	else if (hours > 0)
		snprintf(out, sz, "%ld hours, %ld minutes", hours, mins);
	else
		snprintf(out, sz, "%ld minutes", mins);
}

void live_cpu_model(char *out, size_t sz) {
	if (!cpu_model_name(out, sz))
		snprintf(out, sz, "unknown");
}

void live_me(char *out, size_t sz) {
	char *user = getenv("USER");
	if (!user) user = getenv("LOGNAME");
	if (!user) {
		uid_t uid = getuid();
		struct passwd *pw = getpwuid(uid);
		if (pw) user = pw->pw_name;
	}
	snprintf(out, sz, "%s", user ? user : "unknown");
}

void live_ram(char lines[][LIVE_LINE_SZ], int *n) {
	*n = 0;
	float total = meminfo_value("MemTotal:") / MB;
	float avail = meminfo_value("MemAvailable:") / MB;
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Total RAM   : %.2f GB", total);
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Available   : %.2f GB", avail);
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Used        : %.2f GB",
			total - avail);
}

void live_swap(char lines[][LIVE_LINE_SZ], int *n) {
	*n = 0;
	float total = meminfo_value("SwapTotal:") / MB;
	float free = meminfo_value("SwapFree:") / MB;
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Total Swap  : %.2f GB", total);
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Free Swap   : %.2f GB", free);
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Used Swap   : %.2f GB",
			total - free);
}

void live_root(char lines[][LIVE_LINE_SZ], int *n) {
	*n = 0;
	struct statvfs st;
	if (statvfs("/", &st) != 0) {
		snprintf(lines[(*n)++], LIVE_LINE_SZ, "statvfs failed");
		return;
	}
	unsigned long long total = (unsigned long long)st.f_blocks * st.f_frsize;
	unsigned long long avail = (unsigned long long)st.f_bavail * st.f_frsize;
	unsigned long long used = total - avail;
	double total_gb = total / (1024.0 * 1024 * 1024);
	double used_gb = used / (1024.0 * 1024 * 1024);
	double avail_gb = avail / (1024.0 * 1024 * 1024);
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Total: %.2f GB", total_gb);
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Used:  %.2f GB", used_gb);
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Avail: %.2f GB", avail_gb);
}

int live_cpufreq_count(void) {
	return sysconf(_SC_NPROCESSORS_ONLN);
}

void live_cpufreq_core(char *out, size_t sz, int core) {
	char path[128];
	snprintf(path, sizeof(path),
			"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq",
			core);
	FILE *fp = fopen(path, "r");
	if (!fp) {
		snprintf(out, sz, "Core %d: unavailable", core);
		return;
	}
	long freq_khz;
	if (fscanf(fp, "%ld", &freq_khz) != 1) {
		fclose(fp);
		snprintf(out, sz, "Core %d: unavailable", core);
		return;
	}
	fclose(fp);
	snprintf(out, sz, "Core %d: %.2f MHz", core, freq_khz / 1000.0f);
}

float live_total_cpu_usage(void) {
	static cpu_stat prev[CORES] = {0};
	cpu_stat curr[CORES];
	int cores = sysconf(_SC_NPROCESSORS_ONLN);
	read_cpu_stats(curr, cores);
	float total = 0.0f;
	for (int i = 0; i < cores; i++) {
		total += cpu_usage_percent(&prev[i], &curr[i]);
		prev[i] = curr[i];
	}
	return total / cores;
}

void live_proc_line(char *out, size_t sz, int pid, const char *comm) {
	snprintf(out, sz, "%-6d %s", pid, comm);
}

void live_battery(char *out, size_t sz) {
	FILE *fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	if (!fp) {
		snprintf(out, sz, "N/A");
		return;
	}
	int cap;
	if (fscanf(fp, "%d", &cap) != 1) {
		fclose(fp);
		snprintf(out, sz, "N/A");
		return;
	}
	fclose(fp);

	fp = fopen("/sys/class/power_supply/BAT0/status", "r");
	if (fp) {
		char st[16];
		if (fgets(st, sizeof(st), fp)) {
			size_t len = strlen(st);
			if (len > 0 && st[len - 1] == '\n') st[len - 1] = 0;
			snprintf(out, sz, "%d%% (%s)", cap, st);
		} else {
			snprintf(out, sz, "%d%%", cap);
		}
		fclose(fp);
	} else {
		snprintf(out, sz, "%d%%", cap);
	}
}

void live_battery_full(char lines[][LIVE_LINE_SZ], int *n) {
	*n = 0;

	FILE *fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	if (!fp) {
		snprintf(lines[(*n)++], LIVE_LINE_SZ, "Status: Not present");
		return;
	}
	int cap;
	if (fscanf(fp, "%d", &cap) != 1) {
		fclose(fp);
		snprintf(lines[(*n)++], LIVE_LINE_SZ, "Status: Not present");
		return;
	}
	fclose(fp);

	snprintf(lines[(*n)++], LIVE_LINE_SZ, "Capacity: %d%%", cap);

	fp = fopen("/sys/class/power_supply/BAT0/status", "r");
	if (fp) {
		char st[16];
		if (fgets(st, sizeof(st), fp)) {
			size_t len = strlen(st);
			if (len > 0 && st[len - 1] == '\n') st[len - 1] = 0;
			snprintf(lines[(*n)++], LIVE_LINE_SZ, "Status: %s", st);
		}
		fclose(fp);
	}

	char bar[32];
	int pos = 0;
	bar[pos++] = '[';
	int bar_w = 10;
	int filled = (cap * bar_w) / 100;
	if (filled > bar_w) filled = bar_w;
	for (int i = 0; i < bar_w; i++)
		bar[pos++] = i < filled ? '#' : '.';
	bar[pos++] = ']';
	bar[pos] = '\0';
	snprintf(lines[(*n)++], LIVE_LINE_SZ, "%s", bar);
}
