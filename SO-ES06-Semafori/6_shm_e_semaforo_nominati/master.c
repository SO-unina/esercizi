#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include "condiviso.h"

/* CASO 4: eseguibili distinti, dati condivisi E semaforo nominato.
 * La shm nominata contiene solo i dati; il semaforo per la cooperazione
 * (inizializzato a 0: "il dato non e' ancora pronto") e' un oggetto
 * nominato a se'. Il master crea entrambe le risorse e le rimuove. */

int main(void) {
    /* Elimina eventuali risorse rimaste da una precedente esecuzione. */
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_PRONTO);

    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, sizeof(condiviso_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }
    close(fd);

    /* Valore iniziale 0: il lettore si blocca finche' lo scrittore non segnala. */
    sem_t *pronto = sem_open(SEM_PRONTO, O_CREAT | O_EXCL, 0600, 0);
    if (pronto == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        execl("./lettore", "./lettore", (char *)0);

        perror("Exec lettore fallita");
        exit(1);
    }

    /* Il lettore parte per primo e si blocca: l'attesa sul semaforo
     * rende visibile la cooperazione. */
    sleep(1);

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        execl("./scrittore", "./scrittore", (char *)0);

        perror("Exec scrittore fallita");
        exit(1);
    }

    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }

    /* Il creatore rimuove entrambe le risorse nominate. */
    sem_close(pronto);
    sem_unlink(SEM_PRONTO);
    shm_unlink(SHM_NAME);

    printf("Fine: risorse rimosse.\n");
    return 0;
}
