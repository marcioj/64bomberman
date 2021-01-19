#!/bin/bash

CEN_64_PATH=$HOME/workspace/n64/cen64/cen64
PROJECT_64_PATH=/mnt/c/Users/marcio/N64/Project64-v2.4.0-1377-g8ed4a80/Project64.exe 
MUPEN64_PATH=/mnt/c/Users/marcio/N64/mupen64plus-bundle-win32-2.5.9/mupen64plus-ui-console.exe
PIFDATA_PATH=$HOME/workspace/n64/pifdata.bin

EMULATOR=cen64
EMU_ARGS=
ROM="*.n64"

kill_emulator()
{
    case $EMULATOR in
        cen64)
            killall cen64
            ;;
        project64)
            taskkill.exe "/IM" "Project64.exe" "/F"
            ;;
        mupen64)
            taskkill.exe "/IM" "mupen64plus-ui-console.exe" "/F"
            ;;
    esac
  exit
}

trap kill_emulator SIGINT SIGTERM

run_mupen64() {
    local pwd=$PWD
    local dir=$(dirname $MUPEN64_PATH)
    local program=$(basename $MUPEN64_PATH)
    cd $dir
    # mupen only works if you run it from its own directory and use the full windows path on WSL
    ./$program $(wslpath -w $pwd/$ROM) $EMU_ARGS
}

# parse options
while true
do
    case $# in 0) break ;; esac
    case $1 in
        -c|--cen64) shift; EMULATOR=cen64; shift ;;
        -p|--project64) shift; EMULATOR=project64; shift ;;
        -m|--mupen64) shift; EMULATOR=mupen64 ;;
        -|--) shift; EMU_ARGS=$@; break;;
        -h|--help) cat <<EOF >&2; exit 0 ;;
Syntax: $0 [<rom>] [-c | --cen64 | -p | --project64 | -m | --mupen64] [--help]
EOF
        -*) cat <<EOF >&2; exit 2 ;;
Invalid option $1.  Try $0 --help to see available options.
EOF
        *) ROM=$1; break ;;
    esac
done

set -e

# ./build.sh

case $EMULATOR in
    cen64)
        $CEN_64_PATH -multithread $PIFDATA_PATH $EMU_ARGS $ROM &
        wait $!
        ;;
    project64)
        $PROJECT_64_PATH $PWD/$ROM $EMU_ARGS
        ;;
    mupen64)
        run_mupen64
        ;;
esac