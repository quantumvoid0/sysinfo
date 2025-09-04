#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "functions.h" 

void show_time(int mode) {
    time_t rawtime;
    struct tm *t;

    time(&rawtime);
    t = localtime(&rawtime);

    if (mode == 12) {
        int hour = t->tm_hour % 12;
        if (hour == 0) hour = 12;
        printf("%02d:%02d:%02d %s\n",
               hour,
               t->tm_min,
               t->tm_sec,
               (t->tm_hour < 12) ? "AM" : "PM");
    } else if (mode == 24) {
        printf("%02d:%02d:%02d\n",
               t->tm_hour,
               t->tm_min,
               t->tm_sec);
    } else {
        printf("Invalid mode (use 12 or 24)\n");
    }
}
