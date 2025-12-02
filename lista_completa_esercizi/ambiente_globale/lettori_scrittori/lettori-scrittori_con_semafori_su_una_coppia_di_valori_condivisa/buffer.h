#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "semafori.h"

typedef struct {
    int val_1;
    int val_2;
    int num_lettori;
} buffer;

#define MUTEXL 0
#define SYNCH 1

void leggi_buffer(buffer * b, int sem_id, int *val_1, int *val_2);
void scrivi_buffer(buffer *b, int sem_id, int val_1, int val_2);

#endif