#include "client.h"
#include "../lib/msg.h"
#include "../common/common.h"

void create(clientId_t* clientId) {
    request_t createRequest = {0};
    response_t response;

    createRequest.type = CREATE;
    createRequest.mtype = getpid();

    send_request(&createRequest);
    receive_response(&response);
    printf("returning");
    fflush(stdout);

    if (response.status.code == ERROR) {
        printf("Unexpected error while creating new instance: %s", response.status.description.value);
        exit(EXIT_FAILURE);
    }
    *clientId = response.body.create.id;
}

int publish(clientId_t id, message_t message, topic_t topic) {
    request_t publishRequest = {0};
    response_t response;

    publishRequest.type = PUBLISH;
    publishRequest.mtype = getpid();
    publishRequest.body.publish.id = id;
    publishRequest.body.publish.topic = topic;
    publishRequest.body.publish.message = message;

    send_request(&publishRequest);
    receive_response(&response);

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

void receive_response(response_t *response) {
    int requestQueue = get_msg(CLIENT_SERVICE_RESPONSE_QUEUE);
    receive_msg(requestQueue, response, sizeof(response_t), getpid());
}
