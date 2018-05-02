#!/bin/bash


function build {
    echo "building"
    make clean
    cmake .
    make
}

function clean_ipc {
    echo "Cleaning ipc"
    #remove queues
    ipcs | awk '/^q / { print $2 }' | xargs -n 1 ipcrm -q

    #remove semaphores
    ipcs | awk '/^s / { print $2 }' | xargs -n 1 ipcrm -s

    #remove shared memories
    ipcs | awk '/^m / { print $2 }' | xargs -n 1 ipcrm -m
}

clean_ipc
build
