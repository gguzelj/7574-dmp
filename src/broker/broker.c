#include "broker.h"

brokerConfig config;

int main() {
    init_logger("broker server");
    init_broker();

    while (config.running) {
        safelog("waiting for new connections");
        int fd = accept_new_connection();
        create_new_connection_handlers(fd);
    }

}

void create_new_connection_handlers(int client_socket) {
    char socket[10];
    snprintf(socket, sizeof(socket), "%d", client_socket);

    int receiverFd = fork();
    if (receiverFd == 0) {
        safelog("launching broker connection receiver...");
        execl("./broker_i", "./broker_i", socket, NULL);
        exit(EXIT_FAILURE);
    }

    int responderFd = fork();
    if (responderFd == 0) {
        safelog("launching broker connection responder...");
        execl("./broker_o", "./broker_o", socket, NULL);
        exit(EXIT_FAILURE);
    }
}


void init_broker() {
    fill_config();
    create_socket();
    bind_socket();
    listen_socket();
}

void fill_config() {
    config.running = true;
    config.port = BROKER_LISTENING_PORT;
    config.capacity = BROKER_CAPACITY;
    config.address.sin_family = AF_INET;
    config.address.sin_addr.s_addr = INADDR_ANY;
    config.address.sin_port = htons(config.port);
}

void create_socket() {
    if ((config.brokerFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void bind_socket() {
    if (setsockopt(config.brokerFd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0)
        perror("socket options failed");
    if (bind(config.brokerFd, (struct sockaddr *) &config.address, sizeof(config.address)) < 0) {
        perror("socket bind failed");
        exit(EXIT_FAILURE);
    }
}

void listen_socket() {
    if (listen(config.brokerFd, config.capacity) < 0) {
        perror("socket listen failed");
        exit(EXIT_FAILURE);
    }
}

int accept_new_connection() {
    int newConnection;
    int length = sizeof(config.address);
    if ((newConnection = accept(config.brokerFd, (struct sockaddr *) &config.address, (socklen_t *) &length)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    safelog("new connection!");
    return newConnection;
}