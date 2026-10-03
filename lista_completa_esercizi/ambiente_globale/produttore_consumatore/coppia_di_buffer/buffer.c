#include "buffer.h"

#include <unistd.h>
#include <stdio.h>

void produzione(sincro *sem, buffer *buf1, buffer *buf2, int valore)
{

    buffer * buf;

    sem_wait(&sem->spazio_disp);

    /* RICERCA DEL BUFFER LIBERO */

    /* NOTA: Nel caso ci fossero stati più di un produttore,
     * sarebbe necessario qui usare la mutua esclusione
     * (MUTEX_P) per cerca il buffer LIBERO
     */

    if (buf1->stato == LIBERO)
    {
        printf("[%d] In produzione su buf1...\n", getpid());

        buf = buf1;

    } else { /* buf2->stato == LIBERO */

        printf("[%d] In produzione su buf2...\n", getpid());

        buf = buf2;
    }

    buf->stato = INUSO;

    /* FINE RICERCA DEL BUFFER LIBERO */

    sleep(1);

    buf->valore = valore;

    buf->stato = OCCUPATO;

    printf("[%d] Prodotto il valore %d\n", getpid(), valore);

    sem_post(&sem->messaggio_disp);
}

int consumazione(sincro *sem, buffer *buf1, buffer *buf2)
{

    buffer * buf;

    int valore;

    sem_wait(&sem->messaggio_disp);

    /* RICERCA DEL BUFFER OCCUPATO */

    /* NOTA: Nel caso ci fossero stati più di un consumatore,
     * sarebbe necessario qui usare la mutua esclusione
     * (MUTEX_C) per cerca il buffer OCCUPATO
     */

    if (buf1->stato == OCCUPATO)
    {
        printf("[%d] In consumazione su buf1...\n", getpid());

        buf = buf1;

    } else { /* buf2->stato == OCCUPATO */

        printf("[%d] In consumazione su buf2...\n", getpid());

        buf = buf2;
    }

    buf->stato = INUSO;

    /* FINE RICERCA DEL BUFFER OCCUPATO */

    sleep(2);

    valore = buf->valore;

    buf->stato = LIBERO;

    printf("[%d] Consumato il valore %d\n", getpid(), valore);

    sem_post(&sem->spazio_disp);

    return valore;
}
