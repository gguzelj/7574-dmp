#include "clientd_o.h"
#include "../../common/common.h"

clientId_t create_local_id();
void map_global_to_local(response_t *response);

int main(int argc, char **argv) {
    init_daemon(argc, argv);
    safelog("running responder");
    do {
        response_t response;
        receive_response(config.brokerSocket, &response);
        find_response_handler(response)(response);
    } while (config.running);
    return EXIT_SUCCESS;
}

void init_daemon(int argc, char **argv) {
    init_logger("client responder");
    init_mapper();
    srand(time(NULL));
    config.running = true;
    config.brokerSocket = atoi(argv[1]);
    config.responseQueueId = get_msg(atoi(argv[2]));
    config.clientIdQueueId = get_msg(atoi(argv[3]));
    config.receiveQueueId = get_msg(atoi(argv[4]));
    DEFINE_RESPONSE_HANDLERS
}

responseHandler find_response_handler(response_t response) {
    return responseHandlers[response.type];
}

void createHandler(response_t response) {
    clientId_t localId = create_local_id();
    safelog("create: client pid %d. gid: %ld. lid: %ld", response.mtype, response.body.create.id, localId);
    put(response.body.create.id, localId);
    map_global_to_local(&response);
    send_msg(config.clientIdQueueId, &response, sizeof(response_t));
}

void publishHandler(response_t response) {
    safelog("publish: topic %s for client %ld", response.body.publish.topic.name, response.context.clientId);
    map_global_to_local(&response);
    send_msg(config.responseQueueId, &response, sizeof(response_t));
}

void subscribeHandler(response_t response) {
    safelog("subscribe: topic %s for client %ld", response.body.subscribe.topic.name, response.context.clientId);
    map_global_to_local(&response);
    send_msg(config.responseQueueId, &response, sizeof(response_t));
}

void receiveHandler(response_t response) {
    safelog("receive: new message from broker. topic %s for client %ld", response.body.receive.topic.name, response.context.clientId);
    map_global_to_local(&response);
    send_msg(config.receiveQueueId, &response, sizeof(response_t));
}

void destroyHandler(response_t response) {
    safelog("destroy: for client %ld", response.context.clientId);
    clientId_t globalId = response.context.clientId;
    map_global_to_local(&response);
    delete(globalId);
    send_msg(config.responseQueueId, &response, sizeof(response_t));
}

clientId_t create_local_id() {
    clientId_t localId;
    localId.value = rand();
    localId.value = (localId.value << 32) | rand();
    localId.value = (localId.value % (999999999 - 100000000)) + 100000000;
    return localId;
}

void map_global_to_local(response_t *response) {
    clientId_t globalId = response->body.create.id;
    response->body.create.id = get_local_id(globalId);
    if (response->body.create.id.value < 0) {
        safelog("wrong globalId %ld", globalId);
    }
}