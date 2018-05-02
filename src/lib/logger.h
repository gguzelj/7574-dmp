#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void build_log_message(char *buffer);
void safelog(const char* format, ...);
void safeperror(const char* format, ...);
static void exit_message(void);
void log_exit();
void init_logger(const char* pname, int color);

#endif /* _LOGGER_H_ */

