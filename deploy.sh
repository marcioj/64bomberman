#!/bin/bash
set -eu

LOADER=/mnt/c/Users/marcio/N64/UNFLoader.exe
ROM="${1:-*.n64}"

function fail {
  echo $1 >&2
  exit 1
}

function retry {
  local n=1
  local max=5
  local delay=1
  while true; do
    "$@" && break || {
      if [[ $n -lt $max ]]; then
        ((n++))
        echo "Command failed. Attempt $n/$max:"
        sleep $delay;
      else
        fail "The command has failed after $n attempts."
      fi
    }
  done
}

./build.sh 'clean console'

$LOADER -d -l -r $ROM
