#include "processi.h"

#include <limits.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

void ricerca_minimo(shared_t *shared, int indice_figlio) {
    int chunk = VECTOR_SIZE / CHILDREN;
    int begin = indice_figlio * chunk;

    /* L'ultimo figlio copre anche gli eventuali elementi rimanenti. */
    int end = begin + chunk;
    if (indice_figlio == CHILDREN - 1) {
        end = VECTOR_SIZE;
    }

    /* Ogni figlio calcola il minimo della propria porzione senza sincronizzazione. */
    int minimo_locale = INT_MAX;
    for (int i = begin; i < end; i++) {
        if (shared->vettore[i] < minimo_locale) {
            minimo_locale = shared->vettore[i];
        }
    }

    /* Solo l'aggiornamento del minimo globale costituisce una sezione critica. */
    if (sem_wait(&shared->mutex) == -1) {
        exit(1);
    }
    if (minimo_locale < shared->minimo) {
        shared->minimo = minimo_locale;
    }
    if (sem_post(&shared->mutex) == -1) {
        exit(1);
    }

    exit(0);
}
