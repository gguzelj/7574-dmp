#include "msg.h"

int create_msg(int id){
    key_t clave;
    int fd = open(DIRECTORY, O_WRONLY | O_APPEND | O_CREAT, 0644);
    close(fd);
    clave = ftok(DIRECTORY, id);
    return (msgget(clave,  IPC_CREAT | IPC_EXCL | 0660));
    /* da error si ya existe */
}

int get_msg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (msgget(clave, 0660));
}

void send_msg(int id, const void *msgp, size_t msgsz){
    if(msgsnd(id,msgp,msgsz-sizeof(long),0)==-1){
        safeperror("Can not send message");
        exit(-1);
    }
}

void receive_msg(int id, void *msgp, size_t msgsz, long type){
    if(msgrcv(id,msgp,msgsz-sizeof(long),type,0)==-1){
        safeperror("Can not receive message");
        exit(-1);
    }
}

int remove_msg(int id){
    return (msgctl(id, IPC_RMID, NULL));
}