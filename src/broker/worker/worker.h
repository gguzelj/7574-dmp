#ifndef INC_7574_DMP_WORKER_H
#define INC_7574_DMP_WORKER_H

#include <stdlib.h>
#include "../../lib/lockfile/lockfile.h"
#include "../../lib/log/logger.h"
#include "../../common/common.h"
#include "../../lib/msg.h"

/*-----------------------------------------------------------------------------
 * Broker worker
 *----------------------------------------------------------------------------*/
brokerConfig config;
requestHandler requestHandlers[5];

void init_worker(int argc, char **argv);
requestHandler find_request_handler(request_t request);

request_t receive_request();

void createHandler(request_t request);
void publishHandler(request_t request);
void subscribeHandler(request_t request);
void receiveHandler(request_t request);
void destroyHandler(request_t request);
#endif //INC_7574_DMP_WORKER_H
