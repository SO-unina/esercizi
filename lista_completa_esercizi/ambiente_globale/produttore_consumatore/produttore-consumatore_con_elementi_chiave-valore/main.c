#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "prodcons.h"

int main()
{

    int shm_fd;
    prodcons_t *pc;

    /* Elimina un eventuale oggetto rimasto da una precedente
     * esecuzione, cosi' la shm_open con O_EXCL non fallisce. */
    shm_unlink(SHM_NAME);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0644);

    if (shm_fd < 0)
    {
        perror("Errore creazione SHM");
        exit(1);
    }

    /* La dimensione dell'oggetto (inizialmente 0) va impostata con
     * ftruncate PRIMA di mmap. */
    if (ftruncate(shm_fd, sizeof(prodcons_t)) < 0)
    {
        perror("Errore ftruncate SHM");
        exit(1);
    }

    pc = mmap(NULL, sizeof(prodcons_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (pc == MAP_FAILED)
    {
        perror("Errore mmap SHM");
        exit(1);
    }

    close(shm_fd);

    for(int i=0; i<DIM; i++) {
        pc->stato[i] = LIBERO;
    }

    /* Semafori anonimi condivisi tra processi: pshared = 1 e
     * collocazione nella struttura in shared memory (la mappatura
     * MAP_SHARED viene ereditata dai figli attraverso la fork). */

    sem_init(&pc->spazio_disp, 1, DIM);
    sem_init(&pc->messaggio_disp_1, 1, 0);
    sem_init(&pc->messaggio_disp_2, 1, 0);
    sem_init(&pc->messaggio_disp_3, 1, 0);
    sem_init(&pc->mutex_p, 1, 1);

    for (int i = 0; i < 3; i++)
    {

        pid_t pid_produttore = fork();

        if (pid_produttore == 0)
        {
            /* figlio */

            printf("[%d] Avvio produttore\n", getpid());

            srand(getpid());

            int chiave = i + 1;   /* chiavi 1, 2, 3 */

            for(int j=0; j<3; j++) {

                int valore = rand() % 10;
                produzione(pc, chiave, valore);
            }

            exit(0);
        }
        else if (pid_produttore < 0)
        {
            perror("Errore fork produttore");
            exit(1);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        pid_t pid_consumatore = fork();

        if (pid_consumatore == 0)
        {
            /* figlio */

            printf("[%d] Avvio consumatore\n", getpid());

            srand(getpid());

            int chiave = i + 1;   /* chiavi 1, 2, 3 */

            for(int j=0; j<3; j++) {

                consumazione(pc, chiave);
            }

            exit(0);
        }
        else if (pid_consumatore < 0)
        {
            perror("Errore fork consumatore");
            exit(1);
        }
    }

    for (int i = 0; i < 6; i++)
    {
        wait(NULL);
    }

    /* Cleanup: il creatore distrugge i semafori, rimuove la mappatura
     * e infine rimuove il nome dell'oggetto da /dev/shm (shm_unlink). */

    sem_destroy(&pc->spazio_disp);
    sem_destroy(&pc->messaggio_disp_1);
    sem_destroy(&pc->messaggio_disp_2);
    sem_destroy(&pc->messaggio_disp_3);
    sem_destroy(&pc->mutex_p);

    munmap(pc, sizeof(prodcons_t));
    shm_unlink(SHM_NAME);
}
