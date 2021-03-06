#include "next_broker_connector.h"

void dispatch_to_next_broker(request_t request);

bool is_connection_established();

void find_next_broker(char *nextBroker);

void fill_with_configfile(char config[MAX_BROKER_CLUSTER_NODES][100]);

int main(int argc, char **argv) {
    init_connector(argc, argv);
    safelog("broker connector with id %d ready", config.brokerId);
    do {
        safelog("waiting for next request");
        request_t request = receive_request();
        if (request.type != PUBLISH) {
            safelog("This process only publishes messages to next broker!!");
            continue;
        }
        dispatch_to_next_broker(request);
        safelog("another round...");
    } while (config.running);
    safelog("EXIT");
}

void init_connector(int argc, char **argv) {
    init_logger("broker connector");
    config.running = true;
    config.chainQueue = get_msg(atoi(argv[1]));
    config.brokerId = atoi(argv[2]);
    config.chainBrokerFd = 0;
}

request_t receive_request() {
    request_t request;
    receive_msg(config.chainQueue, &request, sizeof(request_t), 0);
    return request;
}

void dispatch_to_next_broker(request_t request) {
    safelog("publish: topic %s. My id: %ld. Request broker id: %d", request.body.publish.topic.name, config.brokerId, request.broker_id);
    if (is_connection_established() == true && config.brokerId != request.broker_id) {
        if (request.broker_id == -1) {
            request.broker_id = config.brokerId;
        }
        safelog("dispatching message to next broker");
        send_request(config.chainBrokerFd, &request);
        safelog("request sent");
    }
}

/**
 * Is broker connected with the next broker in the chain?
 * if not, try to establish connection
 */
bool is_connection_established() {
    if (config.chainBrokerFd > 0) {
        safelog("connection already established");
        return true;
    }
    char next_broker[100];
    find_next_broker(next_broker);
    safelog("no connection established with next broker %s.", next_broker);
    config.chainBrokerFd = create_broker_socket(next_broker, BROKER_LISTENING_PORT);
    if (config.chainBrokerFd < 0) {
        safelog("impossible to connect %s", next_broker);
        return false;
    }
    safelog("successful connection with broker. fd: %d", config.chainBrokerFd);
    return true;
}

void find_next_broker(char *nextBroker) {
    char configFile[MAX_BROKER_CLUSTER_NODES][100];
    fill_with_configfile(configFile);
    if ((config.brokerId + 1) == MAX_BROKER_CLUSTER_NODES || strcmp(configFile[config.brokerId + 1], "\0") == 0) {
        strcpy(nextBroker, configFile[0]);
        return;
    }
    strcpy(nextBroker, configFile[config.brokerId + 1]);
}

void fill_with_configfile(char config[MAX_BROKER_CLUSTER_NODES][100]) {
    FILE *fd = fopen(BROKER_CONFIG_FILE, "r");
    if (fd == NULL) {
        safelog("error while opening config file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_BROKER_CLUSTER_NODES; ++i) {
        bzero(config[i], 100);
    }
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fd) != -1) {
        char *id = strtok(line, ":");
        char *rest = strtok(NULL, "");
        strncpy(config[atoi(id)], rest, strlen(rest) - 1);
    }
    fclose(fd);
}