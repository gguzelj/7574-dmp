#include <stdio.h>
#include "../common/common.h"
#include "../lib/msg.h"
#include "../clientd/client.h"

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
clientId_t clientId = {0};

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

    if (strcmp("destroy", pch) == 0) {
        execute_destroy(str);
        return;
    }

    if (strcmp("quit", pch) == 0 || strcmp("q", pch) == 0) {
        printf("Cya!");
        return;
    }
    printf("Unrecognized option!\n");
}

void execute_create(char *str) {
    if (strtok(NULL, " ") != NULL) {
        printf("incorrect use of create option!\n");
        return;
    }
    create(&clientId);
    printf("new instance created with id: %ld\n", clientId.value);
}


void execute_publish(char *str) {
    char *message_str = strtok(NULL, "\"");
    strtok(NULL, "\"");
    char *topic_str = strtok(NULL, "\"");
    strtok(NULL, "\"");
    char *rest = strtok(NULL, " ");

    if (topic_str == NULL || message_str == NULL || rest != NULL) {
        printf("incorrect use of publish option!\n");
        return;
    }

    if (clientId.value == 0) {
        printf("no instance created. Execute \"create\"!\n");
        return;
    }

    topic_t topic = {0};
    message_t message = {0};

    strcpy(topic.name, topic_str);
    strcpy(message.value, message_str);

    if (publish(clientId, message, topic) == OK) {
        printf("OK\n");
    } else {
        printf("ERROR\n");
    }
}


void execute_subscribe(char *str) {
    char *topic_str = strtok(NULL, "\"");
    strtok(NULL, "\"");
    char *rest = strtok(NULL, " ");

    if (topic_str == NULL || rest != NULL) {
        printf("incorrect use of subscribe option!\n");
        return;
    }

    if (clientId.value == 0) {
        printf("no instance created. Execute \"create\"!\n");
        return;
    }

    topic_t topic = {0};
    strcpy(topic.name, topic_str);

    if (subscribe(clientId, topic) != OK) {
        printf("Unexpected error while publishing!\n");
    } else {
        printf("OK\n");
    }
}


void execute_receive(char *str) {
    if (strtok(NULL, " ") != NULL) {
        printf("incorrect use of receive option!\n");
        return;
    }

    if (clientId.value == 0) {
        printf("no instance created. Execute \"create\"!\n");
        return;
    }

    message_t message;
    if (receive(clientId, &message) == OK) {
        printf("new message: %s\n", message.value);
    } else {
        printf("ERROR\n");
    }
}


void execute_destroy(char *str) {
    if (strtok(NULL, " ") != NULL) {
        printf("incorrect use of receive option!\n");
        return;
    }

    if (clientId.value == 0) {
        printf("no instance created. Execute \"create\"!\n");
        return;
    }

    if (destroy(clientId) == OK) {
        clientId.value = 0;
        printf("OK\n");
    } else {
        printf("ERROR\n");
    }
}


void show_usage() {
    printf("%s.___  ___.   ______   .___  ___.      ______  __       __   _______ .__   __. .___________.\n", KCYN);
    printf("%s|   \\/   |  /  __  \\  |   \\/   |     /      ||  |     |  | |   ____||  \\ |  | |           |\n", KCYN);
    printf("%s|  \\  /  | |  |  |  | |  \\  /  |    |  ,----'|  |     |  | |  |__   |   \\|  | `---|  |----`\n", KCYN);
    printf("%s|  |\\/|  | |  |  |  | |  |\\/|  |    |  |     |  |     |  | |   __|  |  . `  |     |  |\n", KCYN);
    printf("%s|  |  |  | |  `--'  | |  |  |  |    |  `----.|  `----.|  | |  |____ |  |\\   |     |  |\n", KCYN);
    printf("%s|__|  |__|  \\______/  |__|  |__|     \\______||_______||__| |_______||__| \\__|     |__|\n", KCYN);


    printf("%sDistributed messaging client\n", KGRN);
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