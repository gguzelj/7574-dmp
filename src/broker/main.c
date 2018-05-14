#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "../common/common.h"
#include "../lib/log/logger.h"

request_t *receive_request(int fd);

void send_response(int fd, response_t *response);


void sendCreateResponse(int fd, request_t *pRequest);

void sendPublishResponse(int fd, request_t *pRequest);

int main() {
    init_logger("broker server");

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    bool running = true;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                             (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (running) {
        request_t *request = receive_request(new_socket);
        if (request->type == CREATE) {
            sendCreateResponse(new_socket, request);
        }

        if (request->type == PUBLISH) {
            sendPublishResponse(new_socket, request);
        }
    }

}

void sendCreateResponse(int fd, request_t *pRequest) {
    int newId = rand();
    safelog("New instance to create. Id to assign: %d", newId);
    response_t createResponse;
    createResponse.type = pRequest->type;
    createResponse.mtype = pRequest->mtype;
    createResponse.status.code = OK;
    createResponse.body.create.id.value = newId;
    send_response(fd, &createResponse);
}

void sendPublishResponse(int fd, request_t *request) {
    safelog("publishing on topic %s", request->body.publish.topic.name);
    response_t publishResponse = {0} ;
    publishResponse.type = request->type;
    publishResponse.mtype = request->mtype;
    publishResponse.status.code = OK;
    publishResponse.body.publish.id = request->body.publish.id;
    send_response(fd, &publishResponse);
}

request_t *receive_request(int fd) {
    char *buffer = malloc(sizeof(request_t));
    ssize_t received = 0;
    do {
        received += recv(fd, &buffer[received], sizeof(request_t) - received, 0);
    } while (received < sizeof(request_t));
    return (request_t *) buffer;
}

void send_response(int fd, response_t *response) {
    char *buffer = (char *) response;
    ssize_t sent = 0;
    do {
        sent += send(fd, &buffer[sent], sizeof(response_t) - sent, 0);
    } while (sent < sizeof(response_t));
}