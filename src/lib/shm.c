#include "shm.h"

int create_shm(int id, int size){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (shmget(clave, size, IPC_CREAT | IPC_EXCL | 0660));
    /* da error si ya existe */
}

int get_shm(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (shmget(clave, 1, 0660));
}

void* map_shm(int id){
    void*addr = shmat(id, NULL, 0);
    if(addr==(void*)-1){
        safeperror("No se pudo mapear la memoria");
        exit(-1);
    }
    return addr;
}

int unmap(void* addr){
    return shmdt(addr);
}

int remove_shm(int id){
    return (shmctl(id, IPC_RMID, (struct shmid_ds *) 0));
}
