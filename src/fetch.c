#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INFO_COL 35

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

    char outputs[20][256];
    char last_os[256] = {0};
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
        if (i < ascii_lines) printf("%-22s", ascii[i]);
        else printf("%-22s", "");
        if (i < n) printf("%-14s : %s\n", commands[i], outputs[i]);
        else printf("\n");
    }

    return 0;
}

