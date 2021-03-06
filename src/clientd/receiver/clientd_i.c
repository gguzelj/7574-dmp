#include "clientd_i.h"

void map_local_to_global(request_t *request);

int main(int argc, char **argv) {
    init_daemon(argc, argv);
    safelog("running receiver");
    do {
        request_t request = {0} ;
        read_request(&request);
        find_request_handler(request)(request);
    } while (config.running);
    return EXIT_SUCCESS;
}

void init_daemon(int argc, char **argv) {
    init_logger("client receiver");
    init_mapper();
    config.running = true;
    config.brokerSocket = atoi(argv[1]);
    config.requestQueueId = get_msg(atoi(argv[2]));
    DEFINE_REQUEST_HANDLERS
}

void read_request(request_t *request) {
    receive_msg(config.requestQueueId, request, sizeof(request_t), 0);
}

requestHandler find_request_handler(request_t request) {
    return requestHandlers[request.type];
}

/**
 * All this handlers send requests to the broker via the broker socket.
 * The process that read the broker response is the clientd_o
 */
void createHandler(request_t request) {
    safelog("create: client pid %d", request.mtype);
    request.context.clientId.value = request.mtype;
    send_request(config.brokerSocket, &request);
}

void publishHandler(request_t request) {
    safelog("publish: topic %s for client %ld", request.body.publish.topic.name, request.context.clientId);
    map_local_to_global(&request);
    send_request(config.brokerSocket, &request);
}

void subscribeHandler(request_t request) {
    safelog("subscribe: on topic %s for client %ld", request.body.subscribe.topic.name, request.context.clientId);
    map_local_to_global(&request);
    send_request(config.brokerSocket, &request);
}

void receiveHandler(request_t request) {
    safelog("this function was supposed to not be called");
}

void destroyHandler(request_t request) {
    safelog("destroy: for client %ld", request.context.clientId);
    map_local_to_global(&request);
    send_request(config.brokerSocket, &request);
}

void map_local_to_global(request_t *request) {
    clientId_t localId = request->context.clientId;
    request->context.clientId = get_global_id(localId);
    if (request->context.clientId.value < 0) {
        safelog("Wrong localId %ld", localId);
    }
}