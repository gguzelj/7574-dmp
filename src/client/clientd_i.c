#include "clientd_i.h"

int main(int argc, char **argv) {
    init_logger("Client receiver daemon");
    init_daemon(argc, argv);
    do {
        request_t *request = read_request();
        requestHandler handler = find_request_handler(request);
        handler(request);
    } while (config.running);
    return EXIT_SUCCESS;
}

void init_daemon(int argc, char **argv) {
    config.running = true;
    config.brokerSocket = atoi(argv[1]);
    config.requestQueueId = get_msg(atoi(argv[2]));
    requestHandlers[CREATE] = &createHandler;
    requestHandlers[PUBLISH] = &publishHandler;
    requestHandlers[SUBSCRIBE] = &subscribeHandler;
    requestHandlers[RECEIVE] = &receiveHandler;
    requestHandlers[DESTROY] = &destroyHandler;
}

request_t *read_request() {
    request_t request;
    receive_msg(config.requestQueueId, &request, sizeof(struct request), 0);
    request_t *response = malloc(sizeof(request_t));
    memcpy(response, &request, sizeof(request_t));
    return response;
}

requestHandler find_request_handler(request_t *request) {
    safelog("create request received from pid %d", request->mtype);
    return requestHandlers[request->type];
}

/**
 * All this handlers send requests to the broker via the broker socket.
 * The process that read the broker response is the clientd_o
 */

void createHandler(request_t *request) {
    send_request(config.brokerSocket, request);
}

void publishHandler(request_t *request) {
    safelog("publishing on topic %s", request->body.publish.topic.name);
    send_request(config.brokerSocket, request);
}

void subscribeHandler(request_t *request) {
    safelog("subscribing on topic %s", request->body.subscribe.topic);
    send_request(config.brokerSocket, request);
}

void receiveHandler(request_t *request) {
    safelog("Create handler invoked");
}

void destroyHandler(request_t *request) {
    printf("destroyHandler invoked");
}