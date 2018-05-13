#include "clientd_o.h"

int main(int argc, char **argv) {
    init_logger("Client response daemon");
    init_daemon(argc, argv);
    do {
        response_t *response = receive_response(config.brokerSocket);
        responseHandler handler = find_response_handler(response);
        handler(response);
    } while (config.running);
    return EXIT_SUCCESS;
}

void init_daemon(int argc, char **argv) {
    config.running = true;
    config.brokerSocket = atoi(argv[1]);
    config.responseQueueId = get_msg(atoi(argv[2]));
    responseHandlers[CREATE] = &createHandler;
    responseHandlers[PUBLISH] = &publishHandler;
    responseHandlers[SUBSCRIBE] = &subscribeHandler;
    responseHandlers[RECEIVE] = &receiveHandler;
    responseHandlers[DESTROY] = &destroyHandler;
}

responseHandler find_response_handler(response_t *response) {
    safelog("create response received from pid %d", response->mtype);
    return responseHandlers[response->type];
}

void createHandler(response_t *response) {
    send_msg(config.responseQueueId, response, sizeof(response_t));
}

void publishHandler(response_t *response) {
    safelog("Publish handler invoked");
}

void subscribeHandler(response_t *request) {
    safelog("Subscribe handler invoked");
}

void receiveHandler(response_t *request) {
    safelog("Create handler invoked");
}

void destroyHandler(response_t *request) {
    printf("destroyHandler invoked");
}