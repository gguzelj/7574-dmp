#include <stdio.h>
#include <string.h>
#include "logger.h"

void init_logger(char* name) {
    logConfig.name = name;
    logConfig.lockFd = open_lockfile(LOG_FILE);
}

void safelog(char *logMessage) {
    acquire_lock(logConfig.lockFd);
    char msg[200];
    sprintf(msg, "[%s] - %s\n", logConfig.name, logMessage);
    write_lockfile(logConfig.lockFd, msg, strlen(msg));
    release_lock(logConfig.lockFd);
}