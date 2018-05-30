#include <sys/time.h>
#include "worker.h"

void read_request(request_t *request);

clientId_t create_global_id();

void dispatch(brokerId_t brokerId, topic_t topic, message_t message);

void subscribe_client(clientId_t clientId, topic_t topic);

response_t copy_request_to_response(request_t request);

brokerId_t find_broker_id(clientId_t clientId);

void add_client_to_topic_file(clientId_t id, topic_t topic);

void add_client_to_clients_file(clientId_t id, brokerId_t brokerId);

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

    add_client_to_clients_file(newId, request.context.brokerId);

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
    char filename[100];
    TOPIC_FILE(filename, topic.name);
    FILE *fd = fopen(filename, "r");

    if (fd == NULL) {
        safelog("no subscriptions for topic %s", topic.name);
        return;
    }

    response_t receiveResponse = {0};

    long clientId;
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fd) != -1) {
        clientId = atoi(line);
        receiveResponse.type = RECEIVE;
        receiveResponse.mtype = brokerId.value;
        receiveResponse.status.code = OK;
        receiveResponse.body.receive.message = message;
        receiveResponse.body.receive.topic = topic;
        receiveResponse.context.clientId.value = clientId;
        receiveResponse.context.brokerId = find_broker_id(receiveResponse.context.clientId);

        safelog("publishing message to client %s with broker id %ld", line, receiveResponse.context.brokerId);
        send_msg(config.responseQueue, &receiveResponse, sizeof(response_t));
    }

    fclose(fd);
    if (line)
        free(line);
}

void subscribe_client(clientId_t clientId, topic_t topic) {
    add_client_to_topic_file(clientId, topic);
}

void add_client_to_topic_file(clientId_t clientId, topic_t topic) {
    char filename[100];
    TOPIC_FILE(filename, topic.name);
    char* mode = (access(filename, F_OK) != -1) ? "r" : "a+";

    FILE *fd = fopen(filename, mode);
    if (fd == NULL) {
        safelog("unexpected error while opening subs file %s", topic.name);
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    char id[100];
    snprintf(id, sizeof(id), "%ld", clientId.value);

    while (getline(&line, &len, fd) != -1) {
        if (strcmp(id, line) == 0) {
            safelog("client with id %ld already subscribed", clientId);
            return;
        }
    }

    safelog("adding client %ld subscription to topic %s", clientId.value, topic.name);
    fprintf(fd, "%ld\n", clientId.value);
    fclose(fd);
}

void add_client_to_clients_file(clientId_t clientId, brokerId_t brokerId) {
    char filename[100];
    CLIENT_FILE(filename, clientId);
    FILE *fd = fopen(filename, "a+");
    if (fd == NULL) {
        safelog("unexpected error while creating client id file");
        exit(EXIT_FAILURE);
    }
    fprintf(fd, "%ld,%ld", clientId.value, brokerId.value);
    fclose(fd);
}


brokerId_t find_broker_id(clientId_t clientId) {
    brokerId_t response;
    char filename[100];
    CLIENT_FILE(filename, clientId);
    if (access(filename, F_OK) == -1) {
        safelog("No broker id found for client id %ld", clientId);
        response.value = -1;
        return response;
    }
    FILE *fd = fopen(filename, "r");
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fd) != -1) {
        strtok(line, ",");
        response.value = atoi(strtok (NULL, ","));
        return response;
    }
    response.value = -1;
    return response;
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