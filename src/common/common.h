#ifndef INC_7574_DMP_COMMON_H
#define INC_7574_DMP_COMMON_H

/* Error codes */
#define C_OK        0
#define C_ERR       -1

/* Bool definition */
typedef int bool;
#define true        1
#define false       0

/* */
#define CLIENT_SERVICE_QUEUE 1

enum RequestType {CREATE, PUBLISH, SUBSCRIBE, RECEIVE, DESTROY};

typedef struct request {
    long mtype;
    enum RequestType type;
} request;

typedef struct id {
    int value;
} id;

typedef struct topic {
    char *name;
} topic;

typedef struct message {
    char value[100];
} message;

typedef void (*requestHandler)(request*);

typedef struct clientConfig {
    int port;
    char* address;
    int brokerSocket;
    int queueId;
    bool running;
    requestHandler requestHandlers[5];
} clientConfig;
#endif //INC_7574_DMP_COMMON_H
