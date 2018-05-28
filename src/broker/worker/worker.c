#include "worker.h"
#include "../../common/common.h"

void read_request(request_t *request);
clientId_t create_global_id();

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
    clientId_t newId = create_global_id();
    response_t createResponse;
    createResponse.type = request.type;
    createResponse.mtype = request.mtype;
    createResponse.context.clientId = request.context.clientId;
    createResponse.context.brokerId = request.context.brokerId;
    createResponse.status.code = OK;
    createResponse.body.create.id = newId;
    send_msg(config.responseQueue, &createResponse, sizeof(response_t));
}

void publishHandler(request_t request) {
    safelog("publish: topic %s for client %ld", request.body.publish.topic.name, request.context.clientId);
    response_t publishResponse = {0} ;
    publishResponse.type = request.type;
    publishResponse.mtype = request.mtype;
    publishResponse.status.code = OK;
    publishResponse.context.brokerId = request.context.brokerId;
    publishResponse.context.clientId = request.context.clientId;
    publishResponse.body.publish.topic = request.body.publish.topic;
    publishResponse.body.publish.message = request.body.publish.message;


    //dispatch to others...

    send_msg(config.responseQueue, &publishResponse, sizeof(response_t));
}

void subscribeHandler(request_t request) {
    safelog("subscribe: on topic %s for client %ld", request.body.subscribe.topic.name, request.context.clientId);

    response_t subscribeResponse = {0} ;
    subscribeResponse.type = request.type;
    subscribeResponse.mtype = request.mtype;
    subscribeResponse.status.code = OK;
    subscribeResponse.context.brokerId = request.context.brokerId;
    subscribeResponse.context.clientId = request.context.clientId;
    subscribeResponse.body.subscribe.topic = request.body.subscribe.topic;

    //do stuff

    send_msg(config.responseQueue, &subscribeResponse, sizeof(response_t));
}

void receiveHandler(request_t request) {
    safelog("this function was supposed to not be called");
}

void destroyHandler(request_t request) {
    safelog("destroy: for client %ld", request.context.clientId);
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

clientId_t create_global_id() {
    clientId_t globalId;
    globalId.value = rand();
    globalId.value = (globalId.value << 32) | rand();
    globalId.value = (globalId.value % (999999999 - 100000000)) + 100000000;
    return globalId;
}