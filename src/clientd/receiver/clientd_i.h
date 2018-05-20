#ifndef INC_7574_DMP_CLIENTD_I_H
#define INC_7574_DMP_CLIENTD_I_H

#include <stdlib.h>
#include "../broker_connector/broker_connector.h"
#include "../../common/common.h"
#include "../../lib/lockfile/lockfile.h"
#include "../../lib/log/logger.h"
#include "../../lib/msg.h"
#include "../id_mapper/id_mapper.h"

/*-----------------------------------------------------------------------------
 * Client service data structures, defines, exported API, etc...
 *----------------------------------------------------------------------------*/
clientConfig config;
requestHandler requestHandlers[5];

void init_daemon(int argc, char **argv);

void read_request(request_t* request);
requestHandler find_request_handler(request_t request);

void createHandler(request_t request);
void publishHandler(request_t request);
void subscribeHandler(request_t request);
void receiveHandler(request_t request);
void destroyHandler(request_t request);

#endif //INC_7574_DMP_CLIENTD_I_H
