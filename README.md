# 👨‍💻 Sysinfo
**Sysinfo; because your system deserves the spotlight. Specs, stats & performance at a glance**

---

>[!WARNING]
>`sys live` & `sys fetch` toos are under work and in beta, things might not render correctly or break. you are free to try it out tho.

> [!IMPORTANT]
> 🚧 This project is under active development. Contributions, feature requests, ideas, and testers are welcome!
> ❗ More features coming soon!!

---


## ✨ Features

- 🔄 Get system stats or specs easily for your needs.
- 📱 Watch your system perfomance live in live mode.
- 🎚️ Retrieve your system stats/specs with a ascii art beautifully.
- 🧩 Modular design - use only what you need and remove the ones you don't use.
---

## 💾 Installation & Uninstallation

**To install Better Fetch run the commands as give below**
```fish
git clone https://github.com/quantumvoid0/sysinfo
cd sysinfo
./setup.sh install
```

**To uninstall Better Fetch run the commands as give below**
```fish
git clone https://github.com/quantumvoid0/sysinfo
cd sysinfo
./setup.sh uninstall
```

> [!TIP]
> **Security conscious?** You can review the installer script [here](https://raw.githubusercontent.com/quantumvoid0/sysinfo/refs/heads/main/setup.sh) before running it.

## 🫴 Usage
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
  time       : time
  date       : date
  me       : user info

Commands: [arg2]
  ram [total/used/left]      : ram info
  swap [total/used/left]     : swap info
  cpu [load]                 : cpu load info
  cpufreq [core number]      : cpu freq by core number
  time [12/24]               : time
  date [1/2/3]               : date

Commands: [arg3]
  ram [total/used/left] [val]      : ram info with only values
  swap [total/used/left] [val]     : swap info with only values

Examples:
  sys os
  sys swap left val
  sys ram used

fetch is configured in ~/.config/sysinfo/config.json
```
- If you have any questions on how to use or what something does , feel free to open a discussion

**usage of `sys live`**
- `sys live` contains a status bar at bottom which shows what is selected right now
- use arrow keys to scroll the boxes that is selected
- left and right arrow keys to change selection
- press `q` to quit

**usage of `sys fetch`**
- fetch is configured in `~/.config/sysinfo/config.json`
- formatting is as give below
  ```json
  {
    "commands": ["os","kernel","host","uptime","cpu","me","swap total","ram total","cores"]
  }
  ```
- you can mix and match all `sys` commands in there to display each

---

## 📚 Contribution

If you want to contribute, see [CONTRIBUTING.md](https://github.com/quantumvoid0/sysinfo/blob/main/CONTRIBUTING.md)


## 📄 License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) for more details.

---

### Made with ❤️ for the Linux community

[Report Bug](https://github.com/quantumvoid0/sysinfo/issues) •
[Request Feature](https://github.com/quantumvoid0/sysinfo/discussions) •
[Contribute](https://github.com/quantumvoid0/sysinfo/blob/main/CONTRIBUTING.md)
