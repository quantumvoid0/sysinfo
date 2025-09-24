#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fetch.h"

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

int load_icons_from_config(const char *config_path, char commands[][64],
			   char icons[][32], int max_commands) {
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
			snprintf(pattern, sizeof(pattern), "\"%s\"",
				 commands[i]);
			char *cmdpos = strstr(line, pattern);
			if (cmdpos) {
				char *colon = strchr(cmdpos, ':');
				if (colon) {
					colon++;
					while (*colon == ' ' || *colon == '\"')
						colon++;
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
