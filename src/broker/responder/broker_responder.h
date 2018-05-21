#ifndef INC_7574_DMP_BROKER_RESPONDER_H
#define INC_7574_DMP_BROKER_RESPONDER_H

#include <stdlib.h>
#include <sys/socket.h>
#include "../../common/common.h"
#include "../../lib/log/logger.h"
#include "../../lib/msg.h"

/*-----------------------------------------------------------------------------
 * Broker connection handler responsible for receiving request from clients and
 * forward task to worker
 *----------------------------------------------------------------------------*/
brokerConfig config;
responseHandler responseHandlers[5];

void init_responder(int argc, char **argv);

response_t receive_response();
responseHandler find_response_handler(response_t response);

void createHandler(response_t response);
void publishHandler(response_t response);
void subscribeHandler(response_t response);
void receiveHandler(response_t response);
void destroyHandler(response_t response);

#endif //INC_7574_DMP_BROKER_RESPONDER_H
