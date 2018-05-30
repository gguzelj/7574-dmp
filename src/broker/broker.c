#include "broker.h"


brokerConfig config;
int idSequence;

int create_queue(int queueId);

int new_broker_id();

void init_db();

int main() {
    init_broker();
    start_workers();
    while (config.running) {
        safelog("waiting for new connections");
        int fd = accept_new_connection();
        create_new_connection_handlers(fd);
    }
}

void start_workers() {
    char receive_queue[10], response_queue[10];
    COPY(receive_queue, BROKER_RECEIVE_QUEUE);
    COPY(response_queue, BROKER_RESPONSE_QUEUE);
    for (int i = 0; i < BROKER_AMOUNT_WORKERS; ++i) {
        if (fork() == 0) {
            execl("./worker", "./worker", receive_queue, response_queue, NULL);
            exit(EXIT_FAILURE);
        }
    }
}

void create_new_connection_handlers(int client_socket) {
    char socket[10], receive_queue[10], response_queue[10], broker_id[3];
    COPY(socket, client_socket);
    COPY(receive_queue, BROKER_RECEIVE_QUEUE);
    COPY(response_queue, BROKER_RESPONSE_QUEUE);
    COPY(broker_id, new_broker_id());

    int receiverFd = fork();
    if (receiverFd == 0) {
        safelog("launching broker connection receiver...");
        execl("./broker_i", "./broker_i", socket, receive_queue, broker_id, NULL);
        exit(EXIT_FAILURE);
    }

    int responderFd = fork();
    if (responderFd == 0) {
        safelog("launching broker connection responder...");
        execl("./broker_o", "./broker_o", socket, response_queue, broker_id, NULL);
        exit(EXIT_FAILURE);
    }
}


void init_broker() {
    init_logger("broker server");
    fill_config();
    create_queue(BROKER_RECEIVE_QUEUE);
    create_queue(BROKER_RESPONSE_QUEUE);
    create_socket();
    bind_socket();
    listen_socket();
    idSequence = 13; //why not?
    init_db();
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

int create_queue(int queueId) {
    int response = create_msg(queueId);
    if (response < 0) {
        perror("Can not create IPC message queue");
        exit(EXIT_FAILURE);
    }
    return response;
}

int new_broker_id() {
    return idSequence++;
}

void init_db() {
    struct stat st = {0};
    if (stat(BROKER_DB_TOPIC_FOLDER, &st) == -1) {
        mkdir(BROKER_DB_TOPIC_FOLDER, 0700);
    }

    if (stat(BROKER_DB_CLIENTS_FOLDER, &st) == -1) {
        mkdir(BROKER_DB_CLIENTS_FOLDER, 0700);
    }
}