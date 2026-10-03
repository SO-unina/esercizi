#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/wait.h>

#include "prodcons_msg.h"
#include "prodcons_client.h"

void Produttore();
void Consumatore();

int main() {

    /* Le code POSIX sono aperte/create dai processi figli con
     * init_client(): ogni figlio ha una propria coda di risposta,
     * il cui nome contiene il suo PID. */

    pid_t pid_prod = fork();

    if(pid_prod == 0) {
        init_client();
        Produttore();
        fini_client();
        exit(0);
    }
    else if(pid_prod < 0) {
        perror("Errore fork");
        exit(1);
    }


    pid_t pid_cons = fork();

    if(pid_cons == 0) {
        init_client();
        Consumatore();
        fini_client();
        exit(0);
    }
    else if(pid_cons < 0) {
        perror("Errore fork");
        exit(1);
    }

    wait(NULL);
    wait(NULL);

    /* Rimozione della coda delle richieste: il nome sparisce subito,
     * l'oggetto quando tutti i processi lo hanno chiuso. */
    mq_unlink(CODA_RICHIESTE);
}


void Produttore() {

    srand(getpid());

    int val1 = rand() % 10;
    int val2 = rand() % 10;
    int val3 = rand() % 10;

    printf("[Produttore] Chiamo PRODUCI CON SOMMA(%d, %d, %d)\n", val1, val2, val3);

    produci_con_somma(val1, val2, val3);


    int val = rand() % 10;

    printf("[Produttore] Chiamo PRODUCI(%d)\n", val);

    produci(val);


    val = rand() % 10;

    printf("[Produttore] Chiamo PRODUCI(%d)\n", val);

    produci(val);
}

void Consumatore() {

    printf("[Consumatore] Chiamo CONSUMA()\n");

    int val = consuma();

    printf("[Consumatore] Ricevuto risultato=%d\n", val);



    printf("[Consumatore] Chiamo CONSUMA()\n");

    val = consuma();

    printf("[Consumatore] Ricevuto risultato=%d\n", val);



    printf("[Consumatore] Chiamo CONSUMA()\n");

    val = consuma();

    printf("[Consumatore] Ricevuto risultato=%d\n", val);
}
