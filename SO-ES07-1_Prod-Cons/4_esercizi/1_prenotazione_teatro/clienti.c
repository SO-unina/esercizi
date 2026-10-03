#include "header.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static teatro_t *teatro;

static void Cliente(void) {
    srand(time(NULL) + getpid());

    int sleeptime = rand() % 6;
    int numposti = (rand() % 4) + 1;
    int allocati = 0;
    int postiscelti[4];

    sleep(sleeptime);

    sem_wait(&teatro->mutex);

    printf("<%d> Disponibilita: %d\n", getpid(), teatro->disponibilita);

    if (teatro->disponibilita < numposti) {
        printf("<%d> Disponibilita esaurita (ho tentato di allocare %d posti, ci sono %d posti liberi)\n", getpid(), numposti, teatro->disponibilita);
        sem_post(&teatro->mutex);
        return;
    }

    /* Metto "in aggiornamento" i primi posti liberi che trovo, cosi'
     * posso rilasciare il mutex durante la sleep senza che altri
     * clienti scelgano gli stessi posti. */
    for (int j = 0; j < POSTI && allocati < numposti; j++) {
        if (teatro->posti[j].stato == LIBERO) {
            teatro->posti[j].stato = INAGGIORNAMENTO;
            postiscelti[allocati] = j;
            allocati++;
            printf("<%d> Ho messo in aggiornamento il posto %d\n", getpid(), j);
        }
    }

    teatro->disponibilita -= numposti;

    sem_post(&teatro->mutex);

    /* Simula il tempo necessario a completare la prenotazione. */
    sleep(1);

    sem_wait(&teatro->mutex);

    for (int j = 0; j < numposti; j++) {
        int p = postiscelti[j];
        teatro->posti[p].stato = OCCUPATO;
        teatro->posti[p].id_cliente = getpid();
        printf("<%d> Ho occupato il posto %d\n", getpid(), p);
    }

    sem_post(&teatro->mutex);
}

int main(void) {
    /* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
    shm_unlink(TEATRO_SHM_NAME);

    int fd = shm_open(TEATRO_SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    /* La dimensione dell'oggetto va impostata con ftruncate PRIMA di mmap. */
    if (ftruncate(fd, sizeof(teatro_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    teatro = mmap(NULL, sizeof(teatro_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (teatro == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    for (int i = 0; i < POSTI; i++) {
        teatro->posti[i].stato = LIBERO;
        teatro->posti[i].id_cliente = 0;
    }
    teatro->disponibilita = POSTI;

    /* Semaforo anonimo condiviso tra processi: pshared = 1 e
     * collocazione in shared memory. */
    if (sem_init(&teatro->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }

    for (int i = 0; i < CLIENTI; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            Cliente();
            exit(0);
        }
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < CLIENTI; i++) {
        wait(NULL);
    }

    printf("Tutti i clienti hanno terminato. Premere Invio per rimuovere le risorse.\n");
    getchar();

    /* Cleanup: il creatore distrugge il semaforo e rimuove il nome
     * della shared memory. */
    sem_destroy(&teatro->mutex);
    munmap(teatro, sizeof(teatro_t));
    shm_unlink(TEATRO_SHM_NAME);

    return 0;
}
