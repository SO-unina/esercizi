 /*****************************************************************************
* Programma per il calcolo multithread del prodotto scalare tra due vettori

******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

/*   
Struttura contenente i vettori e il risultato
*/
typedef struct{
   
        double      *a;
        double      *b;
        double     sum; 
        int     veclen;

} DOTDATA;

/* 
Variabili globali e mutex 
VECLEN è la dimensione di ogni sottoporzione.
La dimensione totale dei vettori è NUMTHRDS*VECLEN
*/

#define NUMTHRDS 40
#define VECLEN 10000

DOTDATA dotstr; 

pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;

/*
La funzione dotprod è la start function dei thread.
L'input della funzione è ottenuto da una struttura di tipo DOTDATA
e l'output è scritto nella struttura.
*/

void *dotprod(void *arg)
{

/* Variabili locali */

   int i, start, end, offset, len ;
   double mysum, *x, *y;

/* spiazzamento nei vettori dal quale partire a calcolare il prodotto*/
   offset = (int)arg;
/* calcolo della sottoporzione dei vettori su cui operare.
  Si osservi che la struttura dotstr è condivisa tra i thread, ed accessibile
  in lettura senza bisgono di mutex */
   len = dotstr.veclen;
   start = offset*len;
   end   = start + len;
   x = dotstr.a;
   y = dotstr.b;

/*
Effettua il prodotto scalare ed assegna il risultato alla variabile locale mysum
*/

   mysum = 0;
   for (i=start; i<end ; i++) 
    {
      //printf("*");	
      mysum += (x[i] * y[i]);
    }

/*
Blocco sul mutex prima di aggiornare la variabile condivisa dotstr.sum
e sblocco dopo aver aggiornato.
*/
   pthread_mutex_lock (&mutexsum);
   dotstr.sum += mysum;
   pthread_mutex_unlock (&mutexsum);

   pthread_exit((void*) 0);
}

/* 
Programma principale: 
inizializza i vettori,
crea i thread worker indicando loro la sottoporzione
dei vettori sulla quale operare.
*/

int main (int argc, char *argv[])
{
	int i;
	double *a, *b;
	void *status;
	
        pthread_attr_t attr;

	/* Alloca spazio per i vettori condivisi nell'area heap */

	a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
	b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));


	for (i=0; i<VECLEN*NUMTHRDS; i++) {
	  a[i]=1;
	  b[i]=a[i];
	}

	dotstr.veclen = VECLEN; 
	dotstr.a = a; 
	dotstr.b = b; 
	dotstr.sum=0;

	/* inizializza il mutex*/
	pthread_mutex_init(&mutexsum, NULL);

	/* calcolo del tempo di calcolo: istante iniziale */
	struct timeval t1;
	struct timezone t2;
	gettimeofday(&t1,&t2);
	long start_time = t1.tv_sec*1000000+t1.tv_usec;;

	/* 
	  Crea i thread per effettuare il prodotto scalare 
	  i thread vanno definiti come joinable
	*/
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
        int ret=-1;
	for(i=0;i<NUMTHRDS;i++)
	{
	  /* 
	    Ogni thread lavora su una differente sottoporzione di dati 
	    Lo spiazzamento è specificato da 'i'. La dimensione dei dati
	    di ogni sottoporzione è VECLEN
	   */
	   ret = pthread_create( &callThd[i], &attr, dotprod, (void *)i); 
           /*
           if (ret!=0)
                printf("ERRROOOOOOOOOOOOOOOSS\n");
           */
	}


	/* Aspetta la terminazione dei thread */

	int k;
	for(k=0; k < NUMTHRDS; k++) {
          //printf("JOIN k: %d\n", k);
	  pthread_join( callThd[k], &status);
	  //pthread_join( callThd[k], NULL);
	}

	/* Dopo il join, stampa i risultati e ripulisce la memoria */

	/* calcolo del tempo di calcolo: istante finale */
	gettimeofday(&t1,&t2);
	long end_time = t1.tv_sec*1000000+t1.tv_usec;;
	long elapsed = end_time-start_time;
	printf ("\nProdotto scalare =  %f \nTempo di esecuzione = %ld\n\n", dotstr.sum,elapsed);

	/*rimuove l'attributo usato per definire i thread come joinable*/
	
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutexsum);
	free (a);
	free (b);

	pthread_exit(0);
}
