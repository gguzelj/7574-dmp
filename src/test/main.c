#include <stdio.h>
#include <zconf.h>
#include "../common/common.h"
#include "../lib/msg.h"
#include "../client/client.h"

int main() {

    printf("sending request to create client: %d\n", getpid());
    fflush(stdout);
    clientId_t* id = create();
    printf("sending request to create client recive id %d\n", id->value);
    fflush(stdout);




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