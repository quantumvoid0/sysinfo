#include <arpa/inet.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

#include "functions.h"

int editor(char *out, size_t sz, int argc, char **argv) {
	char *e = getenv("EDITOR");
	snprintf(out, sz, "%s\n", e ? e : "");
	return 0;
}

int locale(char *out, size_t sz, int argc, char **argv) {
	char *l = getenv("LANG");
	snprintf(out, sz, "%s\n", l ? l : "");
	return 0;
}

int language(char *out, size_t sz, int argc, char **argv) {
	char *l = getenv("LANG");
	if (!l) {
		snprintf(out, sz, "\n");
		return 0;
	}
	char *p = strdup(l);
	char *tok = strtok(p, "_.");
	snprintf(out, sz, "%s\n", tok ? tok : "");
	free(p);
	return 0;
}

int region(char *out, size_t sz, int argc, char **argv) {
	char *l = getenv("LANG");
	if (!l) {
		snprintf(out, sz, "\n");
		return 0;
	}
	char *p = strdup(l);
	char *tok = strtok(p, "_.");
	if (tok) tok = strtok(NULL, ".");
	snprintf(out, sz, "%s\n", tok ? tok : "");
	free(p);
	return 0;
}

int encoding(char *out, size_t sz, int argc, char **argv) {
	char *l = getenv("LANG");
	if (!l) {
		snprintf(out, sz, "\n");
		return 0;
	}
	char *p = strdup(l);
	char *tok = strtok(p, ".");
	if (tok) tok = strtok(NULL, "@");
	snprintf(out, sz, "%s\n", tok ? tok : "");
	free(p);
	return 0;
}

int shell(char *out, size_t sz, int argc, char **argv) {
	char *s = getenv("SHELL");
	if (!s) {
		snprintf(out, sz, "\n");
		return 0;
	}
	snprintf(out, sz, "%s\n", strrchr(s, '/') ? strrchr(s, '/') + 1 : s);
	return 0;
}

int term(char *out, size_t sz, int argc, char **argv) {
	char *t = getenv("TERM");
	snprintf(out, sz, "%s\n", t ? t : "");
	return 0;
}

int bios(char *out, size_t sz, int argc, char **argv) {
	const char *file = "bios_vendor";
	if (argc > 2) {
		if (strcmp(argv[2], "vendor") == 0)
			file = "bios_vendor";
		else if (strcmp(argv[2], "version") == 0)
			file = "bios_version";
		else if (strcmp(argv[2], "date") == 0)
			file = "bios_date";
		else if (strcmp(argv[2], "release") == 0)
			file = "bios_release";
		else {
			snprintf(out, sz,
					"Usage: sys bios [vendor/version/date/release]\n");
			return 1;
		}
	}
	char buf[256];
	if (read_dmi(file, buf, sizeof(buf)))
		snprintf(out, sz, "%s\n", buf);
	else
		snprintf(out, sz, "\n");
	return 0;
}

int system_info(char *out, size_t sz, int argc, char **argv) {
	const char *file = "product_name";
	if (argc > 2) {
		if (strcmp(argv[2], "family") == 0)
			file = "product_family";
		else if (strcmp(argv[2], "name") == 0)
			file = "product_name";
		else if (strcmp(argv[2], "sku") == 0)
			file = "product_sku";
		else if (strcmp(argv[2], "version") == 0)
			file = "product_version";
		else {
			snprintf(out, sz,
					"Usage: sys system [family/name/sku/version]\n");
			return 1;
		}
	}
	char buf[256];
	if (read_dmi(file, buf, sizeof(buf)))
		snprintf(out, sz, "%s\n", buf);
	else
		snprintf(out, sz, "\n");
	return 0;
}

int board(char *out, size_t sz, int argc, char **argv) {
	const char *file = "board_vendor";
	if (argc > 2) {
		if (strcmp(argv[2], "tag") == 0)
			file = "board_asset_tag";
		else if (strcmp(argv[2], "name") == 0)
			file = "board_name";
		else if (strcmp(argv[2], "vendor") == 0)
			file = "board_vendor";
		else if (strcmp(argv[2], "version") == 0)
			file = "board_version";
		else {
			snprintf(out, sz,
					"Usage: sys board [tag/name/vendor/version]\n");
			return 1;
		}
	}
	char buf[256];
	if (read_dmi(file, buf, sizeof(buf)))
		snprintf(out, sz, "%s\n", buf);
	else
		snprintf(out, sz, "\n");
	return 0;
}

int init(char *out, size_t sz, int argc, char **argv) {
	FILE *fp = fopen("/proc/1/comm", "r");
	if (!fp) {
		snprintf(out, sz, "\n");
		return 0;
	}
	char buf[256];
	if (fgets(buf, sizeof(buf), fp)) {
		size_t len = strlen(buf);
		if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
		snprintf(out, sz, "%s\n", buf);
	}
	fclose(fp);
	return 0;
}

int proc(char *out, size_t sz, int argc, char **argv) {
	size_t off = 0;
	if (argc > 2 && strcmp(argv[2], "num") == 0) {
		int count = 0;
		for (int pid = 1; pid < 65536; pid++) {
			char path[32];
			snprintf(path, sizeof(path), "/proc/%d/stat", pid);
			if (access(path, F_OK) == 0) count++;
		}
		off = out_append(out, off, sz, "%d\n", count);
		return 0;
	}

	DIR *proc = opendir("/proc");
	if (!proc) {
		snprintf(out, sz, "opendir failed\n");
		return 1;

	}
	off = out_append(out, off, sz, "%-6s %s\n", "PID", "COMMAND");
	struct dirent *de;
	while ((de = readdir(proc)) != NULL) {
		if (de->d_type != DT_DIR) continue;
		int pid = atoi(de->d_name);
		if (pid == 0) continue;
		char path[32];
		snprintf(path, sizeof(path), "/proc/%d/comm", pid);
		FILE *fp = fopen(path, "r");
		if (!fp) continue;
		char comm[256];
		if (fgets(comm, sizeof(comm), fp)) {
			size_t len = strlen(comm);
			if (len > 0 && comm[len - 1] == '\n')
				comm[len - 1] = '\0';
			off = out_append(out, off, sz, "%-6d %s\n",
					pid, comm);
		}
		fclose(fp);
	}
	closedir(proc);
	return 0;
}

int ip(char *out, size_t sz, int argc, char **argv) {
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		snprintf(out, sz, "socket failed\n");
		return 1;

	}
	struct sockaddr_in dst;
	memset(&dst, 0, sizeof(dst));
	dst.sin_family = AF_INET;
	dst.sin_addr.s_addr = inet_addr("1.1.1.1");

	if (connect(sock, (struct sockaddr *)&dst, sizeof(dst)) != 0) {
		snprintf(out, sz, "connect failed\n");
		close(sock);
		return 1;
	}

	struct sockaddr_in local;
	socklen_t len = sizeof(local);
	if (getsockname(sock, (struct sockaddr *)&local, &len) != 0) {
		snprintf(out, sz, "getsockname failed\n");
		close(sock);
		return 1;
	}
	close(sock);

	char ipstr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &local.sin_addr, ipstr, sizeof(ipstr));
	snprintf(out, sz, "%s\n", ipstr);
	return 0;
}

int ping(char *out, size_t sz, int argc, char **argv) {
	FILE *fp = popen("ping -c 1 1.1.1.1", "r");
	if (!fp) {
		snprintf(out, sz, "popen failed\n");
		return 1;

	}
	char buf[256];
	while (fgets(buf, sizeof(buf), fp)) {
		char *t = strstr(buf, "time=");
		if (t) {
			snprintf(out, sz, "%s", t);
			break;
		}
	}
	pclose(fp);
	return 0;
}
