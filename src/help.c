#include <stdio.h>
#include <unistd.h>

static int tty;

#define HDR(s) \
	do { if (tty) fputs("\033[1;36m", stdout); \
		fputs(s, stdout); \
		if (tty) fputs("\033[0m", stdout); } while (0)

#define CMD(s) \
	do { if (tty) fputs("\033[1;33m", stdout); \
		fputs(s, stdout); \
		if (tty) fputs("\033[0m", stdout); } while (0)

#define TXT(s) \
	do { if (tty) fputs("\033[0;37m", stdout); \
		fputs(s, stdout); \
		if (tty) fputs("\033[0m", stdout); } while (0)

#define SEC(n) HDR("\n" n ":\n")
#define ROW(c, d) \
	do { CMD("  " c); fputs("  ", stdout); TXT(d "\n"); } while (0)

int help() {
	tty = isatty(fileno(stdout));

	HDR("Usage: sys <command> [arg2] [arg3]\n\n");

	SEC("Core info");
	ROW("os", "operating system name");
	ROW("kernel", "kernel release");
	ROW("host|hostname|name", "hostname");
	ROW("uptime|up", "system uptime");
	ROW("me", "current user");
	ROW("root", "root filesystem usage");
	ROW("drives", "mounted drives usage");
	ROW("init", "init system (PID 1)");

	SEC("CPU / memory");
	ROW("cpu", "cpu model name");
	ROW("cpu load", "per-core load % (all cores)");
	ROW("cpu load <core>", "load % for one core (0-based)");
	ROW("cpucores|cores|core", "number of online cores");
	ROW("cpufreq|freq [<core>]", "frequency of every core, or one");
	ROW("ram|mem|memory", "ram totals / used / left");
	ROW("ram total|used|left", "one ram value");
	ROW("ram total|used|left val", "value only (no label)");
	ROW("swap", "swap totals / used / left");
	ROW("swap total|used|left", "one swap value");
	ROW("swap total|used|left val", "value only (no label)");

	SEC("Hardware / firmware");
	ROW("bios", "bios vendor");
	ROW("bios vendor|version|date|release", "one bios field");
	ROW("system|sys", "product name");
	ROW("system family|name|sku|version", "one product field");
	ROW("board", "board vendor");
	ROW("board tag|name|vendor|version", "one board field");

	SEC("Environment");
	ROW("shell", "shell name ($SHELL)");
	ROW("term", "terminal name ($TERM)");
	ROW("editor", "editor ($EDITOR, if set)");
	ROW("locale", "full locale ($LANG)");
	ROW("language", "language from $LANG");
	ROW("region", "region from $LANG");
	ROW("encoding", "encoding from $LANG");
	ROW("ip", "local IP (LAN facing)");
	ROW("ping", "latency to 1.1.1.1");
	ROW("proc|ps", "list all processes");
	ROW("proc|ps num", "number of processes");

	SEC("Clock");
	ROW("time", "time (24h)");
	ROW("time 12|24", "time format");
	ROW("date", "date (DD-MM-YYYY)");
	ROW("date 1|2|3", "date format (1=DMY 2=MDY 3=YMD)");

	SEC("Interface");
	ROW("fetch", "info beside an ASCII logo");
	ROW("fetch <logo>", "specific logo (e.g. fetch arch)");
	ROW("fetch os", "list available logos");
	ROW("live", "live dashboard (ncurses)");

	SEC("Self");
	ROW("version", "installed version");
	ROW("help", "this help");

	TXT("\nConfig: fetch reads ~/.config/sysinfo/config.json"
			" (or $XDG_CONFIG_HOME/sysinfo/config.json)\n");

	return 1;
}
