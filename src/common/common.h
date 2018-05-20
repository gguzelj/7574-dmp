#ifndef INC_7574_DMP_COMMON_H
#define INC_7574_DMP_COMMON_H

/* Bool definition */
typedef int bool;
#define true        1
#define false       0

#define CLIENT_SERVICE_REQUEST_QUEUE    1
#define CLIENT_SERVICE_RESPONSE_QUEUE   2
#define CLIENT_SERVICE_CLIEND_ID_QUEUE  3
#define CLIENT_IDS_MAP_ID               4
#define CLIENT_IDS_MAP_CAPACITY         1000

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
    clientId_t id;
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
    clientId_t id;
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
    status_t status;
    enum RequestType type;
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
    bool running;
} clientConfig;
#endif //INC_7574_DMP_COMMON_H
