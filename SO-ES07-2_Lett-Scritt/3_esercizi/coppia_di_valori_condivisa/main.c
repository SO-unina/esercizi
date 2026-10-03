#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

/* Nome dell'oggetto di shared memory condiviso */
#define SHM_NAME "/so_es07_coppia_valori"

#define NUM_SCRITTORI 2
#define NUM_LETTORI 4

typedef struct {
    sem_t tornello;
    sem_t mutex_lettori;
    sem_t stanza_vuota;
    int lettori;
    int a;
    int b;
} shared_t;

void inizio_lettura(shared_t *shared) {
    /* Il tornello impedisce a nuovi lettori di superare uno scrittore in attesa. */
    sem_wait(&shared->tornello);
    sem_post(&shared->tornello);

    sem_wait(&shared->mutex_lettori);

    shared->lettori = shared->lettori + 1;
    if (shared->lettori == 1) /* il primo lettore blocca gli scrittori */
        sem_wait(&shared->stanza_vuota);

    sem_post(&shared->mutex_lettori);
}

void fine_lettura(shared_t *shared) {
    sem_wait(&shared->mutex_lettori);

    shared->lettori = shared->lettori - 1;
    if (shared->lettori == 0) /* l'ultimo lettore rilascia la risorsa */
        sem_post(&shared->stanza_vuota);

    sem_post(&shared->mutex_lettori);
}

void inizio_scrittura(shared_t *shared) {
    sem_wait(&shared->tornello);
    sem_wait(&shared->stanza_vuota);
}

void fine_scrittura(shared_t *shared) {
    sem_post(&shared->stanza_vuota);
    sem_post(&shared->tornello);
}

int main(void) {
    /* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
    shm_unlink(SHM_NAME);

    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, sizeof(shared_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    shared_t *shared = mmap(NULL, sizeof(shared_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    sem_init(&shared->tornello, 1, 1);
    sem_init(&shared->mutex_lettori, 1, 1);
    sem_init(&shared->stanza_vuota, 1, 1);

    shared->lettori = 0;
    shared->a = 0;
    shared->b = 0;

    for (int i = 0; i < NUM_SCRITTORI; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork scrittore");
            exit(1);
        }
        if (pid == 0) {
            for (int k = 1; k <= 20; k++) {
                inizio_scrittura(shared);

                /* Le due assegnazioni devono essere osservate come un'unica operazione. */
                shared->a = k;
                shared->b = 2 * k;

                fine_scrittura(shared);
            }
            exit(0);
        }
    }

    for (int i = 0; i < NUM_LETTORI; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork lettore");
            exit(1);
        }
        if (pid == 0) {
            for (int k = 0; k < 50; k++) {
                inizio_lettura(shared);

                if (shared->b != 2 * shared->a) {
                    printf("Invariante violato\n");
                    exit(1);
                }

                fine_lettura(shared);
            }
            exit(0);
        }
    }

    int ok = 1;
    for (int i = 0; i < NUM_SCRITTORI + NUM_LETTORI; i++) {
        int status;
        if (wait(&status) == -1) {
            perror("wait");
            ok = 0;
            break;
        }
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            ok = 0;
        }
    }

    if (ok) {
        printf("Invariante rispettato: si\n");
    } else {
        printf("Invariante rispettato: no\n");
    }

    sem_destroy(&shared->tornello);
    sem_destroy(&shared->mutex_lettori);
    sem_destroy(&shared->stanza_vuota);
    munmap(shared, sizeof(shared_t));
    shm_unlink(SHM_NAME);

    if (ok) {
        return 0;
    }
    return 1;
}
