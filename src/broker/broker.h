#ifndef INC_7574_DMP_BROKER_H
#define INC_7574_DMP_BROKER_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "../common/common.h"
#include "../lib/msg.h"
#include "../lib/log/logger.h"


void start_workers();

void init_broker();

void fill_config();

void create_socket();

void bind_socket();

void listen_socket();

int accept_new_connection();

void create_new_connection_handlers(int client_socket);


#endif //INC_7574_DMP_BROKER_H
