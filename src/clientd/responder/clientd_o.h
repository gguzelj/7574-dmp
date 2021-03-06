#ifndef INC_7574_DMP_CLIENTD_O_H
#define INC_7574_DMP_CLIENTD_O_H

#include <stdlib.h>
#include "../../common/broker_connector/broker_connector.h"
#include "../../common/common.h"
#include "../../lib/lockfile/lockfile.h"
#include "../../lib/log/logger.h"
#include "../../lib/msg.h"
#include "../id_mapper/id_mapper.h"
#include <time.h>

/*-----------------------------------------------------------------------------
 * Client daemon responsible for dispatch requests from broker to corresponding
 * local process. Id mapping (from global to local) happens here.
 *----------------------------------------------------------------------------*/
clientConfig config;
responseHandler responseHandlers[5];

void init_daemon(int argc, char **argv);

void read_response(response_t*);
responseHandler find_response_handler(response_t request);

void createHandler(response_t);
void publishHandler(response_t);
void subscribeHandler(response_t);
void receiveHandler(response_t);
void destroyHandler(response_t);

#endif //INC_7574_DMP_CLIENTD_O_H
