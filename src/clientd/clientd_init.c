#include <stdio.h>
#include <stdlib.h>
#include "connector/broker_connector.h"
#include "../lib/msg.h"

clientConfig config;

void init_daemon(int argc, char **argv);
int create_queue(int queueId);

int main(int argc, char **argv) {
    init_daemon(argc, argv);

    char broker_socket[10], request_queue[10], response_queue[10], clientId_queue[10];
    snprintf(broker_socket, sizeof(broker_socket), "%d", config.brokerSocket);
    snprintf(request_queue, sizeof(request_queue), "%d", CLIENT_SERVICE_REQUEST_QUEUE);
    snprintf(response_queue, sizeof(response_queue), "%d", CLIENT_SERVICE_RESPONSE_QUEUE);
    snprintf(clientId_queue, sizeof(clientId_queue), "%d", CLIENT_SERVICE_CLIEND_ID_QUEUE);

    if (fork() == 0) {
        printf("Launching client receiver...");
        execl("./clientd_i", "./clientd_i", broker_socket, request_queue, 0);
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        printf("Launching client responder...");
        execl("./clientd_o", "./clientd_o", broker_socket, response_queue, clientId_queue, 0);
        exit(EXIT_FAILURE);
    }

    return 0;
}

void init_daemon(int argc, char **argv) {
    config.address = argv[1];
    config.port = atoi(argv[2]);
    config.brokerSocket = create_broker_socket(config.address, config.port);
    create_queue(CLIENT_SERVICE_REQUEST_QUEUE);
    create_queue(CLIENT_SERVICE_RESPONSE_QUEUE);
    create_queue(CLIENT_SERVICE_CLIEND_ID_QUEUE);
}

int create_queue(int queueId) {
    int response = create_msg(queueId);
    if (response < 0) {
        perror("Can not create IPC message queue");
        exit(EXIT_FAILURE);
    }
    return response;
}