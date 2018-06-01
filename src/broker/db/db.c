#include "db.h"

FILE* open_client_file(clientId_t clientId, const char* mode) {
    char filename[100];
    CLIENT_FILE(filename, clientId.value);
    return fopen(filename, mode);
}

FILE* open_topicname_file(const char* topicName, const char* mode) {
    char filename[100];
    TOPIC_FILE(filename, topicName);
    return fopen(filename, mode);
}

FILE* open_broker_file(clientId_t clientId, const char* mode) {
    char filename[100];
    BROKER_FILE(filename, clientId);
    return fopen(filename, mode);
}

FILE* open_topic_file(topic_t topic, const char* mode) {
    return open_topicname_file(topic.name, mode);
}

bool topic_file_contains_client(clientId_t clientId, topic_t topic) {
    FILE *fd = open_topic_file(topic, "r");
    if (fd == NULL) {
        return false;
    }
    char *line = NULL;
    size_t len = 0;
    char id[100];
    snprintf(id, sizeof(id), "%ld\n", clientId.value);
    while (getline(&line, &len, fd) != -1) {
        if (strcmp(id, line) == 0) {
            fclose(fd);
            return true;
        }
    }
    fclose(fd);
    return false;
}

bool client_file_contains_topic(clientId_t clientId, topic_t topic) {
    FILE *fd = open_client_file(clientId, "r");
    if (fd == NULL) {
        return false;
    }
    char *line = NULL;
    size_t len = 0;
    char id[100];
    snprintf(id, sizeof(id), "%s\n", topic.name);
    while (getline(&line, &len, fd) != -1) {
        if (strcmp(id, line) == 0) {
            fclose(fd);
            return true;
        }
    }
    fclose(fd);
    return false;
}


void add_client_to_topic_file(clientId_t clientId, topic_t topic) {
    if (topic_file_contains_client(clientId, topic) == true) {
        safelog("client %ld already subscribed on topic %s", clientId.value, topic.name);
        return;
    }
    safelog("subscribing client %ld on topic %s", clientId.value, topic.name);
    FILE *fd = open_topic_file(topic, "a+");
    fprintf(fd, "%ld\n", clientId.value);
    fclose(fd);
}

void add_client_to_broker_file(clientId_t clientId, brokerId_t brokerId) {
    char brokerFile[100], broker_id[100];
    BROKER_FILE(brokerFile, clientId);
    snprintf(broker_id, sizeof(broker_id), "%ld\n", brokerId.value);
    if (access(brokerFile, F_OK) != -1) {
        remove(brokerFile);
    }
    FILE *fd = fopen(brokerFile, "a+");
    fprintf(fd, "%s", broker_id);
    fclose(fd);
}

void remove_client_from_topic_file(clientId_t clientId, topic_t topic) {
    FILE *fd = open_topic_file(topic, "r");
    FILE *aux = open_topicname_file("aux", "a+");
    if (fd == NULL || aux == NULL) {
        return;
    }
    char *line = NULL;
    size_t len = 0;
    char id[100];
    snprintf(id, sizeof(id), "%ld\n", clientId.value);
    while (getline(&line, &len, fd) != -1) {
        if (strcmp(id, line) != 0) {
            fprintf(aux, "%s", line);
        }
    }
    fclose(fd);
    fclose(aux);

    char old[100], new[100];
    TOPIC_FILE(old, "aux");
    TOPIC_FILE(new, topic.name);
    rename(old, new);
    remove(old);
}

void add_topic_to_client_file(clientId_t clientId, topic_t topic) {
    if (client_file_contains_topic(clientId, topic) == true) {
        safelog("topic %s already added to client %ld", topic.name, clientId.value);
        return;
    }
    safelog("adding topic %s to client %ld", topic.name, clientId.value);
    FILE *fd = open_client_file(clientId, "a+");
    fprintf(fd, "%s\n", topic.name);
    fclose(fd);
}

void remove_client_from_all_topics(clientId_t clientId) {
    FILE *fd = open_client_file(clientId, "r");
    if (fd == NULL) {
        return;
    }
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fd) != -1) {
        topic_t topic = {0};
        strncpy(topic.name, line, strlen(line) - 1);
        remove_client_from_topic_file(clientId, topic);
    }
    fclose(fd);
}

void remove_client(clientId_t clientId) {
    remove_client_from_all_topics(clientId);
    char client[100], brokerFile[100];
    CLIENT_FILE(client, clientId.value);
    BROKER_FILE(brokerFile, clientId);
    remove(client);
    remove(brokerFile);
}


brokerId_t find_broker_id(clientId_t clientId) {
    brokerId_t response;
    char filename[100];
    BROKER_FILE(filename, clientId.value);
    if (access(filename, F_OK) == -1) {
        safelog("No broker id found for client id %ld", clientId);
        response.value = -1;
        return response;
    }
    FILE *fd = fopen(filename, "r");
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fd) != -1) {
        strtok(line, ",");
        response.value = atoi(strtok (NULL, ","));
        return response;
    }
    response.value = -1;
    return response;
}