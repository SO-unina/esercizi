#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "procedure.h"


/* Per il passaggio degli oggetti monitor dal main() ai thread,
 * sono possibili due soluzioni:
 *  - Dichiarare due puntatori globali (visibili a tutte le funzioni)
 *  - Utilizzare il quarto parametro della funzione pthread_create().
 */

/* Prima soluzione: passaggio tramite puntatori globali */

//MonitorVettore * monitor_vettore;
//MonitorBuffer * monitor_buffer;


/* Seconda soluzione: passaggio tramite pthread_create()
 *
 * NOTA: il thread "aggiornatore" deve prendere in ingresso entrambi
 * gli oggetti monitor. Per passare più di un parametro al thread,
 * bisogna dichiarare una struct contenente la coppia di parametri da passare.
 */

struct parametri {
    MonitorVettore * monitor_vettore;
    MonitorBuffer * monitor_buffer;
};


void * func_generatore(void * p) {
    
    printf("THREAD GENERATORE\n");
    
    
    // Passaggio di parametri tramite pthread_create().

    MonitorVettore * monitor_vettore = (MonitorVettore *) p;

    
    elemento x;
    int i;
    
    for(i=0; i<10; i++) {

        x.a = rand() % 11;
        x.b = rand() % 11;
        
        printf("GENERA: %d %d\n", x.a, x.b);
        
        genera( monitor_vettore, x);
    }

    pthread_exit(NULL);
}

void * func_aggiornatore(void * p) {
    printf("THREAD AGGIORNATORE\n");
    
    
    // Passaggio di parametri tramite pthread_create().
    // Nel caso di puntatori globali, queste istruzioni non sono necessarie.
    
    struct parametri * param = (struct parametri *) p;
    
    MonitorVettore * monitor_vettore = param->monitor_vettore;
    MonitorBuffer * monitor_buffer = param->monitor_buffer;
    
    
    
    elemento x;
    
    int i;
    for(i=0; i<10; i++) {
        
        x = preleva(monitor_vettore);
        
        printf("PRELEVA: %d %d\n", x.a, x.b);
        
        aggiorna(monitor_buffer, x);
        
        sleep(1);
    }
    
    pthread_exit(NULL);
}


void * func_destinatario(void * p) {
    
    printf("THREAD DESTINATARIO\n");

    
    // Passaggio di parametri tramite pthread_create().

    MonitorBuffer * monitor_buffer = (MonitorBuffer *) p;
    

    elemento x;
    int i;
    
    for(i=0; i<6; i++) {
        sleep(2);
        
        consulta( monitor_buffer );
    }

    pthread_exit(NULL);
}


int main() {

    /* Prima soluzione: passaggio tramite puntatori globali */
    
    //monitor_vettore = malloc( sizeof(MonitorVettore) );
    //monitor_buffer = malloc( sizeof(MonitorBuffer) );
    
    
    /* Seconda soluzione: passaggio tramite pthread_create() */
    
    MonitorVettore * monitor_vettore = malloc( sizeof(MonitorVettore) );
    MonitorBuffer * monitor_buffer = malloc( sizeof(MonitorBuffer) );
    
    struct parametri * param = malloc( sizeof(struct parametri) );
    param->monitor_vettore = monitor_vettore;
    param->monitor_buffer = monitor_buffer;
    
    
    
    pthread_mutex_init( &(monitor_vettore->mutex), NULL );
    pthread_cond_init( &(monitor_vettore->cv_produttori), NULL);
    pthread_cond_init( &(monitor_vettore->cv_consumatori), NULL);
    
    monitor_vettore->testa = 0;
    monitor_vettore->coda = 0;
    monitor_vettore->occupati = 0;


    pthread_mutex_init( &(monitor_buffer->mutex), NULL );
    pthread_cond_init( &(monitor_buffer->cv_lettori), NULL);
    pthread_cond_init( &(monitor_buffer->cv_scrittori), NULL);
    
    monitor_buffer->num_lettori = 0;
    monitor_buffer->num_scrittori = 0;
    monitor_buffer->num_scrittori_attesa = 0;

    
    srand(time(NULL));
    
    
    pthread_t thread_generatore;
    pthread_t thread_aggiornatore;
    pthread_t thread_destinatari[3];
    
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    pthread_create( &thread_generatore, &attr, func_generatore, monitor_vettore );
    
    pthread_create( &thread_aggiornatore, &attr, func_aggiornatore, param );
    
    int i;
    for(i=0; i<3; i++) {
        pthread_create( &thread_destinatari[i], &attr, func_destinatario, monitor_buffer );
    }
    
    pthread_join( thread_generatore, NULL);
    pthread_join( thread_aggiornatore, NULL);
    for(i=0; i<3; i++) {
        pthread_join( thread_destinatari[i], NULL);
    }

    
    free(monitor_buffer);
    free(monitor_vettore);
    free(param);
    
}
    
    
