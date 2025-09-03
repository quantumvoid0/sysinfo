install:
	gcc src/main.c src/swap.c src/cpu.c src/ram.c src/sys.c -o sys
	gcc src/live.c -o live -lncurses -ltinfo
	install -Dm755 sys /usr/local/bin/sys
	install -Dm755 live /usr/local/bin/live

uninstall:
	sudo rm -rf /usr/local/bin/sys
	sudo rm -rf /usr/local/bin/live


