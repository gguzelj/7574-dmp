#include "map.h"

entry_t* entry_init(key key, value value) {
    entry_t* entry = malloc(sizeof(entry));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    return entry;
}

void entry_destroy(entry_t* entry);

map_t* map_init() {
    map_t* map = malloc(sizeof(map_t));
    map->first = NULL;
    return map;
}

void map_destroy(map_t* map) {
    entry_t *entry = map->first;
    while (entry != NULL) {
        entry_t* next = entry->next;
        free(entry);
        entry = next;
    }
    free(map);
}

void put(map_t* map, key key, value value){
    if (map->first == NULL) {
        map->first = entry_init(key, value);
        return;
    }
    entry_t *entry = map->first;
    while (entry != NULL) {
        if (key == entry->key) {
            entry->value = value;
            return;
        }
        if (entry->next == NULL) {
            entry->next = entry_init(key, value);
            return;
        }
        entry = entry->next;
    }
}

value get(map_t* map, key key) {
    entry_t *entry = map->first;
    while (entry != NULL) {
        if (key == entry->key) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}
