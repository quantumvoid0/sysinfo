#ifndef FETCH_H
#define FETCH_H

typedef struct {
	char keyword[64];
	char ascii_path[512];
} OSAscii;

int load_ascii_color_from_config(const char *config_path, char *color_name,
				 int max_len);
int load_icons_from_config(const char *config_path, char commands[][64],
			   char icons[][32], int max_commands);
int load_commands_from_config(char commands[][64], int max_commands,
			      const char *config_path);
int scan_ascii_dir(const char *dir_path, OSAscii os_list[], int max_os);
int load_ascii(const char *path, char ascii[][512], int max_lines);

#endif
