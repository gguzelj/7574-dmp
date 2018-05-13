#ifndef INC_7574_DMP_COMMON_H
#define INC_7574_DMP_COMMON_H

/* Error codes */
#define C_OK        0
#define C_ERR       -1

/* Bool definition */
typedef int bool;
#define true        1
#define false       0

#define CLIENT_SERVICE_REQUEST_QUEUE    1
#define CLIENT_SERVICE_RESPONSE_QUEUE   2

enum RequestType {CREATE, PUBLISH, SUBSCRIBE, RECEIVE, DESTROY};

typedef struct id {
    int value;
} id;

typedef struct topic {
    char name[20];
} topic_t;

typedef struct message {
    char value[200];
} message_t;

/* typedef and structs for Requests */
typedef struct createRequest {
    int falopa1;
} createRequest_t;

typedef struct publishRequest {
    topic_t topic;
    message_t message;
} publishRequest_t;

typedef struct subscribeRequest {
    topic_t topic;
    long long r1;
} subscribeRequest_t;

typedef struct receiveRequest {
    long long r1;
} receiveRequest_t;

typedef struct destroyRequest {
} destroyRequest_t;

typedef struct request {
    long mtype;
    enum RequestType type;
    union {
        createRequest_t create;
        publishRequest_t publish;
        subscribeRequest_t subscribe;
        receiveRequest_t receive;
        destroyRequest_t destroy;
    } body;
} request_t;

/* typedef and structs for Responses */
enum status {OK, ERROR};

typedef struct createResponse {
} createResponse_t;

typedef struct publishResponse {
    topic_t topic;
    message_t message;
} publishResponse_t;

typedef struct subscribeResponse {
    topic_t topic;
} subscribeResponse_t;

typedef struct receiveResponse {
    message_t message;
} receiveResponse_t;

typedef struct destroyResponse {
} destroyResponse_t;

typedef struct response {
    long mtype;
    enum status status;
    union {
        createResponse_t create;
        publishResponse_t publish;
        subscribeResponse_t subscribe;
        receiveResponse_t receive;
        destroyResponse_t destroy;
    } body;
} response;

typedef void (*requestHandler)(request_t*);

typedef struct clientConfig {
    int port;
    char* address;
    int brokerSocket;
    int requestQueueId;
    int responseQueueId;
    bool running;
    requestHandler requestHandlers[5];
} clientConfig;
#endif //INC_7574_DMP_COMMON_H
