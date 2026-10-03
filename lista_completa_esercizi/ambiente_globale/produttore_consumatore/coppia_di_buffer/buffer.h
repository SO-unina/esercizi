#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <semaphore.h>

/* Nomi degli oggetti di shared memory POSIX condivisi tra i tre
 * eseguibili: uno per ciascun buffer, piu' uno per i semafori di
 * sincronizzazione. I nomi POSIX iniziano con '/' e sono visibili
 * in /dev/shm. */
#define SHM_BUF1 "/coppia_buf1"
#define SHM_BUF2 "/coppia_buf2"
#define SHM_SINCRO "/coppia_sincro"

typedef struct
{
    int valore;
    int stato;
} buffer;

/* I semafori POSIX sono anonimi e process-shared: campi sem_t
 * collocati in una shared memory, inizializzati dal creatore con
 * sem_init(&s, 1, valore). */
typedef struct
{
    sem_t spazio_disp;
    sem_t messaggio_disp;
} sincro;

void produzione(sincro *sem, buffer *buf1, buffer *buf2, int valore);
int consumazione(sincro *sem, buffer *buf1, buffer *buf2);

#define LIBERO 0
#define INUSO 1
#define OCCUPATO 2

/* non occorrono MUTEX_P e MUTEX_C, poiché ci sono
 * solo un produttore e solo un consumatore
 */

#endif
