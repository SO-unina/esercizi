#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "buffer.h"

int main() {

    printf("[%d] Creazione shared memory\n", getpid());

    key_t chiave_shm = ftok(".",'m');

    int shm_id = shmget(chiave_shm, sizeof(buffer), IPC_CREAT | 0644);

    if(shm_id < 0) {
        perror("Errore creazione shared memory");
        exit(1);
    }

    buffer * b = shmat(shm_id, NULL, 0);

    if(b == (void*)-1) {
        perror("Errore attach shared memory");
        exit(1);
    }


    b->val_1 = 0;
    b->val_2 = 0;
    b->num_lettori = 0;


    printf("[%d] Creazione semafori\n", getpid());

    key_t sem_chiave = ftok(".", 's');

    int sem_id = semget(sem_chiave, 2, IPC_CREAT | 0644);

    if(sem_id < 0) {
        perror("Errore creazione semafori");
        exit(1);
    }


    int ret = semctl(sem_id, MUTEXL, SETVAL, 1);

    if(ret < 0) {
        perror("Errore inizializione semafori");
        exit(1);
    }


    ret = semctl(sem_id, SYNCH, SETVAL, 1);

    if(ret < 0) {
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

    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
}
