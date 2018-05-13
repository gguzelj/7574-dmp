#include "client.h"
#include "../lib/msg.h"

clientId_t *create() {
    request_t createRequest = {0};
    createRequest.type = CREATE;
    createRequest.mtype = getpid();

    send_request(&createRequest);
    response_t *response = receive_response();

    if (response->status.code == ERROR) {
        printf("Unexpected error while creating new instance: %s", response->status.description.value);
        exit(EXIT_FAILURE);
    }
    clientId_t* id = malloc(sizeof(id_t));
    *id = response->body.create.id;
    free(response);
    return id;
}

status_t* publish(clientId_t id, message_t message, topic_t topic) {
    request_t publishRequest;
    publishRequest.type = PUBLISH;
    publishRequest.mtype = getpid();
    publishRequest.body.publish.topic = topic;
    publishRequest.body.publish.message = message;

    send_request(&publishRequest);
    response_t *response = receive_response();
    if (response->status.code == ERROR) {
        printf("Unexpected error while publishing: %s", response->status.description.value);
        exit(EXIT_FAILURE);
    }
    status_t* status = malloc(sizeof(status_t));
    *status = response->status;
    free(response);
    return status;
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
}

response_t *receive_response() {
    int requestQueue = get_msg(CLIENT_SERVICE_RESPONSE_QUEUE);
    response_t response;
    receive_msg(requestQueue, &response, sizeof(response_t), 0);
    response_t *returnValue = malloc(sizeof(response_t));
    memcpy(returnValue, &response, sizeof(response_t));
    return returnValue;
}
