 /*****************************************************************************
* Programma per il calcolo multiprocesso del prodotto scalare tra due vettori

******************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

/*
Struttura contenente il risultato
*/
typedef struct
 {
   sem_t   mutex; // mutua esclusione sull'aggiornamento della somma
   double  sum; // somma parziale
   int     veclen; // lunghezza vettore parziale
 } DOTDATA;

/*
VECLEN è la dimensione di ogni sottoporzione.
La dimensione totale dei vettori è NUMPROC*VECLEN
*/
#define NUMPROC 40
#define VECLEN 10000

/* Nomi degli oggetti di shared memory POSIX */
#define SHM_A   "/prodscal_a"
#define SHM_B   "/prodscal_b"
#define SHM_RES "/prodscal_res"

/* Crea un oggetto di shared memory POSIX della dimensione indicata
   e lo mappa nello spazio di indirizzamento (shm_open+ftruncate+mmap) */
static void *crea_shm(const char *name, size_t size)
{
        shm_unlink(name); /* rimuove eventuali residui di esecuzioni precedenti */

        int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0664);
        if (fd == -1) {
                perror("shm_open");
                exit(1);
        }
        if (ftruncate(fd, size) == -1) {
                perror("ftruncate");
                exit(1);
        }
        void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (p == MAP_FAILED) {
                perror("mmap");
                exit(1);
        }
        close(fd);
        return p;
}

/*
La funzione dotprod elabora il prodotto scalare di due porzioni
dei vettori di ingresso
l'output è scritto nella struttura condivisa dotstr.
*/

void dotprod(int offset, double * a, double * b, DOTDATA * dotstr)
{

/* Variabili locali */

   int i, start, end, len ;
   double mysum, *x, *y;

/* calcolo della parte dei vettori su cui operare.
  Si osservi che i vettori a e b sono condivisi tra i processi, ed accessibili
  in lettura senza bisogno di mutex */
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
   sem_wait(&dotstr->mutex);
   dotstr->sum += mysum;
   sem_post(&dotstr->mutex);

}

/*
Programma principale:
inizializza i vettori,
crea i processi indicando loro la parte
dei vettori sulla quale operare.
*/

int main (int argc, char *argv[]){

        DOTDATA* dotstr;
        int i, pid;
        double *a, *b;
        int status;

        /* Alloca spazio per i vettori condivisi nella memoria condivisa POSIX */

        a = crea_shm(SHM_A, NUMPROC*VECLEN*sizeof(double));
        b = crea_shm(SHM_B, NUMPROC*VECLEN*sizeof(double));

        /* Alloca spazio per la struttura contenente il risultato
           (che include anche il semaforo di mutua esclusione) */

        dotstr = crea_shm(SHM_RES, sizeof(DOTDATA));

        //inizializzazione del semaforo anonimo process-shared (pshared=1)
        if (sem_init(&dotstr->mutex, 1, 1) == -1) {
                perror("sem_init");
                exit(1);
        }

        // inizializzazione vettori
        for (i=0; i<VECLEN*NUMPROC; i++) {
                a[i]=1;
                b[i]=a[i];
        }

        dotstr->veclen = VECLEN;
        dotstr->sum=0;

        /* calcolo del tempo di calcolo: istante iniziale */
        struct timeval t1;
        gettimeofday(&t1,NULL);

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
                dotprod(i, a, b, dotstr);
                exit(0);
           }
        }

        /* Aspetta la terminazione dei processi */

        for(i=0;i<NUMPROC;i++) {
                wait(&status);
        }
        /* stampa i risultati e ripulisce le risorse IPC usate */

        /* calcolo del tempo di calcolo: istante finale */
        gettimeofday(&t1,NULL);
        long end_time = t1.tv_sec*1000000+t1.tv_usec;;
        long elapsed = end_time-start_time;

        printf ("\nProdotto scalare =  %f \nTempo di esecuzione = %ld\n\n", dotstr->sum,elapsed);

        sem_destroy(&dotstr->mutex);

        shm_unlink(SHM_A);
        shm_unlink(SHM_B);
        shm_unlink(SHM_RES);

        return 0;
}
