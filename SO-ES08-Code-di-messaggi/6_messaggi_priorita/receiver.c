#include "messages.h"

#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define NUM_MESSAGES 6

int main(void) {
    struct mq_attr attributes;
    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10;
    attributes.mq_msgsize = sizeof(message_t);
    attributes.mq_curmsgs = 0;

    mq_unlink(QUEUE_NAME);
    mqd_t queue = mq_open(QUEUE_NAME, O_CREAT | O_EXCL | O_RDONLY, 0600, &attributes);
    if (queue == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    printf("Coda pronta: avviare sender.\n");

    for (int i = 0; i < NUM_MESSAGES; i++) {
        message_t message;
        unsigned int priority;

        /* POSIX consegna prima i messaggi con priorita' numerica maggiore. */
        if (mq_receive(queue, (char *)&message, sizeof(message), &priority) == -1) {
            perror("mq_receive");
            return 1;
        }

        printf("priorita=%u, sequenza=%d, testo=%s\n", priority, message.sequence, message.text);
    }

    mq_close(queue);
    mq_unlink(QUEUE_NAME);
    return 0;
}
