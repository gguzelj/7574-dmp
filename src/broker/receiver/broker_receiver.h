#ifndef INC_7574_DMP_BROKER_RECEIVER_H
#define INC_7574_DMP_BROKER_RECEIVER_H

#include <stdlib.h>
#include "../../common/common.h"
#include "../../lib/log/logger.h"

/*-----------------------------------------------------------------------------
 * Broker connection handler responsible for receiving request from clients and
 * forward task to worker
 *----------------------------------------------------------------------------*/
brokerConfig config;
requestHandler requestHandlers[5];

void init_receiver(int argc, char **argv);

request_t receive_request();
requestHandler find_request_handler(request_t request);

void createHandler(request_t request);
void publishHandler(request_t request);
void subscribeHandler(request_t request);
void receiveHandler(request_t request);
void destroyHandler(request_t request);

#endif //INC_7574_DMP_BROKER_RECEIVER_H
