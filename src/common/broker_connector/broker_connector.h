#ifndef INC_7574_DMP_BROKER_CONNECTOR_H
#define INC_7574_DMP_BROKER_CONNECTOR_H

#include <stdlib.h>
#include "../common.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <arpa/inet.h>

int create_broker_socket(char* address, int port);
void send_request(int fd, request_t* request);
void receive_response(int fd, response_t*);


#endif //INC_7574_DMP_BROKER_CONNECTOR_H
