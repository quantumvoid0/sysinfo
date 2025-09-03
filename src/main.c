#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("\nUsage: sys <arg1> [arg2]\n\n");
        printf("Commands: <arg1>\n");
        printf("  os       : operating system name\n");
        printf("  kernel   : kernel info\n");
        printf("  host     : hostname\n");
        printf("  uptime   : system uptime\n");
        printf("  cpu      : cpu info\n");
        printf("  cpucores : number of cpu cores\n");
        printf("  cpufreq  : cpu frequency per core\n");
        printf("  ram      : ram info\n");
        printf("  swap     : swap info\n");
        printf("  root     : root partition info\n");
        printf("  live     : live info (beta)\n");     
        printf("  me       : user info\n\n");
        printf("Commands: [arg2]\n");        
        printf("  ram [total/used/left]      : ram info\n");
        printf("  swap [total/used/left]     : swap info\n");
        printf("  cpu [load]                 : cpu load info\n\n");
        printf("Examples:\n");
        printf("  sys os\n");
        printf("  sys ram used\n\n");

        return 1;
    }

    if (strcmp(argv[1],"os") == 0) {
        os();
    };
    if (strcmp(argv[1],"kernel") == 0) {
        kernel();
    };
    if (strcmp(argv[1],"host") == 0 || strcmp(argv[1],"hostname") == 0 || strcmp(argv[1],"name") == 0)  {
        hostname();
    };
    if (strcmp(argv[1],"uptime") == 0 || strcmp(argv[1],"up") == 0 || strcmp(argv[1],"time") == 0) {
        uptime();
    };
    if (strcmp(argv[1],"cpu") == 0) {
        if (argc<=2) {
            cpu();
        } else if (strcmp(argv[2],"load") == 0) {
            load();
        };
    };

    if (strcmp(argv[1],"cpucores") == 0 || strcmp(argv[1],"cores") == 0 || strcmp(argv[1],"core") == 0 || strcmp(argv[1],"cpucore") == 0) {
        cpucores();
    };
    if (strcmp(argv[1],"cpufreq") == 0 || strcmp(argv[1],"freq") == 0 || strcmp(argv[1],"cpufrequency") == 0 || strcmp(argv[1],"frequency") == 0) {
        cpufreq();
    };
    if (strcmp(argv[1],"mem") == 0 || strcmp(argv[1],"memory") == 0 || strcmp(argv[1],"ram") == 0) {
        if (argc <= 2) {
            ram();                
        } else if (strcmp(argv[2],"total") == 0) {
            ram_total();          
        } else if (strcmp(argv[2],"left") == 0) {
            ram_left();          
        } else if (strcmp(argv[2],"used") == 0) {
            ram_used();          
        };

    };
    if (strcmp(argv[1],"swap") == 0) {
        if (argc <= 2) {
            swap();                
        } else if (strcmp(argv[2],"total") == 0) {
            swap_total();          
        } else if (strcmp(argv[2],"left") == 0) {
            swap_left();          
        } else if (strcmp(argv[2],"used") == 0) {
            swap_used();          
        };

    };
    if (strcmp(argv[1],"root") == 0) {
        root();
    }; 

    if (strcmp(argv[1],"me") == 0) {
        me(); 
    };
    
    if (strcmp(argv[1],"live") == 0) {
        system("live");
    };
    

    return 0;
};
