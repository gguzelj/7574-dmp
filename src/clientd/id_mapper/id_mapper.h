#ifndef INC_7574_DMP_ID_MAPPER_H
#define INC_7574_DMP_ID_MAPPER_H

#include "../../common/common.h"
#include "../../lib/shm.h"

void init_mapper();

void put(clientId_t globalId, clientId_t localId);
void delete(clientId_t globalId);
clientId_t get_global_id(clientId_t localId);
clientId_t get_local_id(clientId_t localId);


#endif //INC_7574_DMP_ID_MAPPER_H
