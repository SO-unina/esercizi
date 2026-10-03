#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"

/* Eseguibile lanciato da "start" tramite fork + exec. Dopo la exec non
 * esiste piu' alcun mapping ereditato: il semaforo viene ritrovato
 * riaprendolo PER NOME con sem_open. */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nome>\n", argv[0]);
        exit(1);
    }

    /* Senza O_CREAT: il semaforo deve gia' esistere (lo crea start). */
    sem_t *mutex = sem_open(SEM_NAME, 0);
    if (mutex == SEM_FAILED) {
        perror("sem_open (avviare tramite ./start)");
        exit(1);
    }

    for (int i = 0; i < NUM_CICLI; i++) {
        printf("[%s, pid %d] Attendo di entrare nella sezione critica\n", argv[1], getpid());

        sem_wait(mutex);

        printf("[%s, pid %d] Sono entrato nella sezione critica\n", argv[1], getpid());

        /* Simula il lavoro svolto in sezione critica. */
        sleep(1);

        sem_post(mutex);

        printf("[%s, pid %d] Ho lasciato la sezione critica\n", argv[1], getpid());
    }

    sem_close(mutex);
    exit(0);
}
