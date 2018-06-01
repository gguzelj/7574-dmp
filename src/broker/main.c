#include <sys/stat.h>
#include "./db/db.h"


int main() {    struct stat st = {0};


    if (stat(BROKER_DB_TOPIC_FOLDER, &st) == -1) {
        mkdir(BROKER_DB_TOPIC_FOLDER, 0700);
    }

    if (stat(BROKER_DB_CLIENTS_FOLDER, &st) == -1) {
        mkdir(BROKER_DB_CLIENTS_FOLDER, 0700);
    }

    if (stat(BROKER_DB_BROKERS_FOLDER, &st) == -1) {
        mkdir(BROKER_DB_BROKERS_FOLDER, 0700);
    }


    clientId_t clientId;
    brokerId_t brokerId;
    topic_t topic;
    strcpy(topic.name, "falopa123");


    clientId.value = 123456;
    add_client_to_topic_file(clientId, topic);
    add_topic_to_client_file(clientId, topic);

    clientId.value = 17;
    brokerId.value = 14;
    add_client_to_topic_file(clientId, topic);
    add_topic_to_client_file(clientId, topic);
    add_client_to_broker_file(clientId, brokerId);

    clientId.value = 128;
    add_client_to_topic_file(clientId, topic);
    add_topic_to_client_file(clientId, topic);

    clientId.value = 17;
    //remove_client(clientId);
}