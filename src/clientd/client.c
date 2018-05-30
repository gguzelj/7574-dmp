#include "client.h"

request_t build_create_request();

request_t build_publish_request(clientId_t, topic_t, message_t);

request_t build_subscribe_request(clientId_t, topic_t);

request_t build_destroy_request(clientId_t);

void create(clientId_t *clientId) {
    send_request(build_create_request());
    clientId_t pid;
    pid.value = getpid();
    response_t response = receive_response(CLIENT_SERVICE_CLIEND_ID_QUEUE, pid);
    *clientId = response.body.create.id;
}

int publish(clientId_t id, message_t message, topic_t topic) {
    send_request(build_publish_request(id, topic, message));
    response_t response = receive_response(CLIENT_SERVICE_RESPONSE_QUEUE, id);
    return response.status.code;
}

int subscribe(clientId_t id, topic_t topic) {
    send_request(build_subscribe_request(id, topic));
    response_t response = receive_response(CLIENT_SERVICE_RESPONSE_QUEUE, id);
    return response.status.code;
}

int receive(clientId_t id, topic_t *topic, message_t *message) {
    response_t response = receive_response(CLIENT_SERVICE_RECEIVE_QUEUE, id);
    *message = response.body.receive.message;
    *topic = response.body.receive.topic;
    return response.status.code;
}

int destroy(clientId_t id) {
    send_request(build_destroy_request(id));
    response_t response = receive_response(CLIENT_SERVICE_RESPONSE_QUEUE, id);
    return response.status.code;
}

int send_request(request_t request) {
    int requestQueue = get_msg(CLIENT_SERVICE_REQUEST_QUEUE);
    send_msg(requestQueue, &request, sizeof(request_t));
    return OK;
}

response_t receive_response(int queueId, clientId_t clientId) {
    int requestQueue = get_msg(queueId);
    response_t response;
    receive_msg(requestQueue, &response, sizeof(response_t), clientId.value);
    if (response.status.code == ERROR) {
        printf("Unexpected error while creating new instance: %s", response.status.description.value);
        exit(EXIT_FAILURE);
    }
    return response;
}

request_t build_create_request() {
    request_t createRequest = {0};
    createRequest.type = CREATE;
    createRequest.mtype = getpid();
    return createRequest;
}

request_t build_publish_request(clientId_t id, topic_t topic, message_t message) {
    request_t publishRequest = {0};
    publishRequest.type = PUBLISH;
    publishRequest.mtype = id.value;
    publishRequest.context.clientId = id;
    publishRequest.body.publish.topic = topic;
    publishRequest.body.publish.message = message;
    return publishRequest;
}

request_t build_subscribe_request(clientId_t id, topic_t topic) {
    request_t subscribeRequest = {0};
    subscribeRequest.type = SUBSCRIBE;
    subscribeRequest.mtype = id.value;
    subscribeRequest.context.clientId = id;
    subscribeRequest.body.subscribe.topic = topic;
    return subscribeRequest;
}

request_t build_destroy_request(clientId_t id) {
    request_t destroyRequest = {0};
    destroyRequest.type = DESTROY;
    destroyRequest.mtype = id.value;
    destroyRequest.context.clientId = id;
    return destroyRequest;
}