#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "buffer.h"

int main() {

    printf("[%d] Creazione shared memory\n", getpid());

    /* Elimina un eventuale oggetto rimasto da una precedente
     * esecuzione, cosi' la shm_open con O_EXCL non fallisce. */
    shm_unlink(SHM_NAME);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0644);

    if(shm_fd < 0) {
        perror("Errore creazione shared memory");
        exit(1);
    }

    /* La dimensione dell'oggetto (inizialmente 0) va impostata con
     * ftruncate PRIMA di mmap. */
    if(ftruncate(shm_fd, sizeof(buffer)) < 0) {
        perror("Errore ftruncate shared memory");
        exit(1);
    }

    buffer * b = mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if(b == MAP_FAILED) {
        perror("Errore mmap shared memory");
        exit(1);
    }

    close(shm_fd);


    b->val_1 = 0;
    b->val_2 = 0;
    b->num_lettori = 0;


    printf("[%d] Creazione semafori\n", getpid());

    /* Semafori anonimi condivisi tra processi: pshared = 1 e
     * collocazione all'interno della shared memory. */

    if(sem_init(&b->mutexl, 1, 1) < 0) {
        perror("Errore inizializione semafori");
        exit(1);
    }

    if(sem_init(&b->synch, 1, 1) < 0) {
        perror("Errore inizializione semafori");
        exit(1);
    }


    printf("[%d] Creazione processo scrittore\n", getpid());

    pid_t pid = fork();

    if(pid == 0) {

        execl("./main_scrittore", "main_scrittore", NULL);

        perror("Errore exec scrittore");
        exit(1);
    }


    for(int i=0; i<2; i++) {

        printf("[%d] Creazione processo lettore\n", getpid());


        pid_t pid = fork();

        if(pid == 0) {

            execl("./main_lettori", "main_lettori", NULL);

            perror("Errore exec lettori");
            exit(1);
        }
    }


    printf("[%d] In attesa di terminazione dei processi\n", getpid());

    for(int i=0; i<3; i++) {
        wait(NULL);
    }


    printf("[%d] Deallocazione risorse\n", getpid());

    /* Il creatore distrugge i semafori, rimuove la mappatura e infine
     * rimuove il nome dell'oggetto da /dev/shm (shm_unlink). */
    sem_destroy(&b->mutexl);
    sem_destroy(&b->synch);
    munmap(b, sizeof(buffer));
    shm_unlink(SHM_NAME);
}
