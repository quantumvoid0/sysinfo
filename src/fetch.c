#define _GNU_SOURCE
#include "fetch/fetch.h"
#include "functions.h"

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define INFO_COL 35

const char *forbidden[] = {"fetch",  "proc", "update",
	"drives", "live", "version-latest"};
int forbidden_count = sizeof(forbidden) / sizeof(forbidden[0]);

const char *current_cmd = NULL;

const char *get_icon(const char *cmd) {
	if (!cmd) return "";
	if (strcmp(cmd, "os") == 0) return "󰌽 ";
	if (strcmp(cmd, "kernel") == 0) return " ";
	if (strcmp(cmd, "host") == 0 || strcmp(cmd, "hostname") == 0 ||
			strcmp(cmd, "name") == 0)
		return "󱩊 ";
	if (strcmp(cmd, "uptime") == 0 || strcmp(cmd, "up") == 0) return " ";
	if (strcmp(cmd, "cpu") == 0 || strcmp(cmd, "cpu load") == 0)
		return " ";
	if (strcmp(cmd, "cpucores") == 0 || strcmp(cmd, "cores") == 0 ||
			strcmp(cmd, "core") == 0 || strcmp(cmd, "cpucore") == 0)
		return " ";
	if (strcmp(cmd, "cpufreq") == 0 || strcmp(cmd, "freq") == 0 ||
			strcmp(cmd, "cpufrequency") == 0)
		return "󰤁";
	if (strcmp(cmd, "ram") == 0 || strcmp(cmd, "mem") == 0 ||
			strcmp(cmd, "memory") == 0)
		return " ";
	if (strcmp(cmd, "swap") == 0) return "󰿣 ";
	if (strcmp(cmd, "root") == 0) return " ";
	if (strcmp(cmd, "me") == 0) return " ";
	if (strcmp(cmd, "time") == 0) return "󰥔 ";
	if (strcmp(cmd, "date") == 0) return " ";
	if (strcmp(cmd, "shell") == 0) return " ";
	if (strcmp(cmd, "term") == 0) return " ";
	if (strcmp(cmd, "ip") == 0) return "󰩟 ";
	if (strcmp(cmd, "bios") == 0) return " ";
	if (strcmp(cmd, "system") == 0) return "󰌢 ";
	if (strcmp(cmd, "board") == 0) return "󰚗 ";
	if (strcmp(cmd, "init") == 0) return " ";
	if (strcmp(cmd, "proc") == 0) return " ";
	if (strcmp(cmd, "ping") == 0) return " ";
	if (strcmp(cmd, "editor") == 0) return "󰯂 ";
	if (strcmp(cmd, "language") == 0) return " ";
	if (strcmp(cmd, "region") == 0) return "󰇧 ";
	if (strcmp(cmd, "encoding") == 0) return "󱘣 ";
	if (strcmp(cmd, "locale") == 0) return " ";
	if (strcmp(cmd, "version") == 0) return " ";

	if (strcmp(cmd, "fetch") == 0 || strcmp(cmd, "proc") == 0 ||
			strcmp(cmd, "update") == 0 || strcmp(cmd, "drives") == 0 ||
			strcmp(cmd, "live") == 0 || strcmp(cmd, "version-latest") == 0)
		return " ";

	return "";
}

const char *get_color(const char *name) {
	if (!name) return "\033[0m";
	if (strcasecmp(name, "red") == 0) return "\033[31m";
	if (strcasecmp(name, "green") == 0) return "\033[32m";
	if (strcasecmp(name, "yellow") == 0) return "\033[33m";
	if (strcasecmp(name, "blue") == 0) return "\033[34m";
	if (strcasecmp(name, "magenta") == 0) return "\033[35m";
	if (strcasecmp(name, "cyan") == 0) return "\033[36m";
	if (strcasecmp(name, "white") == 0) return "\033[37m";
	return "\033[0m";
}

char icons[20][32];

static void run_command(const char *cmd, char *out, size_t outsz) {
	char *copy = strdup(cmd);
	char *argv[16];
	int argc = 1;
	argv[0] = "sys";
	char *tok = strtok(copy, " ");
	while (tok && argc < 16) {
		argv[argc++] = tok;
		tok = strtok(NULL, " ");
	}
	out[0] = '\0';
	if (argc > 1)
		dispatch_command(argc, argv, out, outsz);
	free(copy);
}

static const char *resolve_config_path(char *buf, size_t sz) {
	const char *xdg = getenv("XDG_CONFIG_HOME");
	if (xdg && *xdg)
		snprintf(buf, sz, "%s/sysinfo/config.json", xdg);
	else
		snprintf(buf, sz, "%s/.config/sysinfo/config.json",
				getenv("HOME") ? getenv("HOME") : "");
	return buf;
}

static const char *resolve_ascii_dir(char *buf, size_t sz) {
	const char *candidates[3];
	int n = 0;
	if (getenv("HOME")) {
		snprintf(buf, sz, "%s/.local/share/sysinfo",
				getenv("HOME"));
		candidates[n++] = buf;
	}
	candidates[n++] = "/usr/share/sysinfo";
	for (int i = 0; i < n; i++) {
		DIR *d = opendir(candidates[i]);
		if (d) { closedir(d); return candidates[i]; }
	}
	return candidates[n - 1];
}

int fetch(int argc, char **argv) {
	char config_path[512];
	resolve_config_path(config_path, sizeof(config_path));

	char commands[20][64];
	int n = load_commands_from_config(commands, 20, config_path);
	if (n == 0) {
		const char *default_commands[] = {
			"os", "kernel",	"host",	    "uptime", "cpu",
			"me", "swap total", "ram used", "cores"};
		n = sizeof(default_commands) / sizeof(default_commands[0]);
		for (int i = 0; i < n; i++)
			strncpy(commands[i], default_commands[i], 63);
	}

	char ascii_color[16] = "reset";
	load_ascii_color_from_config(config_path, ascii_color,
			sizeof(ascii_color));
	const char *color_code = get_color(ascii_color);

	char outputs[20][OUT_SZ];
	char last_os[OUT_SZ] = {0};
	char icons[20][32] = {{0}};
	load_icons_from_config(config_path, commands, icons, 20);

	{
		run_command("os", last_os, sizeof(last_os));
		last_os[strcspn(last_os, "\n")] = '\0';
	}

	for (int i = 0; i < n; i++) {
		int is_forbidden = 0;
		for (int f = 0; f < forbidden_count; f++) {
			if (strcasecmp(commands[i], forbidden[f]) == 0) {
				snprintf(outputs[i], sizeof(outputs[i]),
						"command is not allowed");
				is_forbidden = 1;
				break;
			}
		}

		if (is_forbidden) continue;

		run_command(commands[i], outputs[i], sizeof(outputs[i]));
		outputs[i][strcspn(outputs[i], "\n")] = '\0';
		if (strcmp(commands[i], "os") == 0) {
			strncpy(last_os, outputs[i], sizeof(last_os) - 1);
			last_os[sizeof(last_os) - 1] = '\0';
		}
	}

	OSAscii os_list[64];
	char ascii_dir[512];
	const char *adir = resolve_ascii_dir(ascii_dir, sizeof(ascii_dir));
	int os_count = scan_ascii_dir(adir, os_list, 64);
	if (argc > 2 && strcasecmp(argv[2], "os") == 0) {
		printf("Available OS logos:\n\n");
		for (int i = 0; i < os_count; i++) {
			printf("  - %s\n", os_list[i].keyword);
		}
		return 0;
	}

	char ascii[50][512];
	int ascii_lines = 0;
	char logo_override[64] = {0};

	if (argc > 2) {
		snprintf(logo_override, sizeof(logo_override), "%s",
				argv[2]);
	} else {
		snprintf(logo_override, sizeof(logo_override), "%s",
				last_os);
	}

	for (int i = 0; i < os_count; i++) {
		if (strcasestr(logo_override, os_list[i].keyword)) {
			ascii_lines =
				load_ascii(os_list[i].ascii_path, ascii, 50);
			break;
		}
	}

	if (ascii_lines == 0) {
		printf("OS '%s' not found in sysinfo database.\n",
				logo_override);
		printf("Run 'sys fetch os' to see available logos.\n");
		printf("Falling back to system default...\n\n");

		if (strlen(last_os) > 0) {
			for (int i = 0; i < os_count; i++) {
				if (strcasestr(last_os, os_list[i].keyword)) {
					ascii_lines = load_ascii(
							os_list[i].ascii_path, ascii, 50);
					break;
				}
			}
		}
	}

	int max_lines = ascii_lines > n ? ascii_lines : n;
	for (int i = 0; i < max_lines; i++) {
		if (i < ascii_lines)
			printf("%s%-22s\033[0m", color_code, ascii[i]);
		else
			printf("%-22s", "");

		if (i < n) {
			char cmd_copy[64];
			strncpy(cmd_copy, commands[i], sizeof(cmd_copy) - 1);
			cmd_copy[sizeof(cmd_copy) - 1] = 0;
			char *first_word = strtok(cmd_copy, " ");
			const char *icon = (strlen(icons[i]) > 0)
				? icons[i]
				: get_icon(first_word);
			printf("%s %-14s • %s\n", icon, commands[i],
					outputs[i]);
		} else {
			printf("\n");
		}
	}

	return 0;
}

