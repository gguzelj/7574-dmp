#ifndef INC_7574_DMP_LOGGER_H
#define INC_7574_DMP_LOGGER_H

#include "../lockfile/lockfile.h"

#define LOG_FILE    "/tmp/dist_2017.log"

typedef struct loggerConfig {
    char* name;
    int lockFd;
} loggerConfig;

loggerConfig logConfig;

void init_logger(char* name);
void safelog(char *logMessage);

#endif //INC_7574_DMP_LOGGER_H
