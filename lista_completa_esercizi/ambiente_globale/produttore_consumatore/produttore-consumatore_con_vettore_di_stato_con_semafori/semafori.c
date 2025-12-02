// Implementazione delle procedure per i semafori


#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include "semafori.h"

void Wait_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}

void Signal_Sem (int id_sem,int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}

// restituisce il numero di processi in attesa sul semaforo
int Queue_Sem(int id_sem, int numsem)     {
	return semctl(id_sem,numsem,GETNCNT,NULL);
}

// restituisce il valore del semaforo
int Leggi_Sem(int id_sem, int numsem) {

  return semctl(id_sem,numsem,GETVAL);
}

