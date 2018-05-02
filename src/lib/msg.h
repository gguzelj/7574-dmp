#ifndef _MSG_H_
#define _MSG_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#include "logger.h"
#include "resources.h"

int create_msg(int id);

int get_msg(int id);

void send_msg(int id, const void *msgp, size_t msgsz);

void receive_msg(int id, void *msgp, size_t msgsz, long type);

int remove_msg(int id);

#endif /* _MSG_H_ */
