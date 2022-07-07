#!/bin/sh

MODE=BUILD
CONFIG=Debug

while getopts "abcrst:" OPT; do
    case "$OPT" in
    a)
    TARGET="AU"
    ;;
    b)
    MODE=BUILD
    ;;
    c)
    MODE=CONF
    ;;
    r)
    CONFIG=Release
    ;;
    s)
    TARGET="Standalone"
    ;;
    t)
    TARGET="$OPTARG"
    ;;
    *)
    echo what $opt
    ;;
    esac
done

if [ $MODE = "BUILD" ]; then
    if [ -n "$TARGET" ]; then TARGET_STR="--target rtstft_ctl_""$TARGET"; fi
    cmake --build build $TARGET_STR --config "$CONFIG"
elif [ $MODE = "CONF" ]; then
    cmake -Bbuild -G"Ninja Multi-Config"
fi