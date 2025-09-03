#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h" 

void os() {
    char buffer[256];
    FILE *fp = popen("cat /etc/os-release", "r");

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {

        if (strncmp(buffer,"PRETTY_NAME",11) == 0) {
            char *x = buffer + 11;
            if (x[0] == '=') x++;
            if (x[0] == '"') x++;
            size_t len = strlen(x);
            if (x[len - 2] == '"') {
                x[len - 2] = '\0';
            };
            printf("%s\n",x);
            break;
        };
    };
    pclose(fp);
};

void kernel() {
    char buffer[256];
    FILE *fp = popen("uname -r","r");
    fgets(buffer,sizeof(buffer),fp);
    printf("%s",buffer);
    pclose(fp);
};

void hostname() {
    char buffer[256];
    FILE *fp = popen("uname -n","r");
    fgets(buffer,sizeof(buffer),fp);
    printf("%s",buffer);
    pclose(fp);
};

void uptime() {
    char buffer[256];
    FILE *fp = popen("uptime -p","r");

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"up ",3) == 0) {
            char *x = buffer + 3;     
            printf("%s",x);
            break;
        };
    };
    pclose(fp);
};





void root() {
    char buffer[256];
    FILE *fp = popen("df -h /","r");
    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        printf("%s",buffer);
    };
    pclose(fp);
};




void me() {
    char buffer[256];
    FILE *fp = popen("whoami","r");
    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        printf("%s",buffer);
    };
    pclose(fp);
};

