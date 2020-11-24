#!/bin/bash

COMMAND=$1

function timer {
    sleep 5
    echo "timeout"
}

function run() {
    $COMMAND &
    CMD_PID=$!
    echo "running"

    wait $CMD_PID
    echo "dead"
}

while read action; do
    $action &
done
