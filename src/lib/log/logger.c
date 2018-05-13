#include <stdio.h>
#include <string.h>
#include <zconf.h>
#include "logger.h"

#define FIRST_COLUMN 32

void init_logger(char *name) {
    logConfig.name = name;
    logConfig.lockFd = open_lockfile(LOG_FILE);
}

static void build_log_message(char *buffer){
    sprintf(buffer+strlen(buffer), "%s[", logConfig.name);
    sprintf(buffer+strlen(buffer), "%i]:", getpid());


    char space[50];
    memset((void*)space,*(int*)" ", 50);
    if(strlen(buffer)<FIRST_COLUMN){
        space[FIRST_COLUMN-strlen(buffer)]=0;
        sprintf(buffer+strlen(buffer), "%s", space);
    }
}

void safelog(char *logMessage, ...) {
    char buffer[2048] = {0} ;
    build_log_message(buffer);

    va_list args;
    va_start(args, logMessage);
    vsprintf(buffer + (strlen(buffer)), logMessage, args);
    va_end(args);
    sprintf(buffer+strlen(buffer), "\n");

    acquire_lock(logConfig.lockFd);
    write_lockfile(logConfig.lockFd, buffer, strlen(buffer));
    release_lock(logConfig.lockFd);
}
