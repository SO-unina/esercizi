#include "messages.h"

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MESSAGES 6

int main(void) {
    mqd_t queue = mq_open(QUEUE_NAME, O_WRONLY);
    if (queue == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    /* I valori 5 identificano i messaggi urgenti, consegnati prima dei valori 1. */
    const unsigned int priorities[NUM_MESSAGES] = {
        1,
        1,
        5,
        1,
        5,
        1
    };

    for (int i = 0; i < NUM_MESSAGES; i++) {
        message_t message;
        message.sequence = i;
        char *classe = "ordinaria";
        if (priorities[i] > 1) {
            classe = "urgente";
        }
        sprintf(message.text, "messaggio %d, classe %s", i, classe);

        if (mq_send(queue, (const char *)&message, sizeof(message), priorities[i]) == -1) {
            perror("mq_send");
            return 1;
        }
    }

    mq_close(queue);
    return 0;
}
