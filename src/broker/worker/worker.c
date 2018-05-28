#include <sys/time.h>
#include "worker.h"

void read_request(request_t *request);

clientId_t create_global_id();

void dispatch(brokerId_t brokerId, topic_t topic, message_t message);

FILE *open_subs_file(topic_t topic, const char* mode);

void subscribe_client(clientId_t clientId, topic_t topic);

response_t copy_request_to_response(request_t request);


int main(int argc, char **argv) {
    init_worker(argc, argv);
    safelog("waiting for requests to process");
    do {
        // request_t request = receive_request();
        request_t request = {0};
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
    srand(time(NULL));
    config.running = true;
    config.receiveQueue = get_msg(atoi(argv[1]));
    config.responseQueue = get_msg(atoi(argv[2]));
    DEFINE_REQUEST_HANDLERS
}

void createHandler(request_t request) {
    safelog("create: client pid %d", request.mtype);
    clientId_t newId = create_global_id();
    response_t createResponse = copy_request_to_response(request);
    createResponse.status.code = OK;
    createResponse.body.create.id = newId;
    send_msg(config.responseQueue, &createResponse, sizeof(response_t));
}

void publishHandler(request_t request) {
    safelog("publish: topic %s for client %ld", request.body.publish.topic.name, request.context.clientId);
    response_t publishResponse = copy_request_to_response(request);
    publishResponse.status.code = OK;

    //dispatch to others...
    dispatch(request.context.brokerId, request.body.publish.topic, request.body.publish.message);

    send_msg(config.responseQueue, &publishResponse, sizeof(response_t));
}

void subscribeHandler(request_t request) {
    safelog("subscribe: on topic %s for client %ld", request.body.subscribe.topic.name, request.context.clientId);
    response_t subscribeResponse = copy_request_to_response(request);
    subscribeResponse.status.code = OK;

    //do stuff
    subscribe_client(request.context.clientId, request.body.subscribe.topic);

    send_msg(config.responseQueue, &subscribeResponse, sizeof(response_t));
}

void receiveHandler(request_t request) {
    safelog("this function was supposed to not be called");
}

void destroyHandler(request_t request) {
    safelog("destroy: for client %ld", request.context.clientId);
}

void read_request(request_t *request) {
    receive_msg(config.receiveQueue, request, sizeof(request_t), 0);
}

request_t receive_request() {
    request_t request;
    receive_msg(config.receiveQueue, &request, sizeof(request_t), 0);
    return request;
}

clientId_t create_global_id() {
    clientId_t globalId;
    globalId.value = rand();
    globalId.value = (globalId.value << 32) | rand();
    globalId.value = (globalId.value % (999999999 - 100000000)) + 100000000;
    return globalId;
}

void dispatch(brokerId_t brokerId, topic_t topic, message_t message) {
    FILE *fd = open_subs_file(topic, "r");
    if (fd == NULL) {
        safelog("no subscriptions for topic %s", topic.name);
        return;
    }

    response_t receiveResponse = {0};

    char * line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fd) != -1) {
        receiveResponse.type = RECEIVE;
        receiveResponse.mtype = brokerId.value;
        receiveResponse.status.code = OK;
        receiveResponse.body.receive.message = message;
        receiveResponse.body.receive.topic = topic;
        receiveResponse.context.clientId.value = atoi(line);
        receiveResponse.context.brokerId = brokerId;

        safelog("publishing message to client %s", line);
        send_msg(config.responseQueue, &receiveResponse, sizeof(response_t));
    }

    fclose(fd);
    if (line)
        free(line);
}

void subscribe_client(clientId_t clientId, topic_t topic) {
    FILE *fd = open_subs_file(topic, "a");
    if (fd == NULL) {
        safelog("unexpected error while creating subs file %s", topic.name);
        exit(EXIT_FAILURE);
    }
    //TODO check already subscribed
    fprintf(fd, "%ld\n", clientId.value);
    fclose(fd);
}

FILE * open_subs_file(topic_t topic, const char* mode) {
    char filename[100];
    strcpy(filename, BROKER_DB_SUBS_FOLDER);
    strcat(filename, topic.name);
    strcat(filename, BROKER_DB_SUBS_EXT);
    return fopen(filename, mode);
}

response_t copy_request_to_response(request_t request) {
    response_t response;
    response.mtype = request.mtype;
    response.type = request.type;
    response.context.clientId = request.context.clientId;
    response.context.brokerId = request.context.brokerId;
    response.body.publish.message = request.body.publish.message;
    response.body.publish.topic = request.body.publish.topic;
    response.body.subscribe.topic = request.body.subscribe.topic;
    return response;
}