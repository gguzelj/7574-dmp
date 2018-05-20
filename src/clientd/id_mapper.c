#include "id_mapper.h"

ids_map* clients_map;
void create_shared_memory();

void init(int argc, char **argv) {
    int shm = get_shm(CLIENT_IDS_MAP_ID);
    clients_map = (ids_map*) map_shm(shm);
}

void put(clientId_t globalId, clientId_t localId) {
    for (int i = 0; i < CLIENT_IDS_MAP_CAPACITY; ++i) {
        if (clients_map[i]->globalId == 0) {
            clients_map[i]->globalId = globalId.value;
            clients_map[i]->localId = localId.value;
            return;
        };
    }
    printf("Client id mapper overflow");
    exit(EXIT_FAILURE);
}

void get(clientId_t globalId, clientId_t* localId) {
    for (int i = 0; i < CLIENT_IDS_MAP_CAPACITY; ++i) {
        if (clients_map[i]->globalId == globalId.value) {
            localId->value = clients_map[i]->localId;
            return;
        };
    }
    printf("Client id mapper overflow");
    exit(EXIT_FAILURE);
}


