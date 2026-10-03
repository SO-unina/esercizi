#include <unistd.h>
#include <stdio.h>

#include "buffer.h"

void leggi_buffer(buffer *b, int *val_1, int *val_2) {

    printf("[%d] Inizio lettura\n", getpid());

    sem_wait(&b->mutexl);

    b->num_lettori++;

    if(b->num_lettori == 1) {
        sem_wait(&b->synch);
    }

    sem_post(&b->mutexl);

    printf("[%d] Lettura buffer: val1=%d, val2=%d\n", getpid(), b->val_1, b->val_2);

    sleep(1);

    *val_1 = b->val_1;
    *val_2 = b->val_2;

    printf("[%d] Fine lettura\n", getpid());

    sem_wait(&b->mutexl);

    b->num_lettori--;

    if(b->num_lettori == 0) {
        sem_post(&b->synch);
    }

    sem_post(&b->mutexl);
}

void scrivi_buffer(buffer *b, int val_1, int val_2) {

    printf("[%d] Inizio scrittura\n", getpid());

    sem_wait(&b->synch);

    printf("[%d] Scrittura buffer: val1=%d, val2=%d\n", getpid(), val_1, val_2);

    sleep(2);

    b->val_1 = val_1;
    b->val_2 = val_2;

    printf("[%d] Fine scrittura\n", getpid());

    sem_post(&b->synch);
}
