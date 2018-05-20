#ifndef INC_7574_DMP_CLIENT_H
#define INC_7574_DMP_CLIENT_H

#include <stdlib.h>
#include "../common/common.h"
#include "../lib/msg.h"

void create(clientId_t*);
int publish(clientId_t id, message_t message, topic_t topic);
int subscribe(clientId_t id, topic_t topic);
int receive(clientId_t id, message_t *message);
int destroy(clientId_t id);

int send_request(request_t* request);
void receive_response(int queueId, clientId_t clientId, response_t *response);
void receive_clientId(response_t *response);
#endif //INC_7574_DMP_CLIENT_H
