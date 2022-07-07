#!/bin/sh

MODE=BUILD
CONFIG=Debug

while getopts "bcrt:" OPT; do
    case "$OPT" in
    b)
    MODE=BUILD
    ;;
    c)
    MODE=CONF
    ;;
    r)
    CONFIG=Release
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
    echo build
    if [ -n "$TARGET" ]; then TARGET_STR="--target rtstft_ctl_""$TARGET"; fi
    cmake --build build $TARGET_STR --config "$CONFIG"
elif [ $MODE = "CONF" ]; then
    echo CONF
fi