//
//  procedure.c
//  


#include <stdio.h>

#include "procedure.h"


// produce un elemento
void genera(MonitorVettore *v, elemento e) {
    
    pthread_mutex_lock( & v->mutex );
    
    while( v->occupati == 5 ) {
        pthread_cond_wait( & v->cv_produttori, & v->mutex );
    }
    
    v->vettore[ v->testa ] = e;

    v->testa = (v->testa + 1) % 5;
    
    v->occupati++;
    
    pthread_cond_signal( & v->cv_consumatori );
    
    pthread_mutex_unlock( & v->mutex );
}

// consuma un elemento
elemento preleva(MonitorVettore *v) {
    
    elemento e;
    
    pthread_mutex_lock( & v->mutex );
    
    while( v->occupati == 0 ) {
        pthread_cond_wait( & v->cv_consumatori, & v->mutex );
    }
    
    e = v->vettore[ v->coda ];
    
    v->coda = (v->coda + 1) % 5;
    
    v->occupati--;
    
    pthread_cond_signal( & v->cv_produttori );
    
    pthread_mutex_unlock( & v->mutex );
    
    
    return e;
}

// scrive sul buffer
void aggiorna(MonitorBuffer *b, elemento e) {
    
    pthread_mutex_lock( & b->mutex );

    while( b->num_lettori > 0 || b->num_scrittori > 0 ) {
        b->num_scrittori_attesa++;
        pthread_cond_wait( & b->cv_scrittori, & b->mutex );
        b->num_scrittori_attesa--;
    }
    
    b->num_scrittori++;
    
    pthread_mutex_unlock( & b->mutex );
    
    
    
    printf("SCRITTURA: %d %d\n", e.a, e.b);
    
    b->buffer = e;
    
    
    
    pthread_mutex_lock( & b->mutex );

    b->num_scrittori--;
    
    if( b->num_scrittori_attesa > 0 ) {
        pthread_cond_signal( & b->cv_scrittori );
    }
    else {
        pthread_cond_broadcast( & b->cv_lettori );
    }
    
    pthread_mutex_unlock( & b->mutex );
}

// legge dal buffer
void consulta(MonitorBuffer *b) {

    pthread_mutex_lock( & b->mutex );
    
    while( b->num_scrittori > 0 ) {
        pthread_cond_wait( & b->cv_lettori, & b->mutex );
    }
    
    b->num_lettori++;
    
    pthread_mutex_unlock( & b->mutex );
    
    
    
    printf("LETTURA: %d + %d = %d\n", b->buffer.a, b->buffer.b, b->buffer.a+b->buffer.b);
    
    
    
    pthread_mutex_lock( & b->mutex );
    
    b->num_lettori--;
    
    if( b->num_lettori == 0 ) {
        pthread_cond_signal( & b->cv_scrittori );
    }
    
    pthread_mutex_unlock( & b->mutex );
}


