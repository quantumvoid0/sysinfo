#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("\nUsage: sys <arg1> [arg2] [arg3]\n\n");
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
        printf("  fetch    : fetches sys info (beta)\n");
        printf("  time     : time\n");
        printf("  date     : date\n");
        printf("  shell    : shell name\n");
        printf("  term     : terminal name\n");
        printf("  ip       : local ip\n");
        printf("  bios     : bios info\n");
        printf("  system   : system info\n");
        printf("  board    : motherboard info\n");
        printf("  init     : init system info\n");
        printf("  proc     : list all processes\n");
        printf("  ping     : check latency\n");
        printf("  editor   : editor name (only if $EDITOR var is set)\n");
 	printf("  locale   : returns locale\n");
 	printf("  language : returns language\n");
	printf("  region   : returns region\n");
	printf("  encoding : returns encoding\n");
        printf("  me       : user info\n\n");
     	printf("  update         : update current version to the latest\n");
	printf("  version  	 : installed sysinfo version\n");
	printf("  version-latest : latest available sysinfo version\n\n");
        printf("Commands: [arg2]\n");        
        printf("  ram [total/used/left]      		: ram info\n");
        printf("  swap [total/used/left]     		: swap info\n");
        printf("  cpu [load]                 		: cpu load info\n");
        printf("  cpufreq (core number)      		: cpu freq by core number\n");
        printf("  bios [vendor/version/date/release]	: bios info\n");
        printf("  system [family/name/sku/version]      : system family info\n");
        printf("  board [tag/name/vendor/version]       : motherboard info\n");
        printf("  proc [num]                 		: list number of processes\n");
        printf("  time [12/24]               		: time\n");
        printf("  date [1/2/3]              		: date\n\n");
        printf("Commands: [arg3]\n");        
        printf("  ram [total/used/left] (val)      : ram info with only values\n");
        printf("  swap [total/used/left] (val)     : swap info with only values\n");
        printf("  cpu load (core number)           : cpu load info\n");
        printf("Examples:\n");
        printf("  sys os\n");
        printf("  sys swap left val\n");
        printf("  sys ram used\n\n");
        printf("fetch is configured in ~/.config/sysinfo/config.json\n\n");

        return 1;
    }

    if (strcmp(argv[1],"editor") == 0) {
    	system("echo $EDITOR");
    };

    if (strcmp(argv[1],"language") == 0) {
    	system("echo $LANG | cut -d_ -f1");
    };

    if (strcmp(argv[1],"region") == 0) {
    	system("echo $LANG | cut -d_ -f2 | cut -d. -f1");
    };

    if (strcmp(argv[1],"encoding") == 0) {
    	system("echo $LANG | cut -d. -f2");
    };

    if (strcmp(argv[1],"locale") == 0) {
    	system("echo $LANG");
    };

    if (strcmp(argv[1],"os") == 0) {
        os();
    };
    if (strcmp(argv[1],"kernel") == 0) {
        kernel();
    };
    if (strcmp(argv[1],"host") == 0 || strcmp(argv[1],"hostname") == 0 || strcmp(argv[1],"name") == 0)  {
        hostname();
    };
    if (strcmp(argv[1],"uptime") == 0 || strcmp(argv[1],"up") == 0) {
        uptime();
    };
    if (strcmp(argv[1],"cpu") == 0) {
        if (argc == 2) {
            cpu();
        } 
        else if (argc >= 3 && strcmp(argv[2],"load") == 0) {
            if (argc == 3) {
                load();  
            } 
            else if (argc == 4) {
                int core = atoi(argv[3]);
                load_core(core); 
            } 

        } else {
            printf("usage : cpu [load]\nusage : cpu load (core number)\nfor (core number) , actually enter a number starting from 0\n");
        };
    };  


    if (strcmp(argv[1],"cpucores") == 0 || strcmp(argv[1],"cores") == 0 || strcmp(argv[1],"core") == 0 || strcmp(argv[1],"cpucore") == 0) {
        cpucores();
    };
    if (strcmp(argv[1],"cpufreq") == 0 || strcmp(argv[1],"freq") == 0 || strcmp(argv[1],"cpufrequency") == 0 || strcmp(argv[1],"frequency") == 0) {
        if (argc == 3) {
            int core = atoi(argv[2]);
            float freq = get_core_freq(core);
            if (freq > 0)
                printf("Core %d frequency: %.3f MHz\n", core, freq);
            else
                printf("Failed to read core %d frequency\n", core);
        } else {
            cpufreq();   //fallback 
        }
    }
    if (strcmp(argv[1], "mem") == 0 || strcmp(argv[1], "memory") == 0 || strcmp(argv[1], "ram") == 0) {
        if (argc == 2) {
            ram();
        }
        else if (argc >= 3) {
            if (strcmp(argv[2], "total") == 0) {
                if (argc == 3) {
                    ram_total();
                } else if (argc == 4 && strcmp(argv[3], "val") == 0) {
                    ram_total_val();
                } else {
                     printf("usage : sys ram total val\n");
                };
            } 
            else if (strcmp(argv[2], "left") == 0) {
                if (argc == 3) {
                    ram_left();
                } else if (argc == 4 && strcmp(argv[3], "val") == 0) {
                    ram_left_val();
                } else {
                     printf("usage : sys ram left val\n");
                };
            } 
            else if (strcmp(argv[2], "used") == 0) {
                if (argc == 3) {
                    ram_used();
                } else if (argc == 4 && strcmp(argv[3], "val") == 0) {
                    ram_used_val();
                } else {
                     printf("usage : sys ram used val\n");
                };
            } else {
                printf("usage : sys ram [total/left/used]\nusage : sys ram [total/left/used] val\n");
            }
        }
    }
 
    if (strcmp(argv[1], "swap") == 0) {
        if (argc == 2) {
            swap();
        }
        else if (argc >= 3) {
            if (strcmp(argv[2], "total") == 0) {
                if (argc == 3) {
                    swap_total();
                } else if (argc == 4 && strcmp(argv[3], "val") == 0) {
                    swap_total_val();
                } else {
                     printf("usage : sys swap total val\n");
                }
            } 
            else if (strcmp(argv[2], "left") == 0) {
                if (argc == 3) {
                    swap_left();
                } else if (argc == 4 && strcmp(argv[3], "val") == 0) {
                    swap_left_val();
                } else {
                     printf("usage : sys swap left val\n");
                }
            } 
            else if (strcmp(argv[2], "used") == 0) {
                if (argc == 3) {
                    swap_used();
                } else if (argc == 4 && strcmp(argv[3], "val") == 0) {
                    swap_used_val();
                } else {
                     printf("usage : sys swap used val\n");
                }
            } else {
                printf("usage : sys swap [total/left/used]\nusage : sys ram [total/left/used] val\n");
            }
        }
    }

    if (strcmp(argv[1],"root") == 0) {
        root();
    }; 

    if (strcmp(argv[1],"me") == 0) {
        me(); 
    };
    
    if (strcmp(argv[1],"live") == 0) {
        system("live");
    };

    if (argc >= 2 && strcmp(argv[1], "time") == 0) {
        if (argc == 2) {
            show_time(24);
        } else if (strcmp(argv[2], "12") == 0) {
            show_time(12);
        } else if (strcmp(argv[2], "24") == 0) {
            show_time(24);
        } else {
            printf("Usage: %s time [12|24]\n", argv[0]);
        }
    }

    if (argc >= 2 && strcmp(argv[1], "date") == 0) {
        int format = 1; // default: DD-MM-YYYY
        if (argc >= 3) {
            if (strcmp(argv[2], "1") == 0) format = 1;          // DD-MM-YYYY
            else if (strcmp(argv[2], "2") == 0) format = 2;     // MM-DD-YYYY
            else if (strcmp(argv[2], "3") == 0) format = 3;     // YYYY-MM-DD
            else {
                printf("Usage: %s date [1|2|3]\n", argv[0]);
                return 1;
            }
        }
        show_date(format);
    }


    if (strcmp(argv[1], "fetch") == 0) {
        char cmd[256] = "fetch";
        for (int i = 2; i < argc; i++) {
            strncat(cmd, " ", sizeof(cmd)-strlen(cmd)-1);
            strncat(cmd, argv[i], sizeof(cmd)-strlen(cmd)-1);
        }
        system(cmd);
    }

    if (strcmp(argv[1],"shell") == 0) {
        system("echo ${SHELL##*/}");
    };
    if (strcmp(argv[1],"term") == 0) {
        system("echo $TERM");
    };
    if (strcmp(argv[1],"ip") == 0) {
        system("ip route get 1.1.1.1 | awk '{print $7}'");
    };
    if (strcmp(argv[1], "bios") == 0) {
        if (argc == 2) {
            system("cat /sys/devices/virtual/dmi/id/bios_vendor");
        } else if (argc > 2) {
            if (strcmp(argv[2], "vendor") == 0) {
                system("cat /sys/devices/virtual/dmi/id/bios_vendor");
            } else if (strcmp(argv[2], "version") == 0) {
                system("cat /sys/devices/virtual/dmi/id/bios_version");
            } else if (strcmp(argv[2], "date") == 0) {
                system("cat /sys/devices/virtual/dmi/id/bios_date");
            } else if (strcmp(argv[2], "release") == 0) {
                system("cat /sys/devices/virtual/dmi/id/bios_release");
	    } else {
                printf("Usage: sys bios [vendor/version/date/release]\n", argv[0]);
            }
        }
    }
    if (strcmp(argv[1], "system") == 0) {
        if (argc == 2) {
            system("cat /sys/devices/virtual/dmi/id/product_name");
        } else if (argc > 2) {
            if (strcmp(argv[2], "family") == 0) {
                system("cat /sys/devices/virtual/dmi/id/product_family");
            } else if (strcmp(argv[2], "name") == 0) {
                system("cat /sys/devices/virtual/dmi/id/product_name");
            } else if (strcmp(argv[2], "sku") == 0) {
                system("cat /sys/devices/virtual/dmi/id/product_sku");
            } else if (strcmp(argv[2], "version") == 0) {
                system("cat /sys/devices/virtual/dmi/id/product_version");
            }
	    else {
                printf("Usage: sys system [family/name/sku/version]\n", argv[0]);
        }
        } 
    }
    if (strcmp(argv[1], "board") == 0) {
        if (argc == 2) {
            system("cat /sys/devices/virtual/dmi/id/board_vendor");
        } else if (argc > 2) {
            if (strcmp(argv[2], "tag") == 0) {
                system("cat /sys/devices/virtual/dmi/id/board_asset_tag");
            } else if (strcmp(argv[2], "name") == 0) {
                system("cat /sys/devices/virtual/dmi/id/board_name");
            } else  if (strcmp(argv[2], "vendor") == 0) {
                system("cat /sys/devices/virtual/dmi/id/board_vendor");
            } else if (strcmp(argv[2], "version") == 0) {
                system("cat /sys/devices/virtual/dmi/id/board_version");
            } else {
                printf("Usage: sys board [tag/name/vendor/version]\n", argv[0]);
        }
        }
    }
    if (strcmp(argv[1], "init") == 0) {
        system("ps -p 1 -o comm=");
    }
    if (strcmp(argv[1], "ps") == 0 || strcmp(argv[1], "proc") == 0) {
        if (argc == 2) {
            system("ps -eo pid,comm --sort=pid");
        } else if (argc > 2) {
            if (strcmp(argv[2], "num") == 0) {
                system("ps -e | wc -l");
            } else {
                printf("Usage: sys proc [num]\n(dont enter a number, just enter \"num\" for [num]\n", argv[0]);
        }
        } 

    }

    if (strcmp(argv[1], "ping") == 0) {
        system("ping -c 1 1.1.1.1 | awk -F'=' '/time/{print $4}'");
    }

    if (strcmp(argv[1],"version") == 0) {
        printf("sysinfo v2.1\n");
    }

    if (strcmp(argv[1], "version-latest") == 0) {
        FILE *fp;
        fp = popen("curl -s https://api.github.com/repos/quantumvoid0/sysinfo/releases/latest | grep -o -m1 '\"tag_name\": *\"[^\"]*\"' | cut -d'\"' -f4","r");
        if (!fp) return 1;
        char ver[256];
	if (fgets(ver,sizeof(ver),fp) == NULL || strlen(ver) < 2) {
        printf("Please try again in a few minutes.\nYou have exceeded the API rate limit\n");
	} else {
	        printf("sysinfo %s",ver);
        }
            pclose(fp);
        }       
    

    if (strcmp(argv[1], "update") == 0) {
        system("git clone https://github.com/quantumvoid0/sysinfo.git");
	chdir("sysinfo");
	system("./setup.sh update");
	chdir("..");
	system("rm -rf sysinfo");
    }

    return 0;
};
