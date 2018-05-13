#ifndef INC_7574_DMP_MAP_H
#define INC_7574_DMP_MAP_H

#include <stdlib.h>

typedef int key;
typedef char* value;

typedef struct entry {
    key key;
    value value;
    struct entry* next;
} entry_t;


typedef struct map {
    entry_t* first;
} map_t;

entry_t* entry_init(key key, value value);
void entry_destroy(entry_t* entry);

map_t* map_init();
void map_destroy(map_t *map);
void put(map_t*, key, value);
value get(map_t*, key);

#endif //INC_7574_DMP_MAP_H
