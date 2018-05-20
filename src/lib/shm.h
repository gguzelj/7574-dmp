#ifndef _SHM_H_
#define _SHM_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "logger.h"
#include "resources.h"

int create_shm(int id, int size);

int get_shm(int id);

void *map_shm(int id);

int unmap(void *addr);

int remove_shm(int id);

#endif /* _SHM_H_ */
