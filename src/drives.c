#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int drives() {
    char mount_line[512];
    char df_result[512];

    FILE *fp = popen("mount", "r");
    if (!fp) {
        perror("popen mount");
        return 1;
    }

    printf("%-30s %-15s %-30s %s\n", "File System", "  Used/Total", "Usage", "%");

    while (fgets(mount_line, sizeof(mount_line), fp)) {
        mount_line[strcspn(mount_line, "\n")] = 0;

        char *on_pos = strstr(mount_line, " on ");
        if (!on_pos) continue;

        char device[128], mountpoint[256];
        *on_pos = '\0';
        strncpy(device, mount_line, sizeof(device)-1);
        device[sizeof(device)-1] = '\0';

        on_pos += 4;
        char *type_pos = strstr(on_pos, " type ");
        if (!type_pos) continue;
        *type_pos = '\0';
        strncpy(mountpoint, on_pos, sizeof(mountpoint)-1);
        mountpoint[sizeof(mountpoint)-1] = '\0';

        if (strncmp(device, "/dev/", 5) != 0)
            continue;

        char cmd[512];
        snprintf(cmd, sizeof(cmd), "df -B1 \"%s\" 2>/dev/null | tail -n1", mountpoint);

        FILE *dfp = popen(cmd, "r");
        if (!dfp) continue;

        if (fgets(df_result, sizeof(df_result), dfp)) {
            char fs_name[128], mount_str[128], usedp_str[8], avail_str[64];
            long long total_bytes, used_bytes, avail_bytes;
            float used_percent;

            int parsed = sscanf(df_result, "%s %lld %lld %lld %s %s",
                                fs_name, &total_bytes, &used_bytes, &avail_bytes,
                                usedp_str, mount_str);

            if (parsed == 6) {
                sscanf(usedp_str, "%f%%", &used_percent);

                int total_blocks = 30;
                int filled = (int)((used_percent / 100.0) * total_blocks);
                int empty  = total_blocks - filled;

                char blocks[64] = "";
                for (int i = 0; i < filled; i++) strcat(blocks, "▋");
                for (int i = 0; i < empty; i++) strcat(blocks, "_");

                char used_total[32];
                snprintf(used_total, sizeof(used_total), "%6.2f/%-6.2fGB",
                         used_bytes / (1024.0*1024*1024),
                         total_bytes / (1024.0*1024*1024));

                printf("%-30s %-15s [%-30s] %3.0f%%\n",
                       mountpoint,
                       used_total,
                       blocks,
                       used_percent);
            }
        }

        pclose(dfp);
    }

    pclose(fp);
    return 0;
}

