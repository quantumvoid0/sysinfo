#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INFO_COL 35

const char *forbidden[] = {"fetch",  "proc", "update",
			   "drives", "live", "verson-latest"};
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
		return " ";
	if (strcmp(cmd, "ram") == 0 || strcmp(cmd, "mem") == 0 ||
	    strcmp(cmd, "memory") == 0)
		return " ";
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

int load_ascii(const char *path, char ascii[][512], int max_lines) {
	FILE *fp = fopen(path, "r");
	if (!fp) return 0;
	int count = 0;
	while (fgets(ascii[count], 512, fp) && count < max_lines) {
		ascii[count][strcspn(ascii[count], "\n")] = 0;
		count++;
	}
	fclose(fp);
	return count;
}

typedef struct {
	char keyword[64];
	char ascii_path[512];
} OSAscii;

int scan_ascii_dir(const char *dir_path, OSAscii os_list[], int max_os) {
	DIR *d = opendir(dir_path);
	if (!d) return 0;
	struct dirent *entry;
	int count = 0;
	while ((entry = readdir(d)) != NULL && count < max_os) {
		if (strstr(entry->d_name, ".ascii")) {
			snprintf(os_list[count].ascii_path,
				 sizeof(os_list[count].ascii_path), "%s/%s",
				 dir_path, entry->d_name);

			strncpy(os_list[count].keyword, entry->d_name,
				sizeof(os_list[count].keyword) - 1);
			os_list[count]
			    .keyword[sizeof(os_list[count].keyword) - 1] = 0;

			char *dot = strrchr(os_list[count].keyword, '.');
			if (dot) *dot = 0;

			os_list[count].keyword[0] =
			    toupper(os_list[count].keyword[0]);
			count++;
		}
	}
	closedir(d);
	return count;
}

int load_commands_from_config(char commands[][64], int max_commands,
			      const char *config_path) {
	FILE *fp = fopen(config_path, "r");
	if (!fp) return 0;
	char line[256];
	int count = 0;
	while (fgets(line, sizeof(line), fp) && count < max_commands) {
		char *start = strstr(line, "\"commands\"");
		if (!start) continue;
		start = strchr(line, '[');
		if (!start) continue;
		start++;
		while (*start && count < max_commands) {
			while (*start && (*start == ' ' || *start == '\"' ||
					  *start == ','))
				start++;
			if (!*start || *start == ']') break;
			char *end = strpbrk(start, "\",]");
			if (!end) break;
			int len = end - start;
			if (len > 63) len = 63;
			strncpy(commands[count], start, len);
			commands[count][len] = 0;
			count++;
			start = end + 1;
		}
		break;
	}
	fclose(fp);
	return count;
}

char icons[20][32];   
int load_icons_from_config(const char *config_path, char commands[][64], char icons[][32], int max_commands) {
    FILE *fp = fopen(config_path, "r");
    if (!fp) return 0;

    char line[256];
    int in_icons = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (!in_icons) {
            if (strstr(line, "\"icons\"")) {
                in_icons = 1;
            }
            continue;
        }

        if (strchr(line, '}')) break;

        for (int i = 0; i < max_commands; i++) {
            if (strlen(commands[i]) == 0) continue;

            char pattern[128];
            snprintf(pattern, sizeof(pattern), "\"%s\"", commands[i]);
            char *cmdpos = strstr(line, pattern);
            if (cmdpos) {
                char *colon = strchr(cmdpos, ':');
                if (colon) {
                    colon++;
                    while (*colon == ' ' || *colon == '\"') colon++;
                    char *end = strpbrk(colon, "\",}");
                    if (end) {
                        int len = end - colon;
                        if (len > 31) len = 31;
                        strncpy(icons[i], colon, len);
                        icons[i][len] = ' ';
			icons[i][len + 1] = 0;
                    }
                }
            }
        }
    }

    fclose(fp);
    return 1;
}

int load_ascii_color_from_config(const char *config_path, char *color_name,
				 int max_len) {
	FILE *fp = fopen(config_path, "r");
	if (!fp) return 0;
	char line[256];
	while (fgets(line, sizeof(line), fp)) {
		char *start = strstr(line, "\"ascii\"");
		if (start) {
			start = strchr(line, ':');
			if (start) {
				start++;
				while (*start == ' ' || *start == '\"') start++;
				char *end = strpbrk(start, "\",");
				if (end) {
					int len = end - start;
					if (len >= max_len) len = max_len - 1;
					strncpy(color_name, start, len);
					color_name[len] = 0;
					fclose(fp);
					return 1;
				}
			}
		}
	}
	fclose(fp);
	return 0;
}

int main(int argc, char **argv) {
	char config_path[512];
	const char *xdg = getenv("XDG_CONFIG_HOME");
	if (xdg && *xdg)
		snprintf(config_path, sizeof(config_path),
			 "%s/sysinfo/config.json", xdg);
	else
		snprintf(config_path, sizeof(config_path),
			 "%s/.config/sysinfo/config.json", getenv("HOME"));

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

	char outputs[20][256];
	char last_os[256] = {0};
	char icons[20][32] = {{0}};
	load_icons_from_config(config_path, commands, icons, 20);


	{
		FILE *fp = popen("sys os", "r");
		if (fp && fgets(last_os, sizeof(last_os), fp)) {
			last_os[strcspn(last_os, "\n")] = 0;
		}
		if (fp) pclose(fp);
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

		char cmd[256];
		snprintf(cmd, sizeof(cmd), "sys %s", commands[i]);
		FILE *fp = popen(cmd, "r");
		if (!fp) continue;

		if (fgets(outputs[i], sizeof(outputs[i]), fp)) {
			outputs[i][strcspn(outputs[i], "\n")] = 0;
			if (strcmp(commands[i], "os") == 0)
				strncpy(last_os, outputs[i],
					sizeof(last_os) - 1);
		}
		pclose(fp);
	}

	OSAscii os_list[64];
	int os_count = scan_ascii_dir("/usr/share/sysinfo", os_list, 64);
	if (argc > 1 && strcasecmp(argv[1], "os") == 0) {
		printf("Available OS logos:\n\n");
		for (int i = 0; i < os_count; i++) {
			printf("  - %s\n", os_list[i].keyword);
		}
		return 0;
	}

	char ascii[50][512];
	int ascii_lines = 0;
	char logo_override[64] = {0};

	if (argc > 1) {
		strncpy(logo_override, argv[1], sizeof(logo_override) - 1);
	} else {
		strncpy(logo_override, last_os, sizeof(logo_override) - 1);
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
			const char *icon = (strlen(icons[i]) > 0) ? icons[i] : get_icon(first_word);
			printf("%s %-14s • %s\n", icon, commands[i], outputs[i]);
		} else {
			printf("\n");
		}
	}

	return 0;
}

