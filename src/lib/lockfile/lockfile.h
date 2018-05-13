#ifndef INC_7574_DMP_LOCKFILE_H
#define INC_7574_DMP_LOCKFILE_H

#include <unistd.h>
#include <fcntl.h>

int open_lockfile(char* filename);
int acquire_lock(int fd);
int release_lock(int fd);
ssize_t write_lockfile(int fd, void *buffer, ssize_t buffsize);

#endif //INC_7574_DMP_LOCKFILE_H