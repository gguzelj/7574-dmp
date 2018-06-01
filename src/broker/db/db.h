#ifndef INC_7574_DMP_DB_H
#define INC_7574_DMP_DB_H

#include <memory.h>
#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../common/common.h"
#include "../../lib/log/logger.h"

void add_client_to_broker_file(clientId_t clientId, brokerId_t brokerId);
void add_client_to_topic_file(clientId_t clientId, topic_t topic);
void remove_client_from_topic_file(clientId_t clientId, topic_t topic);
void add_topic_to_client_file(clientId_t clientId, topic_t topic);
void remove_client(clientId_t clientId);

#endif //INC_7574_DMP_DB_H
