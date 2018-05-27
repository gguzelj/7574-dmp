#include "broker_receiver.h"
#include "../../common/common.h"

int main(int argc, char **argv) {
    init_receiver(argc, argv);
    safelog("waiting for requests");
    do {
        request_t request = receive_request() ;
        find_request_handler(request)(request);
    } while (config.running);
    return EXIT_SUCCESS;
}

requestHandler find_request_handler(request_t request) {
    return requestHandlers[request.type];
}

void init_receiver(int argc, char **argv) {
    init_logger("broker receiver");
    config.running = true;
    config.clientFd = atoi(argv[1]);
    config.receiveQueue = get_msg(atoi(argv[2]));
    config.brokerId.value = atoi(argv[3]);
    safelog("receiver id %ld", config.brokerId);
    DEFINE_REQUEST_HANDLERS
}

/**
 * All this handlers send requests to the broker via the broker socket.
 * The process that read the broker response is the clientd_o
 */
void createHandler(request_t request) {
    safelog("create: client pid %d", request.context.clientId);
    send_msg(config.receiveQueue, &request, sizeof(request_t));
}

void publishHandler(request_t request) {
    safelog("publish: topic %s for client %ld", request.body.publish.topic.name, request.context.clientId);
    //map_local_to_global(&request);
    //send_request(config.brokerSocket, &request);
}

void subscribeHandler(request_t request) {
    safelog("subscribe: on topic %s for client %ld", request.body.subscribe.topic.name, request.context.clientId);
    //map_local_to_global(&request);
    // send_request(config.brokerSocket, &request);
}

void receiveHandler(request_t request) {
    safelog("this function was supposed to not be called");
}

void destroyHandler(request_t request) {
    safelog("destroy: for client %ld", request.context.clientId);
    //map_local_to_global(&request);
    //send_request(config.brokerSocket, &request);
}

request_t receive_request() {
    request_t request;
    char *buffer = (char*) &request;
    ssize_t received = 0;
    do {
        received += recv(config.clientFd, &buffer[received], sizeof(request_t) - received, 0);
    } while (received < sizeof(request_t));
    request.context.brokerId = config.brokerId;
    request.mtype = config.brokerId.value;
    return request;
}