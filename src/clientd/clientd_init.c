#include <stdio.h>
#include <stdlib.h>
#include "broker_connector/broker_connector.h"
#include "../lib/msg.h"
#include "../lib/shm.h"

clientConfig config;

void init_daemon(int argc, char **argv);

void create_shared_memory();

int create_queue(int queueId);

void check_unique_instance_running();

void create_pid_file(int receiverFd, int responderFd);

int main(int argc, char **argv) {
    check_unique_instance_running();
    init_daemon(argc, argv);

    char broker_socket[10], request_queue[10], response_queue[10], clientId_queue[10], receive_queue[10];
    snprintf(broker_socket, sizeof(broker_socket), "%d", config.brokerSocket);
    snprintf(request_queue, sizeof(request_queue), "%d", CLIENT_SERVICE_REQUEST_QUEUE);
    snprintf(response_queue, sizeof(response_queue), "%d", CLIENT_SERVICE_RESPONSE_QUEUE);
    snprintf(clientId_queue, sizeof(clientId_queue), "%d", CLIENT_SERVICE_CLIEND_ID_QUEUE);
    snprintf(receive_queue, sizeof(receive_queue), "%d", CLIENT_SERVICE_RECEIVE_QUEUE);

    int receiverFd = fork();
    if (receiverFd == 0) {
        printf("Launching client receiver...");
        execl("./clientd_i", "./clientd_i", broker_socket, request_queue, NULL);
        exit(EXIT_FAILURE);
    }

    int responderFd = fork();
    if (responderFd == 0) {
        printf("Launching client responder...");
        execl("./clientd_o", "./clientd_o", broker_socket, response_queue, clientId_queue, receive_queue, NULL);
        exit(EXIT_FAILURE);
    }

    create_pid_file(receiverFd, responderFd);
    return 0;
}

void init_daemon(int argc, char **argv) {
    config.address = argv[1];
    config.port = BROKER_LISTENING_PORT;
    config.brokerSocket = create_broker_socket(config.address, config.port);
    create_queue(CLIENT_SERVICE_REQUEST_QUEUE);
    create_queue(CLIENT_SERVICE_RESPONSE_QUEUE);
    create_queue(CLIENT_SERVICE_CLIEND_ID_QUEUE);
    create_queue(CLIENT_SERVICE_RECEIVE_QUEUE);
    create_shared_memory();
}

int create_queue(int queueId) {
    int response = create_msg(queueId);
    if (response < 0) {
        perror("Can not create IPC message queue");
        exit(EXIT_FAILURE);
    }
    return response;
}

void create_shared_memory() {
    int shm = create_shm(CLIENT_IDS_MAP_ID, sizeof(id_tuple_t) * CLIENT_IDS_MAP_CAPACITY);
    id_tuple_t *map = (id_tuple_t *) map_shm(shm);
    for (int i = 0; i < CLIENT_IDS_MAP_CAPACITY; ++i) {
        map[i].globalId = 0;
        map[i].localId = 0;
    }
}

void check_unique_instance_running() {
    if (access(CLIENT_PID_FILE, F_OK) != -1) {
        printf("client daemon already runnning");
        exit(EXIT_FAILURE);
    }
}

void create_pid_file(int receiverFd, int responderFd) {
    int fd = open(CLIENT_PID_FILE, O_CREAT | O_WRONLY, 0777);
    char content[20] = {0};
    snprintf(content, sizeof(content), "%d\n%d", receiverFd, responderFd);
    write(fd, content, sizeof(content));
    close(fd);
}