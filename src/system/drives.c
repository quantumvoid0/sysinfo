#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>

#include "functions.h"

int drives(char *out, size_t sz, int argc, char **argv) {
	FILE *mtab = setmntent("/proc/mounts", "r");
	if (!mtab) {
		snprintf(out, sz, "setmntent failed\n");
		return 1;
	}

	size_t off = 0;
	off = out_append(out, off, sz,
			"%-30s %-15s %-30s %s\n", "File System",
			"  Used/Total", "Usage", "%");

	struct mntent *mnt;
	while ((mnt = getmntent(mtab)) != NULL) {
		if (strncmp(mnt->mnt_fsname, "/dev/", 5) != 0) continue;

		struct statvfs st;
		if (statvfs(mnt->mnt_dir, &st) != 0) continue;

		unsigned long long total =
			(unsigned long long)st.f_blocks * st.f_frsize;
		unsigned long long avail =
			(unsigned long long)st.f_bavail * st.f_frsize;
		unsigned long long used = total - avail;
		if (total == 0) continue;

		int percent = (int)((used * 100) / total);

		int total_blocks = 30;
		int filled = (percent * total_blocks) / 100;
		int empty = total_blocks - filled;

		char blocks[128] = "";
		for (int i = 0; i < filled; i++) strncat(blocks, "▋", sizeof(blocks) - strlen(blocks) - 1);
		for (int i = 0; i < empty; i++) strncat(blocks, "_", sizeof(blocks) - strlen(blocks) - 1);

		char used_total[32];
		snprintf(used_total, sizeof(used_total), "%6.2f/%-6.2fGB",
				used / (1024.0 * 1024 * 1024),
				total / (1024.0 * 1024 * 1024));

		off = out_append(out, off, sz,
				"%-30s %-15s [%-30s] %3d%%\n", mnt->mnt_dir,
				used_total, blocks, percent);
	}

	endmntent(mtab);
	return 0;
}

