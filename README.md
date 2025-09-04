# better-fetch
Better-Fetch; because your system deserves the spotlight. Specs, stats & performance at a glance

>[!WARNING]
>`sys live` & `sys fetch` toos are under work and in beta, things might not render correctly or break. you are free to try it out tho

## Installation

```fish
git clone https://github.com/better-ecosystem/better-fetch
cd better-fetch
./setup install
```

## Uninstallation
```fish
git clone https://github.com/better-ecosystem/better-fetch
cd better-fetch
./setup uninstall
```

## Usage
```

Usage: sys <arg1> [arg2] [arg3]

Commands: <arg1>
  os       : operating system name
  kernel   : kernel info
  host     : hostname
  uptime   : system uptime
  cpu      : cpu info
  cpucores : number of cpu cores
  cpufreq  : cpu frequency per core
  ram      : ram info
  swap     : swap info
  root     : root partition info
  live     : live info (beta)
  fetch    : fetches sys info (beta)
  me       : user info

Commands: [arg2]
  ram [total/used/left]      : ram info
  swap [total/used/left]     : swap info
  cpu [load]                 : cpu load info

Commands: [arg3]
  ram [total/used/left] [val]      : ram info with only values
  swap [total/used/left] [val]     : swap info with only values

Examples:
  sys os
  sys swap left val
  sys ram used

fetch is configured in ~/.config/better-fetch/config.json
```

**usage of `sys live`**
- `sys live` contains a status bar at bottom which shows what is selected right now
- use arrow keys to scroll the boxes that is selected
- left and right arrow keys to change selection
