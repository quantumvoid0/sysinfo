#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define INFO_COL 35

const char *current_cmd = NULL;

const char* get_icon(const char *cmd) {
    if (!cmd) return "";
    if (strcmp(cmd, "os") == 0) return "󰌽 ";
    if (strcmp(cmd, "kernel") == 0) return " ";
    if (strcmp(cmd, "host") == 0 || strcmp(cmd,"hostname")==0 || strcmp(cmd,"name")==0) return "󱩊 ";
    if (strcmp(cmd, "uptime") == 0 || strcmp(cmd,"up")==0) return " ";
    if (strcmp(cmd, "cpu") == 0 || strcmp(cmd,"cpu load")==0) return " ";
    if (strcmp(cmd, "cpucores") == 0 || strcmp(cmd,"cores")==0 || strcmp(cmd,"core")==0 || strcmp(cmd,"cpucore")==0)  return " ";
    if (strcmp(cmd, "cpufreq") == 0 || strcmp(cmd,"freq")==0 || strcmp(cmd,"cpufrequency")==0) return " ";
    if (strcmp(cmd, "ram") == 0 || strcmp(cmd,"mem")==0 || strcmp(cmd,"memory")==0)  return " ";
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
    return "";
}

const char* get_color(const char* name) {
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

typedef struct { const char *keyword; const char *ascii_path; } OSAscii;

int load_commands_from_config(char commands[][64], int max_commands, const char *config_path) {
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
            while (*start && (*start == ' ' || *start == '\"' || *start == ',')) start++;
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

int load_ascii_color_from_config(const char *config_path, char *color_name, int max_len) {
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

int is_known_os(const char *os, OSAscii os_list[], int os_count) {
    for (int i = 0; i < os_count; i++) {
        if (strcasecmp(os, os_list[i].keyword) == 0) return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    char config_path[512];
    const char *xdg = getenv("XDG_CONFIG_HOME");
    if (xdg && *xdg)
        snprintf(config_path, sizeof(config_path), "%s/sysinfo/config.json", xdg);
    else
        snprintf(config_path, sizeof(config_path), "%s/.config/sysinfo/config.json", getenv("HOME"));

    char commands[20][64];
    int n = load_commands_from_config(commands, 20, config_path);
    if (n == 0) {
        const char *default_commands[] = {"os","kernel","host","uptime","cpu","me","swap total","ram used","cores"};
        n = sizeof(default_commands)/sizeof(default_commands[0]);
        for(int i=0;i<n;i++) strncpy(commands[i], default_commands[i], 63);
    }

    char ascii_color[16] = "reset";
    load_ascii_color_from_config(config_path, ascii_color, sizeof(ascii_color));
    const char* color_code = get_color(ascii_color);

    char outputs[20][256];
    char last_os[256] = {0};

    if (argc > 1)
        current_cmd = argv[1];

    for (int i = 0; i < n; i++) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "sys %s", commands[i]);
        FILE *fp = popen(cmd, "r");
        if (!fp) continue;

        if (fgets(outputs[i], sizeof(outputs[i]), fp)) {
            outputs[i][strcspn(outputs[i], "\n")] = 0;
            if (strcmp(commands[i], "os") == 0)
                strncpy(last_os, outputs[i], sizeof(last_os)-1);
        }
        pclose(fp);
    }

    OSAscii os_list[] = {
        {"Gentoo", "/usr/share/sysinfo/gentoo.ascii"},
        {"Arch", "/usr/share/sysinfo/arch.ascii"},
        {"Fedora", "/usr/share/sysinfo/fedora.ascii"},
        {"Debian", "/usr/share/sysinfo/debian.ascii"},
        {"Mint", "/usr/share/sysinfo/mint.ascii"},
        {"Ubuntu", "/usr/share/sysinfo/ubuntu.ascii"},
        {"Manjaro","/usr/share/sysinfo/manjaro.ascii"},
        {"Pop","/usr/share/sysinfo/pop.ascii"},
        {"Zorin","/usr/share/sysinfo/zorin.ascii"},
        {"Elementary","/usr/share/sysinfo/elementary.ascii"},
        {"MX","/usr/share/sysinfo/mx.ascii"},
        {"Endeavour","/usr/share/sysinfo/endeavour.ascii"},
        {"Kali","/usr/share/sysinfo/kali.ascii"},
    };

    char ascii[50][512];
    int ascii_lines = 0;
    char logo_override[64] = {0};
    if (argc > 1) strncpy(logo_override, argv[1], sizeof(logo_override)-1);

    int os_count = sizeof(os_list)/sizeof(os_list[0]);
    if (strlen(logo_override)) {
        for (int i = 0; i < os_count; i++) {
            if (strcasecmp(logo_override, os_list[i].keyword) == 0) {
                ascii_lines = load_ascii(os_list[i].ascii_path, ascii, 50);
                break;
            }
        }
    }

    if (ascii_lines == 0) {
        for (int i = 0; i < os_count; i++) {
            if (strstr(last_os, os_list[i].keyword)) {
                ascii_lines = load_ascii(os_list[i].ascii_path, ascii, 50);
                break;
            }
        }
    }

    int max_lines = ascii_lines > n ? ascii_lines : n;
    for (int i = 0; i < max_lines; i++) {
        // ASCII with color
        if (i < ascii_lines)
            printf("%s%-22s\033[0m", color_code, ascii[i]);
        else
            printf("%-22s", "");

        // Command output plain
        if (i < n) {
            char cmd_copy[64];
            strncpy(cmd_copy, commands[i], sizeof(cmd_copy)-1);
            cmd_copy[sizeof(cmd_copy)-1] = 0;
            char *first_word = strtok(cmd_copy, " ");
            printf("%s %-14s • %s\n", get_icon(first_word), commands[i], outputs[i]);
        } else {
            printf("\n");
        }
    }

    if (argc > 1) {
        if (!is_known_os(argv[1], os_list, os_count)) {
            printf("Usage: %s [OS]\n", argv[0]);
            printf("Available OS options:\n");
            for (int i = 0; i < os_count; i++)
                printf("  %s\n", os_list[i].keyword);
            return 1;
        }
    }

    return 0;
}

