#ifndef _SENSORE_H_
#define _SENSORE_H_

#include <mqueue.h>

/* Nomi delle code POSIX: sostituiscono gli identificatori delle
 * code System V create dal processo padre. */
#define CODA_SENSORE "/aggregatore_sensore"
#define CODA_COLLETTORE_FMT "/aggregatore_collettore_%d"
#define NOME_CODA_MAX 64

#define MAX_MESSAGGI 10

/* Con le code POSIX non esiste il campo "mtype": il messaggio
 * contiene solo il valore letto dal sensore. */
typedef struct {

    int valore;

} messaggio;

void sensore(mqd_t coda_sensore);

#endif
