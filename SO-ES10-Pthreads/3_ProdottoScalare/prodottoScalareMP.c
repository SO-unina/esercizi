 /*****************************************************************************
* Programma per il calcolo multiprocesso del prodotto scalare tra due vettori

******************************************************************************/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

//Procedure per la sincronizzazione sui semafori

void Wait_Sem(int id_sem, int numsem){

       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}


void Signal_Sem (int id_sem, int numsem){

       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}


/*   
Struttura contenente il risultato
*/
typedef struct 
 {
   double  sum; // somma parziale
   int     veclen; // lunghezza vettore parziale
 } DOTDATA;

/* 
VECLEN è la dimensione di ogni sottoporzione.
La dimensione totale dei vettori è NUMPROC*VECLEN
*/
#define NUMPROC 40
#define VECLEN 10000
    
   
/*
La funzione dotprod elabora il prodotto scalare di due porzioni
dei vettori di ingresso
l'output è scritto nella struttura condivisa dotstr.
*/

void dotprod(int offset, double * a, double * b, DOTDATA * dotstr, int mutex)
{

/* Variabili locali */

   int i, start, end, len ;
   double mysum, *x, *y;


/* calcolo della parte dei vettori su cui operare.
  Si osservi che i vettori a e b sono condivisi tra i processi, ed accessibili
  in lettura senza bisgono di mutex */
   len = dotstr->veclen;
   start = offset*len;
   end   = start + len;
   x = a;
   y = b;

/*
Effettua il prodotto scalare ed assegna il risultato alla variabile locale mysum
*/

   mysum = 0;
   for (i=start; i<end ; i++) 
    {
      mysum += (x[i] * y[i]);
    }

/*
Blocco sul mutex prima di aggiornare la variabile condivisa dotstr.sum
e sblocco dopo aver aggiornato.
*/
   Wait_Sem(mutex,0);
   dotstr->sum += mysum;
   Signal_Sem(mutex,0);

}

/* 
Programma principale: 
inizializza i vettori,
crea i processi indicando loro la parte
dei vettori sulla quale operare.
*/

int main (int argc, char *argv[]){

        DOTDATA* dotstr;
        int id_bufferA, id_bufferB, id_bufferRes, mutex;
        int i, pid;
        double *a, *b;
        int status;


        /* Alloca spazio per i vettori condivisi nella memoria condivisa */

        id_bufferA = shmget(IPC_PRIVATE, NUMPROC*VECLEN*sizeof(double), IPC_CREAT | 0664);
        a = (double *) ( shmat(id_bufferA,0,0));

        id_bufferB = shmget(IPC_PRIVATE, NUMPROC*VECLEN*sizeof(double), IPC_CREAT | 0664);
        b = (double *) ( shmat(id_bufferB,0,0));

        /* Alloca spazio per la struttura contenente il risultato*/

        id_bufferRes = shmget(IPC_PRIVATE, sizeof(DOTDATA), IPC_CREAT | 0664);
        dotstr = (DOTDATA *) ( shmat(id_bufferRes,0,0));

        //richiesta dei  semafori  ed inizializzazione
        mutex=semget(IPC_PRIVATE,1,IPC_CREAT|0664);
        semctl(mutex,0,SETVAL,1);

        // inizializzazione vettori
        for (i=0; i<VECLEN*NUMPROC; i++) {
                a[i]=1;
                b[i]=a[i];
        }

        dotstr->veclen = VECLEN; 
        dotstr->sum=0;

        /* calcolo del tempo di calcolo: istante iniziale */
        struct timeval t1;
        struct timezone t2;
        gettimeofday(&t1,&t2);

        long start_time = t1.tv_sec*1000000+t1.tv_usec;;

        /* 
          Crea i processi per effettuare il prodotto scalare 
          */

        for(i=0;i<NUMPROC;i++)
        {
          /* 
            Ogni processo lavora su una differente parte di dati 
            Lo spiazzamento è specificato da 'i'. La dimensione dei dati
            di ogni parte è VECLEN
           */
           pid = fork();

           if (pid == 0) {
                dotprod(i, a, b, dotstr, mutex); 
                exit(0);
           }  
        }

        /* Aspetta la terminazione dei processi */

        for(i=0;i<NUMPROC;i++) {
                wait(&status);
        }
        /* stampa i risultati e ripulisce le risorse IPC usate */

        /* calcolo del tempo di calcolo: istante finale */
        gettimeofday(&t1,&t2);
        long end_time = t1.tv_sec*1000000+t1.tv_usec;;
        long elapsed = end_time-start_time;
        
        printf ("\nProdotto scalare =  %f \nTempo di esecuzione = %ld\n\n", dotstr->sum,elapsed);
        
        shmctl(id_bufferA, IPC_RMID, 0);
        shmctl(id_bufferB, IPC_RMID, 0);
        shmctl(id_bufferRes, IPC_RMID, 0);
        semctl(mutex, IPC_RMID, 0);

        return 0;
}
