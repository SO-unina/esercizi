#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#include "processi.h"

int main()
{

    int shm_fd;
    condiviso_t *condiviso;

    /* Elimina un eventuale oggetto rimasto da una precedente
     * esecuzione, cosi' la shm_open con O_EXCL non fallisce. */
    shm_unlink(SHM_NAME);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0644);

    if (shm_fd < 0)
    {
        perror("Impossibile creare la shared memory condivisa");
        exit(1);
    }

    /* La dimensione dell'oggetto (inizialmente 0) va impostata con
     * ftruncate PRIMA di mmap. */
    if (ftruncate(shm_fd, sizeof(condiviso_t)) < 0)
    {
        perror("Impossibile dimensionare la shared memory condivisa");
        exit(1);
    }

    condiviso = mmap(NULL, sizeof(condiviso_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (condiviso == MAP_FAILED)
    {
        perror("Impossibile mappare la shared memory condivisa");
        exit(1);
    }

    close(shm_fd);

    /* Inizializza il vettore con numeri casuali tra 0 e INT_MAX */

    srand(time(NULL));

    for (int i = 0; i < NUM_ELEMENTI; i++)
    {

        condiviso->vettore[i] = rand() % INT_MAX;

        //printf("%d\n", condiviso->vettore[i]); // per debugging
    }

    /* Inizializza il buffer ad INT_MAX.
     * Il valore da ricercare sarà, per definizione, minore del valore iniziale.
     */

    condiviso->buffer = INT_MAX;


    /* Inizializzazione semafori (anonimi, process-shared, collocati
     * nella struttura in shared memory; la mappatura MAP_SHARED viene
     * ereditata dai figli attraverso la fork) */

    if (inizializza_semafori(condiviso) < 0)
    {
        perror("Impossibile inizializzare i semafori");
        exit(1);
    }


    /* Avvio dei processi figli */

    pid_t pid;

    for (int i = 0; i < 10; i++)
    {

        pid = fork();

        if (pid == 0)
        {
            /* Processo figlio */

            /* NUM_ELEMENTI/NUM_PROCESSI = 1000
             * i=0: 0...999
             * i=1: 1000...1999
             * i=2: 2000...2999
             * ...
             * i=9: 9000...9999
             */
            figlio(condiviso, i * (NUM_ELEMENTI / NUM_PROCESSI), NUM_ELEMENTI / NUM_PROCESSI);

            exit(0);
        }

        else if (pid < 0)
        {
            perror("Impossibile avviare processo figlio");
            exit(1);
        }
    }


    /* Processo padre */

    padre(condiviso);


    /* Deallocazione risorse IPC: prima si distruggono i semafori,
     * poi si rimuovono la mappatura e il nome della shared memory. */

    distruggi_semafori(condiviso);
    munmap(condiviso, sizeof(condiviso_t));
    shm_unlink(SHM_NAME);
}
