#!/bin/bash


function build {
    echo "building"
    make clean
    cmake .
    make
}

function delete_pid_file {
    rm /tmp/cliendd.pid
}

function delete_log_file {
    rm /tmp/dist_2017.log
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
delete_pid_file
delete_log_file
build
