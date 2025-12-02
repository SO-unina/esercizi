#include <stdlib.h>

#include "prodcons_msg.h"
#include "prodcons_server.h"

static monitor_prodcons * m;

void init_monitor() {

    if(m != NULL)
        return;
    
    m = malloc(sizeof(monitor_prodcons));

    pthread_mutex_init(&m->mutex, NULL);
    pthread_cond_init(&m->cv_cons, NULL);
    pthread_cond_init(&m->cv_prod, NULL);

    m->stato = LIBERO;
    m->buffer = 0;
}

void remove_monitor() {

    if(m == NULL)
        return;

    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->cv_cons);
    pthread_cond_destroy(&m->cv_prod);

    free(m);
    m = NULL;
}

void produci_con_somma(int val1, int val2, int val3) {

    pthread_mutex_lock(&m->mutex);

    while(m->stato == OCCUPATO) {
        pthread_cond_wait(&m->cv_prod, &m->mutex);
    }

    m->buffer = val1 + val2 + val3;
    m->stato = OCCUPATO;

    pthread_cond_signal(&m->cv_cons);

    pthread_mutex_unlock(&m->mutex);
}

void produci(int val) {

    pthread_mutex_lock(&m->mutex);

    while(m->stato == OCCUPATO) {
        pthread_cond_wait(&m->cv_prod, &m->mutex);
    }

    m->buffer = val;
    m->stato = OCCUPATO;

    pthread_cond_signal(&m->cv_cons);

    pthread_mutex_unlock(&m->mutex);
}

int consuma() {

    int valore;

    pthread_mutex_lock(&m->mutex);

    while(m->stato == LIBERO) {
        pthread_cond_wait(&m->cv_cons, &m->mutex);
    }

    valore = m->buffer;
    m->stato = LIBERO;

    pthread_cond_signal(&m->cv_prod);

    pthread_mutex_unlock(&m->mutex);

    return valore;
}