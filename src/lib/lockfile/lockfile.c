#include "lockfile.h"

int open_lockfile(char *filename) {
    return open(filename, O_CREAT | O_WRONLY, 0777);
}

int acquire_lock(int fd) {
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    return fcntl(fd, F_SETLKW, &(fl));
}

int release_lock(int fd) {
    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    return fcntl(fd, F_SETLK, &(fl));
}


ssize_t write_lockfile(int fd, void *buffer, ssize_t buffsize) {
    lseek(fd, 0, SEEK_END);
    return write(fd, buffer, buffsize);
}