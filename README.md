# better-fetch
Small utility to view system informations (for linux)

>[!WARNING]
>`sys live` tool is under work and in beta, things might not render correctly or break. you are free to try it out tho

## Installation

```fish
git clone https://github.com/better-ecosystem/better-fetch
cd sysinfo
sudo make install
```

## Uninstallation
```fish
git clone https://github.com/better-ecosystem/better-fetch
cd sysinfo
sudo make uninstall
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
```

**usage of `sys live`**
- `sys live` contains a status bar at bottom which shows what is selected right now
- use arrow keys to scroll the boxes that is selected
- left and right arrow keys to change selection
