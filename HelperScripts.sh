#!/bin/sh
if [[ $1 == "default-host" ]]; then
    echo i686-elf

elif [[ $1 == "target-triplet" ]]; then
    if echo "$2" | grep -Eq 'i[[:digit:]]86-'; then
    echo i386
    else
    echo "$2" | grep -Eo '^[[:alnum:]_]*'
    fi

elif [[ $1 == "clean" ]];then
    . ./config.sh
    for PROJECT in $PROJECTS; do
        (cd $PROJECT && $MAKE clean)
    done

elif [[ $1 == "build" ]]; then
    set -e
    . ./HelperScripts.sh headers
    for PROJECT in $PROJECTS; do
        (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
    done

elif [[ $1 == "headers" ]]; then
    set -e
    . ./config.sh
    mkdir -p "$SYSROOT"
    for PROJECT in $SYSTEM_HEADER_PROJECTS; do
        (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install-headers)
    done


else
    echo "Error: Unrecognized Parameter passed"
    exit -1
fi
