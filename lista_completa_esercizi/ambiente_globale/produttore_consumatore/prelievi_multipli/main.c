#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "prodcons.h"

#define NUM_PRODUTTORI 5
#define NUM_CONSUMATORI 5
#define NUM_PRODUZIONI 6
#define NUM_CONSUMAZIONI 3

/* argomenti passati ad ogni thread: identificativo del thread
   e puntatore alla struttura condivisa */
typedef struct {
    int id;
    ProdCons * p;
} ArgomentiThread;


void * produttore(void * arg)
{
    ArgomentiThread * a = (ArgomentiThread *) arg;

    printf("[Produttore %d] Avvio produttore\n", a->id);

    for (int j = 0; j < NUM_PRODUZIONI; j++)
    {

        int val = rand() % 10;

        produci(a->p, a->id, val);

        sleep(2);
    }

    pthread_exit(NULL);
}


void * consumatore(void * arg)
{
    ArgomentiThread * a = (ArgomentiThread *) arg;

    printf("[Consumatore %d] Avvio consumatore\n", a->id);

    for (int j = 0; j < NUM_CONSUMAZIONI; j++)
    {

        int val_1, val_2;

        consuma(a->p, a->id, &val_1, &val_2);

        sleep(1);
    }

    pthread_exit(NULL);
}


int main()
{

    pthread_t threads[NUM_PRODUTTORI + NUM_CONSUMATORI];
    ArgomentiThread argomenti[NUM_PRODUTTORI + NUM_CONSUMATORI];

    /* lo stato condiviso è una normale struttura allocata nel main:
       i thread condividono lo stesso spazio di indirizzamento */
    ProdCons * p = malloc(sizeof(ProdCons));

    if (p == NULL)
    {
        perror("Errore allocazione struttura condivisa");
        exit(1);
    }

    inizializza(p);

    srand(time(NULL));

    for (int i = 0; i < NUM_PRODUTTORI; i++)
    {

        /* Produttore */

        argomenti[i].id = i;
        argomenti[i].p = p;

        pthread_create(&threads[i], NULL, produttore, (void *) &argomenti[i]);
    }

    for (int i = 0; i < NUM_CONSUMATORI; i++)
    {

        /* Consumatore */

        int k = NUM_PRODUTTORI + i;

        argomenti[k].id = k;
        argomenti[k].p = p;

        pthread_create(&threads[k], NULL, consumatore, (void *) &argomenti[k]);
    }

    printf("[main] Thread main in attesa...\n");

    for (int i = 0; i < NUM_PRODUTTORI + NUM_CONSUMATORI; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("[main] Terminazione\n");

    rimuovi(p);

    free(p);

    return 0;
}
