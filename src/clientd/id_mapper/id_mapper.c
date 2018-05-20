#include "id_mapper.h"

id_tuple_t* clients_map;
void create_shared_memory();

void init_mapper() {
    int shm = get_shm(CLIENT_IDS_MAP_ID);
    if (shm < 0) {
        perror("Error while mapping shared memory");
        exit(EXIT_FAILURE);
    }
    clients_map = (id_tuple_t*) map_shm(shm);
}

void put(clientId_t globalId, clientId_t localId) {
    for (int i = 0; i < CLIENT_IDS_MAP_CAPACITY; ++i) {
        if (clients_map[i].globalId == 0) {
            clients_map[i].globalId = globalId.value;
            clients_map[i].localId = localId.value;
            return;
        };
    }
    printf("Client id mapper overflow");
    exit(EXIT_FAILURE);
}

clientId_t get_global_id(clientId_t localId) {
    clientId_t response;
    for (int i = 0; i < CLIENT_IDS_MAP_CAPACITY; ++i) {
        if (clients_map[i].localId == localId.value) {
            response.value = clients_map[i].globalId;
            return response;
        };
    }
    response.value = -1;
    return response;
}


clientId_t get_local_id(clientId_t globalId) {
    clientId_t response;
    for (int i = 0; i < CLIENT_IDS_MAP_CAPACITY; ++i) {
        if (clients_map[i].globalId == globalId.value) {
            response.value = clients_map[i].localId;
            return response;
        };
    }
    response.value = -1;
    return response;
}


