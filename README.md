# sysinfo
Small utility to view system informations (for linux)

>[!WARNING]
>`sys live` tool is under work and in beta, things might not render correctly or break. you are free to try it out tho

## Installation

```fish
git clone https://github.com/quantumvoid0/sysinfo
cd sysinfo
sudo make install
```

## Uninstallation
```fish
git clone https://github.com/quantumvoid0/sysinfo
cd sysinfo
sudo make uninstall
```

## Usage
```
Usage: sys <arg1> [arg2]

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

Examples:
  sys os
  sys ram used
```

**usage of `sys live`**
- `sys live` contains a status bar at bottom which shows what is selected right now
- use arrow keys to scroll the boxes that is selected
- left and right arrow keys to change selection
