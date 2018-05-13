#ifndef INC_7574_DMP_CLIENTD_I_H
#define INC_7574_DMP_CLIENTD_I_H

#include <stdlib.h>
#include "connector/broker_connector.h"
#include "../common/common.h"
#include "../lib/lockfile/lockfile.h"
#include "../lib/log/logger.h"
#include "../lib/msg.h"

/*-----------------------------------------------------------------------------
 * Client service data structures, defines, exported API, etc...
 *----------------------------------------------------------------------------*/
clientConfig config;
requestHandler requestHandlers[5];

void init_daemon(int argc, char **argv);

request_t* read_request(void);
requestHandler find_request_handler(request_t* request);

void createHandler(request_t*);
void publishHandler(request_t*);
void subscribeHandler(request_t*);
void receiveHandler(request_t*);
void destroyHandler(request_t*);

#endif //INC_7574_DMP_CLIENTD_I_H
