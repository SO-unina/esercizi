//
//  procedure.h
//  


#ifndef procedure_h
#define procedure_h

#include <pthread.h>

typedef struct { int a; int b; } elemento;

typedef struct {
    elemento vettore[5];

    // inserire qui variabili per la sincronizzazione
    pthread_mutex_t mutex;
    pthread_cond_t cv_produttori;
    pthread_cond_t cv_consumatori;
    
    int testa;
    int coda;
    int occupati;

} MonitorVettore;

typedef struct {
    
    elemento buffer;
    
    // inserire qui variabili per la sincronizzazione
    pthread_mutex_t mutex;
    pthread_cond_t cv_lettori;
    pthread_cond_t cv_scrittori;
    
    int num_scrittori;
    int num_lettori;
    int num_scrittori_attesa;
    
} MonitorBuffer;

// produce un elemento
void genera(MonitorVettore *v, elemento e);

// consuma un elemento
elemento preleva(MonitorVettore *v);

// scrive sul buffer
void aggiorna(MonitorBuffer *b, elemento e);

// legge dal buffer
void consulta(MonitorBuffer *b);

#endif /* procedure_h */
