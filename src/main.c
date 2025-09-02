#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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

void cpu() {
    char buffer[256];
    FILE *fp = popen("cat /proc/cpuinfo","r");

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"model name	: ",13) == 0) {
            char *x = buffer + 13;
            printf("%s",x);
            break;
        };
    };
    pclose(fp);
};

void cpucores() {
    char buffer[256];
    FILE *fp = popen("nproc","r");
    fgets(buffer,sizeof(buffer),fp);
    printf("%s",buffer);
    pclose(fp);
};

void cpufreq() {
    char buffer[256];
    FILE *fp = popen("cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq","r");
    int i = 1;
    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        char core[256];
        FILE *cr = popen("nproc","r");
        fgets(core,sizeof(core),cr);
        float buffer_h = atol(buffer);
        buffer_h = buffer_h/1000;
        printf("frequncy of core %d : %0.3f Mhz\n",i,buffer_h);
        i++;
    };
    pclose(fp);
};

void ram() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h,y_h;

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"MemTotal:",9) == 0) {
            char *x = buffer + 9;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;
            printf("total memory in ram : %0.3f GB\n",x_h);
        };
        if (strncmp(buffer,"MemAvailable:",13) == 0) {
            char *y = buffer + 13;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
            printf("memory left in ram : %0.3f GB\n",y_h);
        };
    };    
    float used = x_h - y_h;
    printf("memory used in ram : %0.3f GB\n",used);    
    pclose(fp);
};

void swap() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h,y_h;

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"SwapTotal:",10) == 0) {
            char *x = buffer + 10;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;
            printf("total memory in swap : %0.3f GB\n",x_h);
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

void ram_total() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h;

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"MemTotal:",9) == 0) {
            char *x = buffer + 9;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;
            printf("total memory in ram : %0.3f GB\n",x_h);
        };
    };
    pclose(fp);
};

void ram_left() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float y_h;

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"MemAvailable:",13) == 0) {
            char *y = buffer + 13;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
            printf("memory left in ram : %0.3f GB\n",y_h);
        };
    };    
    pclose(fp);
};

void ram_used() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h,y_h;

    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        if (strncmp(buffer,"MemTotal:",9) == 0) {
            char *x = buffer + 9;
            while (*x == ' ' || *x == '\t') x++;
            x_h = atol(x);
            x_h = x_h/1048576;
        };
        if (strncmp(buffer,"MemAvailable:",13) == 0) {
            char *y = buffer + 13;
            while (*y == ' ' || *y == '\t') y++;
            y_h = atol(y);
            y_h = y_h/1048576;
        };
    };    
    float used = x_h - y_h;
    printf("memory used in ram : %0.3f GB\n",used);
    pclose(fp);
};

void swap_total() {
    char buffer[256];
    FILE *fp = popen("cat /proc/meminfo","r");
    float x_h;

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

void me() {
    char buffer[256];
    FILE *fp = popen("whoami","r");
    while (fgets(buffer,sizeof(buffer),fp) != NULL) {
        printf("%s",buffer);
    };
    pclose(fp);
};

#define CORES 256

typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} cpu_stat;

void read_cpu_stats(cpu_stat stats[], int cores) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return;

    char line[256];
    int core_index = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "cpu", 3) != 0) break;

        if (line[3] >= '0' && line[3] <= '9') { 
            sscanf(line, "cpu%*d %llu %llu %llu %llu %llu %llu %llu %llu",
                &stats[core_index].user,
                &stats[core_index].nice,
                &stats[core_index].system,
                &stats[core_index].idle,
                &stats[core_index].iowait,
                &stats[core_index].irq,
                &stats[core_index].softirq,
                &stats[core_index].steal
            );
            core_index++;
            if (core_index >= cores) break;
        }
    }
    fclose(fp);
}

void load() {
    cpu_stat prev[CORES], curr[CORES];
    int i, cores = sysconf(_SC_NPROCESSORS_ONLN);

    read_cpu_stats(prev, cores);
    usleep(100000); 
    read_cpu_stats(curr, cores);

    for (i = 0; i < cores; i++) {
        unsigned long long prev_idle = prev[i].idle + prev[i].iowait;
        unsigned long long curr_idle = curr[i].idle + curr[i].iowait;

        unsigned long long prev_total = prev[i].user + prev[i].nice + prev[i].system + prev[i].idle + prev[i].iowait + prev[i].irq + prev[i].softirq + prev[i].steal;
        unsigned long long curr_total = curr[i].user + curr[i].nice + curr[i].system + curr[i].idle + curr[i].iowait + curr[i].irq + curr[i].softirq + curr[i].steal;

        unsigned long long total_diff = curr_total - prev_total;
        unsigned long long idle_diff = curr_idle - prev_idle;

        double usage = (double)(total_diff - idle_diff) / total_diff * 100.0;
        printf("Core %d: %.2f%%\n", i, usage);
    }
}

//void live();

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
