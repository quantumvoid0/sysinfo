#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h" 

void swap() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    char checkswap[256];
    FILE *cs = popen("swapon","r");
    float x_h,y_h;
    
    while (fgets(checkswap,sizeof(checkswap),cs) == 0) {
        printf("You dont have a swap partition\n\n");
        break;
        return;
    };

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapTotal:",10) == 0) {
            char *x = buffer + 10;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;
            printf("total memory is swap : %0.3f GB\n",x_h);

        };

        if (strncmp(buffer,"SwapFree:",9) == 0) {
            char *y = buffer + 9;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
            printf("memory left in swap : %0.3f GB\n",y_h);

        };
    };    
    float used = x_h - y_h;
    printf("memory used in swap : %0.3f GB\n",used);
    pclose(cs);
    pclose(fp);
};

void swap_total() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h;
    
    char checkswap[256];
    FILE *cs = popen("swapon","r");
    while (fgets(checkswap,sizeof(checkswap),cs) == 0) {
        printf("You dont have a swap partition\n\n");
        break;
        return;
    };

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapTotal:",10) == 0) {
            char *x = buffer + 10;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;
            printf("total memory in swap : %0.3f GB\n",x_h);
        };
    };    
    pclose(fp);
};

void swap_left() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float y_h;

    char checkswap[256];
    FILE *cs = popen("swapon","r");
    while (fgets(checkswap,sizeof(checkswap),cs) == 0) {
        printf("You dont have a swap partition\n\n");
        break;
        return;
    };


    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapFree:",9) == 0) {
            char *y = buffer + 9;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
            printf("memory left in swap : %0.3f GB\n",y_h);
        };
    };    
    pclose(fp);
};

void swap_used() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h,y_h;

    char checkswap[256];
    FILE *cs = popen("swapon","r");
    while (fgets(checkswap,sizeof(checkswap),cs) == 0) {
        printf("You dont have a swap partition\n\n");
        break;
        return;
    };


    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapTotal:",10) == 0) {
            char *x = buffer + 10;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;        
        };
        if (strncmp(buffer,"SwapFree:",9) == 0) {
            char *y = buffer + 9;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
        };
    };    
    float used = x_h - y_h;
    printf("memory used in swap : %0.3f GB\n",used);
    pclose(fp);
};

void swap_total_val() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h;
    
    char checkswap[256];
    FILE *cs = popen("swapon","r");
    while (fgets(checkswap,sizeof(checkswap),cs) == 0) {
        printf("You dont have a swap partition\n\n");
        break;
        return;
    };

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapTotal:",10) == 0) {
            char *x = buffer + 10;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;
            printf("%0.3f GB\n",x_h);
        };
    };    
    pclose(fp);
};

void swap_left_val() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float y_h;

    char checkswap[256];
    FILE *cs = popen("swapon","r");
    while (fgets(checkswap,sizeof(checkswap),cs) == 0) {
        printf("You dont have a swap partition\n\n");
        break;
        return;
    };


    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapFree:",9) == 0) {
            char *y = buffer + 9;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
            printf("%0.3f GB\n",y_h);
        };
    };    
    pclose(fp);
};

void swap_used_val() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h,y_h;

    char checkswap[256];
    FILE *cs = popen("swapon","r");
    while (fgets(checkswap,sizeof(checkswap),cs) == 0) {
        printf("You dont have a swap partition\n\n");
        break;
        return;
    };


    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapTotal:",10) == 0) {
            char *x = buffer + 10;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;        
        };
        if (strncmp(buffer,"SwapFree:",9) == 0) {
            char *y = buffer + 9;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
        };
    };    
    float used = x_h - y_h;
    printf("%0.3f GB\n",used);
    pclose(fp);
};
