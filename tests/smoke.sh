#!/usr/bin/env bash
set -u

SYS="${1:-./sys}"
fail=0

ok()   { printf '  ok   %s\n' "$1"; }
bad()  { printf '  FAIL %s -> %s\n' "$1" "$2"; fail=1; }

run() {
	out="$("$SYS" $1 2>/dev/null)"
	if [[ -z "$out" ]]; then
		bad "$1" "empty output"
	else
		ok "$1"
	fi
}

soft() {
	out="$("$SYS" $1 2>/dev/null)"
	if [[ -z "$out" ]]; then
		printf '  WARN %s -> empty (network likely unavailable)\n' "$1"
	else
		ok "$1"
	fi
}

echo "Core info:"
run os
run kernel
run host
run hostname
run name
run uptime
run up
run me
run root
run drives
run init

echo "CPU / memory:"
run cpu
run "cpu load"
run "cpu load 0"
run "cpu load 7"
run cpucores
run cores
run core
run cpucore
run cpufreq
run freq
run cpufrequency
run frequency
run "cpufreq 0"
run "freq 3"
run ram
run mem
run memory
run "ram total"
run "ram used"
run "ram left"
run "ram total val"
run "ram used val"
run "ram left val"
run swap
run "swap total"
run "swap used"
run "swap left"
run "swap total val"
run "swap used val"
run "swap left val"

echo "Hardware / firmware:"
run bios
run "bios vendor"
run "bios version"
run "bios date"
run "bios release"
run system
run sys
run "system family"
run "system name"
run "system sku"
run "system version"
run board
run "board tag"
run "board name"
run "board vendor"
run "board version"

echo "Environment:"
run shell
soft term
soft editor
run locale
run language
run region
run encoding
run ip
run "proc num"
run proc
run ps
run "ps num"

echo "Clock:"
run time
run "time 12"
run "time 24"
run date
run "date 1"
run "date 2"
run "date 3"

echo "Interface / self:"
run "fetch arch"
run "fetch os"
soft ping
run version
run help
timeout 1 "$SYS" live >/dev/null 2>&1
live_rc=$?
if [[ $live_rc -eq 2 ]]; then
	bad "live" "unknown command"
elif [[ $live_rc -eq 124 || $live_rc -eq 0 || $live_rc -eq 1 ]]; then
	ok "live (starts or exits cleanly without TTY: rc=$live_rc)"
else
	bad "live" "exit $live_rc"
fi

echo "Negative (must fail, not crash):"
"$SYS" time 99 >/dev/null 2>&1
if [[ $? -eq 0 ]]; then bad "time 99" "should fail"; else ok "time 99 fails"; fi

"$SYS" bios bogus >/dev/null 2>&1
if [[ $? -eq 0 ]]; then bad "bios bogus" "should fail"; else ok "bios bogus fails"; fi

"$SYS" definitely-not-a-command >/dev/null 2>&1
if [[ $? -ne 2 ]]; then bad "unknown-cmd exit" "expected 2"; else ok "unknown-cmd exit code"; fi

echo "Fetch layout:"
fetch_out="$("$SYS" fetch arch 2>/dev/null)"
blank=$(printf '%s\n' "$fetch_out" | grep -c '^$' || true)
entries=$(printf '%s\n' "$fetch_out" | grep -c '◆' || true)
if [[ "$blank" -ne 0 ]]; then
	bad "fetch blank lines" "$blank blank line(s)"
else
	ok "fetch: no blank lines ($entries entries)"
fi

if [[ $fail -eq 0 ]]; then
	echo "ALL TESTS PASSED"
	exit 0
else
	echo "SOME TESTS FAILED"
	exit 1
fi
