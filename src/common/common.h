#ifndef INC_7574_DMP_COMMON_H
#define INC_7574_DMP_COMMON_H

#define COPY(target, source) snprintf(target, sizeof(target), "%d", source);
#define DEFINE_REQUEST_HANDLERS                             \
            requestHandlers[CREATE] = &createHandler;       \
            requestHandlers[PUBLISH] = &publishHandler;     \
            requestHandlers[SUBSCRIBE] = &subscribeHandler; \
            requestHandlers[RECEIVE] = &receiveHandler;     \
            requestHandlers[DESTROY] = &destroyHandler;

#define DEFINE_RESPONSE_HANDLERS                                \
            responseHandlers[CREATE] = &createHandler;          \
            responseHandlers[PUBLISH] = &publishHandler;        \
            responseHandlers[SUBSCRIBE] = &subscribeHandler;    \
            responseHandlers[RECEIVE] = &receiveHandler;        \
            responseHandlers[DESTROY] = &destroyHandler;



/* Bool definition */
typedef int bool;
#define true        1
#define false       0

#define CLIENT_SERVICE_REQUEST_QUEUE    1
#define CLIENT_SERVICE_RESPONSE_QUEUE   2
#define CLIENT_SERVICE_CLIEND_ID_QUEUE  3
#define CLIENT_SERVICE_RECEIVE_QUEUE    4
#define CLIENT_IDS_MAP_ID               5
#define CLIENT_IDS_MAP_CAPACITY         1000
#define CLIENT_PID_FILE                 "/tmp/cliendd.pid"

#define BROKER_RECEIVE_QUEUE            6
#define BROKER_RESPONSE_QUEUE           7
#define BROKER_CAPACITY                 100
#define BROKER_LISTENING_PORT           8000
#define BROKER_AMOUNT_WORKERS           1

#define BROKER_DB_SUBS_FOLDER           "/tmp/subs/"
#define BROKER_DB_SUBS_EXT              ".db"

#include <netinet/in.h>

typedef struct id_tuple {
    long globalId;
    long localId;
} id_tuple_t;

enum RequestType {
    CREATE, PUBLISH, SUBSCRIBE, RECEIVE, DESTROY
};

typedef struct clientId {
    long value;
} clientId_t;

typedef struct brokerId {
    long value;
} brokerId_t;

typedef struct topic {
    char name[20];
} topic_t;

typedef struct message {
    char value[200];
} message_t;

typedef struct context {
    clientId_t clientId;
    brokerId_t brokerId;
} context_t;

/* typedef and structs for Requests */
typedef struct publishRequest {
    topic_t topic;
    message_t message;
} publishRequest_t;

typedef struct subscribeRequest {
    topic_t topic;
} subscribeRequest_t;

typedef struct request {
    long mtype;
    context_t context;
    enum RequestType type;
    union {
        publishRequest_t publish;
        subscribeRequest_t subscribe;
    } body;
} request_t;

/* typedef and structs for Responses */
enum statusCode {
    OK, ERROR
};

typedef struct status {
    enum statusCode code;
    message_t description;
} status_t;

typedef struct createResponse {
    clientId_t id;
} createResponse_t;

typedef struct publishResponse {
    topic_t topic;
    message_t message;
} publishResponse_t;

typedef struct subscribeResponse {
    topic_t topic;
} subscribeResponse_t;

typedef struct receiveResponse {
    topic_t topic;
    message_t message;
} receiveResponse_t;

typedef struct destroyResponse {
} destroyResponse_t;

typedef struct response {
    long mtype;
    context_t context;
    enum RequestType type;
    status_t status;
    union {
        createResponse_t create;
        publishResponse_t publish;
        subscribeResponse_t subscribe;
        receiveResponse_t receive;
        destroyResponse_t destroy;
    } body;
} response_t;

typedef void (*requestHandler)(request_t);
typedef void (*responseHandler)(response_t);

typedef struct clientConfig {
    int port;
    char *address;
    int brokerSocket;
    int requestQueueId;
    int responseQueueId;
    int clientIdQueueId;
    int receiveQueueId;
    bool running;
} clientConfig;

typedef struct brokerConfig {
    int port;
    bool running;
    int brokerFd;
    int clientFd;
    int capacity;
    struct sockaddr_in address;
    int receiveQueue;
    int responseQueue;
    brokerId_t brokerId;
} brokerConfig;

#endif //INC_7574_DMP_COMMON_H
