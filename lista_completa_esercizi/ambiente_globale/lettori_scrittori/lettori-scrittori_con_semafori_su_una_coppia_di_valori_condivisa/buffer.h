#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <semaphore.h>

/* Nome dell'oggetto di shared memory POSIX condiviso tra i tre
 * eseguibili. I nomi POSIX iniziano con '/' e sono visibili
 * in /dev/shm. */
#define SHM_NAME "/lett_scritt_coppia"

/* I semafori POSIX sono anonimi e process-shared: sono campi sem_t
 * collocati direttamente nella struttura in shared memory, e vengono
 * inizializzati dal creatore con sem_init(&s, 1, valore). */
typedef struct {
    sem_t mutexl;   /* mutua esclusione sul contatore num_lettori */
    sem_t synch;    /* sincronizzazione lettori/scrittore */
    int val_1;
    int val_2;
    int num_lettori;
} buffer;

void leggi_buffer(buffer *b, int *val_1, int *val_2);
void scrivi_buffer(buffer *b, int val_1, int val_2);

#endif
