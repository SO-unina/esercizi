#include "sensore.h"
#include "aggregatore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

void aggregatore(int id_coda_sensore, int id_code_collettori[3]) {

    printf("Avvio processo aggregatore...\n");

    pthread_t scrittore;
    pthread_t lettori[3];
    int ret;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);




    MonitorLS * m = malloc(sizeof(MonitorLS));

    m->num_lettori = 0;

    pthread_mutex_init(&m->mutex, NULL);
    pthread_cond_init(&m->cv_scrittore, NULL);




    Parametri * p_scrittore = malloc(sizeof(Parametri));

    p_scrittore->m = m;
    p_scrittore->coda = id_coda_sensore;

    ret = pthread_create(&scrittore, &attr, thread_scrittore, p_scrittore);

    if(ret < 0) {
        printf("Errore pthread_create");
        exit(1);
    }





    Parametri * p_lettori[3];

    for(int i=0; i<3; i++) {

        p_lettori[i] = malloc(sizeof(Parametri));
        p_lettori[i]->m = m;
        p_lettori[i]->coda = id_code_collettori[i];

        ret = pthread_create(&lettori[i], &attr, thread_lettore, p_lettori[i]);

        if(ret < 0) {
          printf("Errore pthread_create");
            exit(1);
        }
    }



    pthread_join(scrittore, NULL);

    for(int i=0; i<3; i++) {

        pthread_join(lettori[i], NULL);
    }


    free(p_scrittore);

    for(int i=0; i<3; i++) {
        free(p_lettori[i]);
    }

    free(m);

}


void * thread_lettore(void * x) {

    Parametri * p = (Parametri *)x;

    for(int i=0; i<10; i++) {

        messaggio msg;
        int valore;
        int ret;


        sleep(1);

        lettura(p->m, &valore);

        msg.tipo = TIPO;
        msg.valore = valore;

        printf("Aggregatore: Invio valore=%d\n", valore);

        ret = msgsnd(p->coda, &msg, sizeof(messaggio) - sizeof(long), 0);

        if(ret < 0) {
            perror("Errore msgsnd");
            exit(1);
        }
    }


    pthread_exit(NULL);
}

void * thread_scrittore(void * x) {

    Parametri * p = (Parametri *)x;

    for(int i=0; i<10; i++) {

        messaggio msg;
        int ret;

        printf("Aggregatore: In attesa di messaggi...\n");

        ret = msgrcv(p->coda, &msg, sizeof(messaggio) - sizeof(long), 0, 0);

        if(ret < 0) {
            printf("Errore msgrcv");
            exit(1);
        }

        printf("Aggregatore: Ricevuto valore=%d\n", msg.valore);

        scrittura(p->m, msg.valore);
    }

    pthread_exit(NULL);
}

void lettura(MonitorLS * m, int * valore) {

    /*
        NOTA: Questa è una versione semplificata dell'algoritmo dei lettori-scrittori,
              che è valida nel caso di un solo scrittore.
     */

    pthread_mutex_lock(&m->mutex);

    m->num_lettori++;

    pthread_mutex_unlock(&m->mutex);


    *valore = m->variabile;

    printf("Aggregatore: Lettura valore=%d\n", *valore);


    pthread_mutex_lock(&m->mutex);

    m->num_lettori--;

    if(m->num_lettori == 0) {
        pthread_cond_signal(&m->cv_scrittore);
    }

    pthread_mutex_unlock(&m->mutex);
}

void scrittura(MonitorLS * m, int valore) {

    /*
        NOTA: Questa è una versione semplificata dell'algoritmo dei lettori-scrittori,
              che è valida nel caso di un solo scrittore.
     */

    pthread_mutex_lock(&m->mutex);

    while(m->num_lettori > 0) {

        pthread_cond_wait(&m->cv_scrittore, &m->mutex);
    }

    printf("Aggregatore: Scrittura valore=%d\n", valore);

    m->variabile = valore;

    pthread_mutex_unlock(&m->mutex);
}
