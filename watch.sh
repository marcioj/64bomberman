#!/bin/sh

PID=

sigint_handler()
{
  kill $PID
  exit
}

trap sigint_handler INT

while true; do
  ./run.sh "$@" &
  PID=$!
  inotifywait -e modify *.n64
  kill $PID
done