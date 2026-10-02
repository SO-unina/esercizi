#ifndef PROCESSI_H
#define PROCESSI_H

#include <semaphore.h>

#define VECTOR_SIZE 10000
#define CHILDREN 10

typedef struct {
    sem_t mutex;
    int minimo;
    int vettore[VECTOR_SIZE];
} shared_t;

void ricerca_minimo(shared_t *shared, int indice_figlio);

#endif
