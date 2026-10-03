#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "header.h"

/* Il programma crea un semaforo NOMINATO e lancia con fork + exec due
 * eseguibili distinti ("lavoratore"), che si contendono una sezione
 * critica.
 *
 * Il semaforo deve essere nominato: la exec sostituisce lo spazio di
 * indirizzamento del figlio e distrugge ogni mapping ereditato, quindi
 * un semaforo anonimo in shared memory non sarebbe piu' raggiungibile.
 * Il nuovo programma puo' pero' riaprire per nome le risorse nominate. */

int main(void) {
    /* Elimina un eventuale semaforo rimasto da una precedente esecuzione. */
    sem_unlink(SEM_NAME);

    /* Valore iniziale 1: il semaforo realizza la mutua esclusione. */
    sem_t *mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0600, 1);
    if (mutex == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        execl("./lavoratore", "./lavoratore", "A", (char *)0);

        perror("Exec fallita");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        execl("./lavoratore", "./lavoratore", "B", (char *)0);

        perror("Exec fallita");
        exit(1);
    }

    for (int i = 0; i < 2; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            exit(1);
        }
    }

    /* Il creatore chiude il proprio riferimento e rimuove il nome. */
    sem_close(mutex);
    sem_unlink(SEM_NAME);

    printf("I due lavoratori hanno terminato.\n");
    return 0;
}
