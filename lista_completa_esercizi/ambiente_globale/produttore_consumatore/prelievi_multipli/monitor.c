/*************************************Monitor*************************************************/
// Implementazione di un Monitor signal-and-continue mediante i Pthreads
// (mutex + condition variables)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "monitor.h"


/********************IMPLEMENTAZIONE DELLE PROCEDURE***********************/

void init_monitor (Monitor *M, int num_var){

    int i;

    //inizializza il mutex per l'accesso al monitor
    pthread_mutex_init(&M->mutex, NULL);

    M->num_var_cond = num_var;

    //alloca e inizializza le condition variables pthread
    M->conds = malloc(num_var * sizeof(pthread_cond_t));

    //alloca un contatore per ogni variabile condition
    M->cond_counts = malloc(num_var * sizeof(int));

    for (i=0; i<num_var; i++) {
        pthread_cond_init(&M->conds[i], NULL);
        M->cond_counts[i] = 0;
    }

#ifdef DEBUG_
    printf("Monitor inizializzato con %d condition variables. Buona Fortuna ! \n",num_var);
#endif

}


void enter_monitor(Monitor * M){

    pthread_mutex_lock(&M->mutex);

}


void leave_monitor(Monitor* M){

    pthread_mutex_unlock(&M->mutex);

}


void remove_monitor(Monitor* M){

    int i;

    for (i=0; i<M->num_var_cond; i++)
        pthread_cond_destroy(&M->conds[i]);

    pthread_mutex_destroy(&M->mutex);

    free(M->conds);
    free(M->cond_counts);

#ifdef DEBUG_
    printf(" \n Il Monitor è stato rimosso ! Arrivederci \n");
#endif

}


/* NOTA: con la semantica signal-and-continue questa procedura va
 * invocata all'interno di un ciclo while che ricontrolla la
 * condizione logica di attesa. */
void wait_condition(Monitor* M, int id_var){

    M->cond_counts[id_var] = M->cond_counts[id_var] + 1;

    /* la pthread_cond_wait rilascia atomicamente il mutex del monitor
     * e sospende il thread; al risveglio, il mutex viene riacquisito
     * prima di ritornare al chiamante */
    pthread_cond_wait(&M->conds[id_var], &M->mutex);

    M->cond_counts[id_var] = M->cond_counts[id_var] - 1;
}


void signal_condition(Monitor* M, int id_var){

    /* il thread segnalante prosegue l'esecuzione nel monitor
     * (signal-and-continue): il thread risvegliato rientra solo
     * quando il mutex viene rilasciato */
    if (M->cond_counts[id_var] > 0)
        pthread_cond_signal(&M->conds[id_var]);

}


int queue_condition(Monitor * M, int id_var){
    return M->cond_counts[id_var];
}
