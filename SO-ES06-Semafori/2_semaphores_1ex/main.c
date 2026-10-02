#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    sem_t mutex;
    int valore;
} shared_t;

static void figlio(shared_t *shared) {
    srand(time(NULL) + getpid());

    for (int i = 0; i < 10; i++) {
        printf("[%d] Attendo di entrare nella sezione critica\n", getpid());

        if (sem_wait(&shared->mutex) == -1) {
            exit(1);
        }

        printf("[%d] Sono entrato nella sezione critica\n", getpid());

        /* La pausa casuale aumenta la probabilita' di osservare una race senza mutex. */
        int temp = shared->valore;
        printf("[%d] Valore corrente: %d, lo incremento\n", getpid(), temp);
        sleep(rand() % 2);
        shared->valore = temp + 1;
        printf("[%d] Nuovo valore: %d\n", getpid(), shared->valore);

        if (sem_post(&shared->mutex) == -1) {
            exit(1);
        }

        printf("[%d] Ho lasciato la sezione critica\n", getpid());
    }

    exit(0);
}

/*
    ESEMPIO DI OUTPUT:
        [12708] Attendo di entrare nella sezione critica
        [12708] Sono entrato nella sezione critica
        [12708] Valore corrente: 0, lo incremento
        [12709] Attendo di entrare nella sezione critica
        [12708] Nuovo valore: 1
        [12708] Ho lasciato la sezione critica
        [12708] Attendo di entrare nella sezione critica
        [12708] Sono entrato nella sezione critica
        [12708] Valore corrente: 1, lo incremento
        [12708] Nuovo valore: 2
        [12708] Ho lasciato la sezione critica
        [12708] Attendo di entrare nella sezione critica
        [12708] Sono entrato nella sezione critica
        [12708] Valore corrente: 2, lo incremento
        [12708] Nuovo valore: 3
        ....

    Perché torna sempre 12708 nella sezione critica anche se 12709 sta aspettando da più tempo?

    Perché il semaforo garantisce la mutua esclusione, non l'equità. POSIX non promette che chi aspetta da più tempo entri per primo: quando arriva la sem_post, chi acquisisce il 
    semaforo tra i contendenti è deciso dallo scheduler, non da una coda FIFO. Il 12709 è bloccato dentro sem_wait (si vede: stampa "Attendo" una volta sola e poi sparisce).
    Il 12708 invece esegue sem_post e, due istruzioni di loop dopo, chiama di nuovo sem_wait. A quel punto la gara è impari: il 12708 sta già girando sulla CPU, mentre il 12709 
    è stato appena svegliato e deve ancora essere rimesso in esecuzione dallo scheduler. Il 12708 trova il semaforo libero, lo decrementa e rientra prima ancora che il 12709 abbia
    avuto modo di provarci. Questo fenomeno ha anche un nome: barging — chi "irrompe" correndo batte chi era educatamente in coda. 

*/


// Che succede invece SENZA l'utilizzo del semaforo?

int main(void) {
    char name[64];
    sprintf(name, "/so_es06_ex1_%d", getpid());

    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
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

    if (sem_init(&shared->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    shared->valore = 0;

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            figlio(shared);
        }
    }

    for (int i = 0; i < 2; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            exit(1);
        }
    }

    printf("Valore finale: %d (atteso: 20)\n", shared->valore);
    int result = shared->valore == 20 ? 0 : 1;

    sem_destroy(&shared->mutex);
    munmap(shared, sizeof(shared_t));
    close(fd);
    shm_unlink(name);
    return result;
}
