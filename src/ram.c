#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

void ram() {
	char buffer[256];
	FILE *fp = popen("cat /proc/meminfo", "r");
	float x_h, y_h;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "MemTotal:", 9) == 0) {
			char *x = buffer + 9;
			while (*x == ' ' || *x == '\t') x++;
			x_h = atol(x);
			x_h = x_h / 1048576;
			printf("total memory in ram : %0.3f GB\n", x_h);
		};
		if (strncmp(buffer, "MemAvailable:", 13) == 0) {
			char *y = buffer + 13;
			while (*y == ' ' || *y == '\t') y++;
			y_h = atol(y);
			y_h = y_h / 1048576;
			printf("memory left in ram : %0.3f GB\n", y_h);
		};
	};
	float used = x_h - y_h;
	printf("memory used in ram : %0.3f GB\n", used);
	pclose(fp);
};

void ram_total() {
	char buffer[256];
	FILE *fp = popen("cat /proc/meminfo", "r");
	float x_h;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "MemTotal:", 9) == 0) {
			char *x = buffer + 9;
			while (*x == ' ' || *x == '\t') x++;
			x_h = atol(x);
			x_h = x_h / 1048576;
			printf("total memory in ram : %0.3f GB\n", x_h);
		};
	};
	pclose(fp);
};

void ram_total_val() {
	char buffer[256];
	FILE *fp = popen("cat /proc/meminfo", "r");
	float x_h;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "MemTotal:", 9) == 0) {
			char *x = buffer + 9;
			while (*x == ' ' || *x == '\t') x++;
			x_h = atol(x);
			x_h = x_h / 1048576;
			printf("%0.3f GB\n", x_h);
		};
	};
	pclose(fp);
};

void ram_left() {
	char buffer[256];
	FILE *fp = popen("cat /proc/meminfo", "r");
	float y_h;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "MemAvailable:", 13) == 0) {
			char *y = buffer + 13;
			while (*y == ' ' || *y == '\t') y++;
			y_h = atol(y);
			y_h = y_h / 1048576;
			printf("memory left in ram : %0.3f GB\n", y_h);
		};
	};
	pclose(fp);
};

void ram_left_val() {
	char buffer[256];
	FILE *fp = popen("cat /proc/meminfo", "r");
	float y_h;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "MemAvailable:", 13) == 0) {
			char *y = buffer + 13;
			while (*y == ' ' || *y == '\t') y++;
			y_h = atol(y);
			y_h = y_h / 1048576;
			printf("%0.3f GB\n", y_h);
		};
	};
	pclose(fp);
};

void ram_used() {
	char buffer[256];
	FILE *fp = popen("cat /proc/meminfo", "r");
	float x_h, y_h;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "MemTotal:", 9) == 0) {
			char *x = buffer + 9;
			while (*x == ' ' || *x == '\t') x++;
			x_h = atol(x);
			x_h = x_h / 1048576;
		};
		if (strncmp(buffer, "MemAvailable:", 13) == 0) {
			char *y = buffer + 13;
			while (*y == ' ' || *y == '\t') y++;
			y_h = atol(y);
			y_h = y_h / 1048576;
		};
	};
	float used = x_h - y_h;
	printf("memory used in ram : %0.3f GB\n", used);
	pclose(fp);
};

void ram_used_val() {
	char buffer[256];
	FILE *fp = popen("cat /proc/meminfo", "r");
	float x_h, y_h;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		if (strncmp(buffer, "MemTotal:", 9) == 0) {
			char *x = buffer + 9;
			while (*x == ' ' || *x == '\t') x++;
			x_h = atol(x);
			x_h = x_h / 1048576;
		};
		if (strncmp(buffer, "MemAvailable:", 13) == 0) {
			char *y = buffer + 13;
			while (*y == ' ' || *y == '\t') y++;
			y_h = atol(y);
			y_h = y_h / 1048576;
		};
	};
	float used = x_h - y_h;
	printf("%0.3f GB\n", used);
	pclose(fp);
};

