#include "broker_responder.h"

void send_response(int fd, response_t *response);

int main(int argc, char **argv) {
    init_responder(argc, argv);
    safelog("waiting for requests");
    do {
        response_t response = receive_response() ;
        find_response_handler(response)(response);
    } while (config.running);
    return EXIT_SUCCESS;
}

responseHandler find_response_handler(response_t response) {
    return responseHandlers[response.type];
}

void init_responder(int argc, char **argv) {
    init_logger("broker responder");
    config.running = true;
    config.clientFd = atoi(argv[1]);
    config.responseQueue = get_msg(atoi(argv[2]));
    config.brokerDaemonId.value = atoi(argv[3]);
    safelog("responder id %ld", config.brokerDaemonId);
    DEFINE_RESPONSE_HANDLERS
}

/**
 * All this handlers send requests to the broker via the broker socket.
 * The process that read the broker response is the clientd_o
 */
void createHandler(response_t response) {
    safelog("create: client pid %d", response.context.clientId);
    send_response(config.clientFd, &response);
}

void publishHandler(response_t response) {
    safelog("publish: topic %s for client %ld", response.body.publish.topic.name, response.context.clientId);
    send_response(config.clientFd, &response);
}

void subscribeHandler(response_t response) {
    safelog("subscribe: on topic %s for client %ld", response.body.subscribe.topic.name, response.context.clientId);
    send_response(config.clientFd, &response);
}

void receiveHandler(response_t response) {
    safelog("receive: on topic %s for client %ld", response.body.receive.topic.name, response.context.clientId);
    send_response(config.clientFd, &response);
}

void destroyHandler(response_t response) {
    safelog("destroy: for client %ld", response.context.clientId);
    send_response(config.clientFd, &response);
}

void send_response(int fd, response_t *response) {
    char *buffer = (char *) response;
    ssize_t sent = 0;
    do {
        sent += send(fd, &buffer[sent], sizeof(response_t) - sent, 0);
    } while (sent < sizeof(response_t));
}

response_t receive_response() {
    response_t response;
    receive_msg(config.responseQueue, &response, sizeof(response_t), config.brokerDaemonId.value);
    response.mtype = response.context.clientId.value;
    return response;
}