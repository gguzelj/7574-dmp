#ifndef INC_7574_DMP_BROKER_CONNECTOR_C
#define INC_7574_DMP_BROKER_CONNECTOR_C

#include "broker_connector.h"

int create_broker_socket(char* address, int port) {
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Unexpected error while creating broker Socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if(inet_pton(AF_INET, address, &serv_addr.sin_addr)<=0) {
        perror("Invalid broker address");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection with broker Failed");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void send_request(int fd, request_t *request) {
    char* buffer = (char*) request;
    ssize_t sent = 0;
    do {
        sent += send(fd, &buffer[sent], sizeof(request_t) - sent , 0);
    } while (sent < sizeof(request_t));
}


void receive_response(int fd, response_t *buffer) {
    ssize_t received = 0;
    do {
        received += recv(fd, &buffer[received], sizeof(response_t) - received, 0);
    } while (received < sizeof(response_t));
}

#endif //INC_7574_DMP_BROKER_CONNECTOR_C