#include "clientd_o.h"
#include "../../common/common.h"

void create_local_id(clientId_t*);

int main(int argc, char **argv) {
    init_daemon(argc, argv);
    safelog("running responder");
    do {
        response_t response;
        receive_response(config.brokerSocket, &response);
        responseHandler handler = find_response_handler(response);
        handler(response);
    } while (config.running);
    return EXIT_SUCCESS;
}

void init_daemon(int argc, char **argv) {
    init_logger("Client responder");
    init_mapper();
    srand(time(NULL));
    config.running = true;
    config.brokerSocket = atoi(argv[1]);
    config.responseQueueId = get_msg(atoi(argv[2]));
    config.clientIdQueueId = get_msg(atoi(argv[3]));
    responseHandlers[CREATE] = &createHandler;
    responseHandlers[PUBLISH] = &publishHandler;
    responseHandlers[SUBSCRIBE] = &subscribeHandler;
    responseHandlers[RECEIVE] = &receiveHandler;
    responseHandlers[DESTROY] = &destroyHandler;
}

responseHandler find_response_handler(response_t response) {
    return responseHandlers[response.type];
}

void createHandler(response_t response) {
    safelog("create response received for pid %d. Id assigned %d", response.mtype, response.body.create.id.value);
    clientId_t localId = {0};
    create_local_id(&localId);
    safelog("assigning local id %ld to global id %ld", localId.value, response.body.create.id.value);
    put(response.body.create.id, localId);
    response.body.create.id = localId;
    send_msg(config.clientIdQueueId, &response, sizeof(response_t));
}

void publishHandler(response_t response) {
    safelog("publish response received for id %d", response.body.publish.id.value);
    send_msg(config.responseQueueId, &response, sizeof(response_t));
}

void subscribeHandler(response_t request) {
    safelog("Subscribe handler invoked");
}

void receiveHandler(response_t request) {
    safelog("Create handler invoked");
}

void destroyHandler(response_t request) {
    printf("destroyHandler invoked");
}

void create_local_id(clientId_t* localId) {
    localId->value = rand();
    localId->value = (localId->value << 32) | rand();
    localId->value = (localId->value % (999999999 - 100000000)) + 100000000;
}