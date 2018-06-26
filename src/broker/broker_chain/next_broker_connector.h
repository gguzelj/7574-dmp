#ifndef INC_7574_DMP_NEXT_BROKER_CONNECTOR_H
#define INC_7574_DMP_NEXT_BROKER_CONNECTOR_H

#include <stdlib.h>

#include "../../common/broker_connector/broker_connector.h"
#include "../../lib/lockfile/lockfile.h"
#include "../../lib/log/logger.h"
#include "../../common/common.h"
#include "../../lib/msg.h"
#include "../db/db.h"

/*-----------------------------------------------------------------------------
 * Broker worker
 *----------------------------------------------------------------------------*/
brokerConfig config;

void init_connector(int argc, char **argv);
request_t receive_request();

#endif //INC_7574_DMP_NEXT_BROKER_CONNECTOR_H
