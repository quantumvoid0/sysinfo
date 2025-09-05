#!/usr/bin/env bash
set -e

ACTION=$1
PREFIX=/usr/local
USER_CONFIG="$HOME/.config/sysinfo"
ART="/usr/share/sysinfo"

if [[ -z "$ACTION" ]]; then
    echo "Usage: $0 [install|uninstall]"
    exit 1
fi

if [[ "$ACTION" == "install" ]]; then
    echo "Compiling binaries..."
    gcc src/main.c src/swap.c src/cpu.c src/ram.c src/sys.c src/time.c src/date.c -o sys
    gcc src/live.c -o live -lncurses -ltinfo
    gcc src/fetch.c -o fetch

    echo "Installing binaries to $PREFIX/bin..."
    sudo install -Dm755 sys "$PREFIX/bin/sys"
    sudo install -Dm755 live "$PREFIX/bin/live"
    sudo install -Dm755 fetch "$PREFIX/bin/fetch"

    echo "Creating config directory at $USER_CONFIG..."
    mkdir -p "$USER_CONFIG"
    cp src/config.json "$USER_CONFIG/config.json"

    echo "Installing art to $ART..."
    sudo mkdir -p "$ART"
    sudo cp -r src/art/* "$ART/"

    echo "Installation complete!"

elif [[ "$ACTION" == "uninstall" ]]; then
    echo "Removing binaries..."
    sudo rm -f "$PREFIX/bin/sys"
    sudo rm -f "$PREFIX/bin/live"
    sudo rm -f "$PREFIX/bin/fetch"

    echo "Removing config..."
    rm -rf "$USER_CONFIG"

    echo "Removing art..."
    sudo rm -rf "$ART"

    echo "Uninstallation complete!"

else
    echo "Unknown action: $ACTION"
    echo "Usage: $0 [install|uninstall]"
    exit 1
fi

