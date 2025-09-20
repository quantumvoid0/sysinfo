#include <stdio.h>

int help() {
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
	printf("  drives   : drive info\n");
	printf("  me       : user info\n\n");
	printf(
	    "  update         : update current version to the "
	    "latest\n");
	printf("  version        : installed sysinfo version\n");
	printf("  version-latest : latest available sysinfo version\n\n");
	printf("Commands: [arg2]\n");
	printf("  ram [total/used/left]                 : ram info\n");
	printf("  swap [total/used/left]                : swap info\n");
	printf(
	    "  cpu [load]                               : cpu load "
	    "info\n");
	printf(
	    "  cpufreq (core number)                    : cpu freq by "
	    "core number\n");
	printf("  bios [vendor/version/date/release]    : bios info\n");
	printf(
	    "  system [family/name/sku/version]      : system family "
	    "info\n");
	printf(
	    "  board [tag/name/vendor/version]       : motherboard "
	    "info\n");
	printf(
	    "  proc [num]                               : list number "
	    "of processes\n");
	printf("  time [12/24]                          : time\n");
	printf("  date [1/2/3]                          : date\n\n");
	printf("Commands: [arg3]\n");
	printf(
	    "  ram [total/used/left] (val)      : ram info with only "
	    "values\n");
	printf(
	    "  swap [total/used/left] (val)     : swap info with only "
	    "values\n");
	printf("  cpu load (core number)           : cpu load info\n");
	printf("Examples:\n");
	printf("  sys os\n");
	printf("  sys swap left val\n");
	printf("  sys ram used\n\n");
	printf("fetch is configured in ~/.config/sysinfo/config.json\n\n");

	return 1;
};
