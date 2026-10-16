#ifndef MESSAGES_H
#define MESSAGES_H

#define QUEUE_NAME "/so_es08_priority"
#define MAX_TEXT 96

typedef struct {
    int sequence;
    char text[MAX_TEXT];
} message_t;

#endif
