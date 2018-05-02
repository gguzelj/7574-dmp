#ifndef INC_7574_DMP_CLIENT_H
#define INC_7574_DMP_CLIENT_H

#include "../common/common.h"
#include <stdlib.h>

int init_service(int argc, char **argv);

/**
 * Creates a new connection with the broker
 * @return an ID that identifies the new connection
 */
id* create(void);
int publish(id id, message message, topic topic);
int subscribe(id id, topic topic);
int receive(id id);
int destroy(id id);

#endif //INC_7574_DMP_CLIENT_H
