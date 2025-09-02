# sysinfo
Small utility to view system informations

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
  live     : live info
  me       : user info

Commands: [arg2]
  ram [total/used/left]      : ram info
  swap [total/used/left]     : swap info
  cpu [load]                 : cpu load info

Examples:
  sys os
  sys ram used
```
