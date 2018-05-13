#ifndef INC_7574_DMP_CLIENT_H
#define INC_7574_DMP_CLIENT_H

#include "../common/common.h"
#include <stdlib.h>

int init_service(int argc, char **argv);

/**
 * Creates a new connection with the broker
 * @return an ID that identifies the new connection
 */
clientId_t* create();
status_t* publish(clientId_t id, message_t message, topic_t topic);
status_t* subscribe(clientId_t id, topic_t topic);
message_t* receive(clientId_t id);
status_t* destroy(clientId_t id);

int send_request(request_t* request);
response_t *receive_response();
#endif //INC_7574_DMP_CLIENT_H
