#include "worker.h"

void read_request(request_t *request);

int main(int argc, char **argv) {
    init_worker(argc, argv);
    safelog("waiting for requests to process");
    do {
        // request_t request = receive_request();
        request_t request = {0} ;
        read_request(&request);
        find_request_handler(request)(request);
    } while (config.running);
    return EXIT_SUCCESS;
}

requestHandler find_request_handler(request_t request) {
    return requestHandlers[request.type];
}

void init_worker(int argc, char **argv) {
    init_logger("worker");
    config.running = true;
    config.receiveQueue =get_msg(atoi(argv[1]));
    config.responseQueue = get_msg(atoi(argv[2]));
    DEFINE_REQUEST_HANDLERS
}

void createHandler(request_t request) {
    safelog("create: client pid %d", request.mtype);
    int newId = rand();
    response_t createResponse;
    createResponse.type = request.type;
    createResponse.mtype = request.mtype;
    createResponse.status.code = OK;
    createResponse.id.value = newId;
    send_msg(config.responseQueue, &createResponse, sizeof(response_t));
}

void publishHandler(request_t request) {
    safelog("publish: topic %s for client %ld", request.body.publish.topic.name, request.id);
    //map_local_to_global(&request);
    //send_request(config.brokerSocket, &request);
}

void subscribeHandler(request_t request) {
    safelog("subscribe: on topic %s for client %ld", request.body.subscribe.topic.name, request.id);
    //map_local_to_global(&request);
    // send_request(config.brokerSocket, &request);
}

void receiveHandler(request_t request) {
    safelog("this function was supposed to not be called");
}

void destroyHandler(request_t request) {
    safelog("destroy: for client %ld", request.id);
    //map_local_to_global(&request);
    //send_request(config.brokerSocket, &request);
}

void read_request(request_t *request) {
    receive_msg(config.receiveQueue, request, sizeof(request_t), 0);
}

request_t receive_request() {
    request_t request;
    receive_msg(config.receiveQueue , &request, sizeof(request_t), 0);
    return request;
}