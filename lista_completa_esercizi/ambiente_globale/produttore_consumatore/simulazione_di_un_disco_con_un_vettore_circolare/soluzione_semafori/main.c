#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "coda_richieste.h"

#define DIM_DISCO 20

void Schedulatore(coda_richieste *c)
{

    int disco[DIM_DISCO];
    int posizione_corrente = 0;

    richiesta r;

    for (int i = 0; i < 25; i++)
    {

        preleva_richiesta(c, &r);

        printf("[%d] Prelevo richiesta: posizione=%d, processo=%d\n", getpid(), r.posizione, r.processo);

        int attesa = abs(posizione_corrente - (int)r.posizione);

        printf("[%d] Attesa Schedulatore... (%d secondi)\n", getpid(), attesa);

        sleep(attesa);

        disco[r.posizione] = r.processo;

        posizione_corrente = r.posizione;
    }
}

void Utente(coda_richieste *c)
{

    srand(getpid());

    richiesta r;

    for (int i = 0; i < 5; i++)
    {

        r.processo = getpid();
        r.posizione = rand() % DIM_DISCO;

        printf("[%d] Richiesta Utente: posizione=%d, processo=%d\n",getpid(), r.posizione, r.processo);

        inserisci_richiesta(c, &r);
    }
}

int main()
{

    coda_richieste * c;

    c = inizializza_coda();

    pid_t pid_sched = fork();

    if (pid_sched == 0)
    {
        /* figlio */

        printf("[%d] Avvio processo Schedulatore\n", getpid());

        Schedulatore(c);
        exit(0);
    }
    else if (pid_sched < 0)
    {
        perror("Errore creazione processo Schedulatore");
        exit(1);
    }

    for (int i = 0; i < 5; i++)
    {

        pid_t pid_utente = fork();

        if (pid_utente == 0)
        {
            /* figlio */

            printf("[%d] Avvio processo Utente\n", getpid());

            Utente(c);
            exit(0);
        }
        else if (pid_utente < 0)
        {
            perror("Errore creazione processo Utente");
            exit(1);
        }
    }


    for(int i=0; i<6; i++) {
        wait(NULL);
    }


    rimuovi_coda(c);

}
