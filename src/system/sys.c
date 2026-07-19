#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "functions.h"

static char *trim_quotes(char *s) {
	while (*s == '"' || *s == '\'') s++;
	size_t len = strlen(s);
	while (len > 0 && (s[len - 1] == '"' || s[len - 1] == '\'' ||
				s[len - 1] == '\n'))
		s[--len] = '\0';
	return s;
}

int os(char *out, size_t sz, int argc, char **argv) {
	FILE *fp = fopen("/etc/os-release", "r");
	if (!fp) {
		snprintf(out, sz, "unknown\n");
		return 0;
	}
	char buffer[256];
	int found = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "PRETTY_NAME", 11) == 0) {
			char *x = buffer + 11;
			if (x[0] == '=') x++;
			x = trim_quotes(x);
			snprintf(out, sz, "%s\n", x);
			found = 1;
			break;
		}
	}
	fclose(fp);
	if (!found) {
		snprintf(out, sz, "unknown\n");
		return 0;
	}
	return 0;
}

int kernel(char *out, size_t sz, int argc, char **argv) {
	struct utsname u;
	if (uname(&u) == 0) {
		snprintf(out, sz, "%s\n", u.release);
		return 0;
	}
	snprintf(out, sz, "uname failed\n");
	return 1;
}

int hostname(char *out, size_t sz, int argc, char **argv) {
	char buf[256];
	if (gethostname(buf, sizeof(buf)) == 0) {
		snprintf(out, sz, "%s\n", buf);
		return 0;
	}
	snprintf(out, sz, "gethostname failed\n");
	return 1;
}

int uptime(char *out, size_t sz, int argc, char **argv) {
	struct sysinfo info;
	if (sysinfo(&info) != 0) {
		snprintf(out, sz, "sysinfo failed\n");
		return 1;
	}
	long secs = info.uptime;
	long days = secs / 86400;
	secs %= 86400;
	long hours = secs / 3600;
	secs %= 3600;
	long mins = secs / 60;

	if (days > 0)
		snprintf(out, sz, "%ld days, %ld hours, %ld minutes\n", days,
				hours, mins);
	else if (hours > 0)
		snprintf(out, sz, "%ld hours, %ld minutes\n", hours, mins);
	else
		snprintf(out, sz, "%ld minutes\n", mins);
	return 0;
}

int root(char *out, size_t sz, int argc, char **argv) {
	struct statvfs st;
	if (statvfs("/", &st) != 0) {
		snprintf(out, sz, "statvfs failed\n");
		return 1;
	}
	unsigned long long total = (unsigned long long)st.f_blocks * st.f_frsize;
	unsigned long long bfree = (unsigned long long)st.f_bfree * st.f_frsize;
	unsigned long long used = total - bfree;
	double used_gb = used / (1024.0 * 1024 * 1024);
	double total_gb = total / (1024.0 * 1024 * 1024);
	int percent = total ? (int)(used * 100 / total) : 0;
	snprintf(out, sz, "%0.2f/%0.2f GB [%d%%]\n", used_gb, total_gb,
			percent);
	return 0;
}

int me(char *out, size_t sz, int argc, char **argv) {
	char *user = getenv("USER");
	if (!user) user = getenv("LOGNAME");
	if (!user) {
		uid_t uid = getuid();
		struct passwd *pw = getpwuid(uid);
		if (pw) user = pw->pw_name;
	}
	snprintf(out, sz, "%s\n", user ? user : "unknown");
	return 0;
}
