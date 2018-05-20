#include <stdio.h>
#include "../common/common.h"
#include "../lib/msg.h"
#include "../clientd/client.h"
#include "../clientd/id_mapper/id_mapper.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BUFFER_LIMIT 4096

bool running;
char *prefix = "client> ";
clientId_t clientId;

void init_client();

void show_usage();

void execute_command(char *str);

void execute_create(char *str);

void execute_publish(char *str);

void execute_subscribe(char *str);

void execute_receive(char *str);

void execute_destroy(char *str);

int main() {
    init_client();
    show_usage();
    do {
        const int bufsize = BUFFER_LIMIT;
        char cmd[bufsize];
        printf("%s%s%s", KWHT, prefix, KNRM);
        fgets(cmd, bufsize, stdin);
        /* get rid of the '\n' from fgets */
        if (cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) - 1] = '\0';

        execute_command(cmd);
    } while (running);

/*
    while(config.running) {

        send

        topic_t topic;
        strcpy(topic.name, "Falopa");

        request_t request1 = {0};
        request1.type = PUBLISH;
        request1.mtype = getpid();
        request1.body.publish.topic = topic;

        printf("Sending body...");
        fflush(stdout);
        send_msg(f, &request1, sizeof(struct request));


        sleep(2);

    }*/

}

void execute_command(char *str) {
    char *pch = strtok(str, " ");
    if (pch == NULL)
        return;

    if (strcmp("create", pch) == 0) {
        execute_create(str);
        return;
    }

    if (strcmp("publish", pch) == 0) {
        execute_publish(str);
        return;
    }

    if (strcmp("subscribe", pch) == 0) {
        execute_subscribe(str);
        return;
    }

    if (strcmp("receive", pch) == 0) {
        execute_receive(str);
        return;
    }

    if (strcmp("destroy", pch) == 0 || strcmp("quit", pch) == 0 || strcmp("q", pch) == 0) {
        execute_destroy(str);
        return;
    }

    printf("Unrecognized option!\n");
}

void execute_create(char *str) {
    if (strtok(NULL, " ") != NULL) {
        printf("wrong usage of create option!\n");
        return;
    }
    create(&clientId);
    printf("Id from broker: %ld\n", clientId.value);
}


void execute_publish(char *str) {
    char *message_str = strtok(NULL, "\"");
    strtok(NULL, "\"");
    char *topic_str = strtok(NULL, "\"");
    strtok(NULL, "\"");
    char *rest = strtok(NULL, " ");

    if (topic_str == NULL || message_str == NULL || rest != NULL) {
        printf("wrong usage of publish option!\n");
        return;
    }

    topic_t topic = {0};
    message_t message = {0};

    strcpy(topic.name, topic_str);
    strcpy(message.value, message_str);

    if (publish(clientId, message, topic) != OK) {
        printf("Unexpected error while publishing!");
    }
}


void execute_subscribe(char *str) {
    char *topic = strtok(NULL, " ");
    char *rest = strtok(NULL, " ");

    if (topic == NULL || rest != NULL) {
        printf("wrong usage of subscribe option!\n");
        return;
    }

    printf("Subscribing on topic %s\n", topic);
}


void execute_receive(char *str) {
    if (strtok(NULL, " ") != NULL) {
        printf("wrong usage of receive option!\n");
        return;
    }

    printf("receiving...\n");
}


void execute_destroy(char *str) {
    if (strtok(NULL, " ") != NULL) {
        printf("wrong usage of receive option!\n");
        return;
    }

    printf("destroying...\n");
}


void show_usage() {
    printf("%s.___  ___.   ______   .___  ___.      ______  __       __   _______ .__   __. .___________.\n", KCYN);
    printf("%s|   \\/   |  /  __  \\  |   \\/   |     /      ||  |     |  | |   ____||  \\ |  | |           |\n", KCYN);
    printf("%s|  \\  /  | |  |  |  | |  \\  /  |    |  ,----'|  |     |  | |  |__   |   \\|  | `---|  |----`\n", KCYN);
    printf("%s|  |\\/|  | |  |  |  | |  |\\/|  |    |  |     |  |     |  | |   __|  |  . `  |     |  |\n", KCYN);
    printf("%s|  |  |  | |  `--'  | |  |  |  |    |  `----.|  `----.|  | |  |____ |  |\\   |     |  |\n", KCYN);
    printf("%s|__|  |__|  \\______/  |__|  |__|     \\______||_______||__| |_______||__| \\__|     |__|\n", KCYN);


    printf("%sDistributed messaging client\n",KGRN);
    printf("%sUsage: \n\n", KRED);
    printf("%screate\n", KWHT);
    printf("%screates a new instance\n\n", KRED);
    printf("%spublish %s\"text to publish\" %s\"topic\"\n", KWHT, KGRN, KCYN);
    printf("%spublishes a new message\n\n", KRED);
    printf("%ssubscribe %s\"topic\"\n", KWHT, KCYN);
    printf("%ssubscribe to a topic\n\n", KRED);
    printf("%sreceive\n", KWHT);
    printf("%sreceive a new message\n", KRED);
    printf("%s\n", KNRM);
}

void init_client() {
    running = true;
}