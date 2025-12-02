#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "semafori.h"

typedef struct
{
    int valore;
    int stato;
} buffer;

void produzione(int sem_id, buffer *buf1, buffer *buf2, int valore);
int consumazione(int sem_id, buffer *buf1, buffer *buf2);

#define LIBERO 0
#define INUSO 1
#define OCCUPATO 2

#define SPAZIO_DISP 0
#define MESSAGGIO_DISP 1

/* non occorrono MUTEX_P e MUTEX_C, poiché ci sono
 * solo un produttore e solo un consumatore
 */

#endif