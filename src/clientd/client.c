#include "client.h"

void build_create_request(request_t*);
void build_publish_request(request_t*, clientId_t, topic_t, message_t);

void create(clientId_t* clientId) {
    request_t createRequest = {0};
    build_create_request(&createRequest);
    send_request(&createRequest);

    response_t response;
    receive_clientId(&response);

    *clientId = response.body.create.id;
}

int publish(clientId_t id, message_t message, topic_t topic) {
    request_t publishRequest = {0};
    build_publish_request(&publishRequest, id, topic, message);
    send_request(&publishRequest);

    response_t response;
    receive_response(CLIENT_SERVICE_RESPONSE_QUEUE, id, &response);

    return response.status.code;
}

status_t* subscribe(clientId_t id, topic_t topic) {
    return 0;
}

message_t* receive(clientId_t id) {
    return 0;
}

status_t* destroy(clientId_t id) {
    return 0;
}

int send_request(request_t* request) {
    int requestQueue = get_msg(CLIENT_SERVICE_REQUEST_QUEUE);
    send_msg(requestQueue, request, sizeof(request_t));
    return OK;
}

void receive_clientId(response_t *response) {
    clientId_t clientId = {getpid()};
    receive_response(CLIENT_SERVICE_CLIEND_ID_QUEUE, clientId, response);
}

void receive_response(int queueId, clientId_t clientId, response_t *response) {
    int requestQueue = get_msg(queueId);
    receive_msg(requestQueue, response, sizeof(response_t), clientId.value);
    if (response->status.code == ERROR) {
        printf("Unexpected error while creating new instance: %s", response->status.description.value);
        exit(EXIT_FAILURE);
    }
}

void build_create_request(request_t *createRequest) {
    createRequest->type = CREATE;
    createRequest->mtype = getpid();
}

void build_publish_request(request_t *publishRequest, clientId_t id, topic_t topic, message_t message) {
    publishRequest->type = PUBLISH;
    publishRequest->mtype = id.value;
    publishRequest->body.publish.id = id;
    publishRequest->body.publish.topic = topic;
    publishRequest->body.publish.message = message;
}