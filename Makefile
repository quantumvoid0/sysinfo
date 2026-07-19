CC      ?= gcc
CFLAGS ?= -Os -Wall -Wextra -Wno-unused-parameter -Wno-format-truncation -Isrc/core -Isrc/fetch
LDFLAGS ?=
LDLIBS  ?= -lncurses

BIN    := sys
BINDIR := $(HOME)/.local/bin
ARTDIR := $(HOME)/.local/share/sysinfo
CFGDIR := $(HOME)/.config/sysinfo

SRCS := \
	src/main.c \
	src/commands.c \
	src/core/parse.c \
	src/info/misc.c \
	src/system/swap.c \
	src/system/cpu.c \
	src/system/ram.c \
	src/system/sys.c \
	src/system/time.c \
	src/system/date.c \
	src/system/drives.c \
	src/help.c \
	src/fetch.c \
	src/fetch/fetch_utils.c \
	src/live.c \
	src/live/live_utils.c \
	src/live/live_data.c

OBJS := $(SRCS:.c=.o)

.PHONY: all debug install uninstall clean test

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

debug: CFLAGS += -g -O0 -fsanitize=address,undefined
debug: $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(BIN)
	install -Dm755 $(BIN) $(BINDIR)/$(BIN)
	mkdir -p $(CFGDIR)
	[ -f $(CFGDIR)/config.json ] || cp src/config.json $(CFGDIR)/config.json
	mkdir -p $(ARTDIR)
	cp -r src/art/* $(ARTDIR)/

update: $(BIN)
	install -Dm755 $(BIN) $(BINDIR)/$(BIN)
	mkdir -p $(ARTDIR)
	cp -r src/art/* $(ARTDIR)/

uninstall:
	rm -f $(BINDIR)/$(BIN)
	rm -rf $(CFGDIR)
	rm -rf $(ARTDIR)

data_test: tests/data_test.c src/live/live_data.c src/core/parse.c
	$(CC) -Isrc/core -Isrc/live $^ -o $@

test: $(BIN) data_test
	./tests/smoke.sh ./$(BIN)
	./data_test

clean:
	rm -f $(BIN) $(OBJS) data_test
