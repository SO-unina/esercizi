/*************************************Monitor*************************************************/
// Implementazione di un Monitor signal-and-return


#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

#include "monitor_signal_return.h"


//Funzioni di utilita' private alla libreria Monitor

static void Wait_Sem(int, int);
static void Signal_Sem (int,int);
static int Queue_Sem (int,int);   //restituisce il num di processi in attesa su un semaforo


/********************IMPLEMENTAZIONE DELLE PROCEDURE***********************/

void init_monitor (Monitor *M,int num_var){
    int i;
    M->num_var_cond=num_var;
    M->mutex=semget(IPC_PRIVATE,1,IPC_CREAT|0664);
    M->id_conds=semget(IPC_PRIVATE,num_var,IPC_CREAT|0664);
    semctl(M->mutex,0,SETVAL,1);
    for (i=0;i<num_var;i++)
         semctl(M->id_conds,i,SETVAL,0);
    M->id_shared=shmget(IPC_PRIVATE,num_var*sizeof(int),IPC_CREAT|0664);
    M->cond_counts=(int*) (shmat(M->id_shared,0,0));
    for (i=0;i<num_var;i++)
        M->cond_counts[i]=0;
#ifdef DEBUG_
    printf("Monitor inizializzato con %d condition variables. Buona Fortuna ! \n",num_var);
#endif

} 



void enter_monitor(Monitor * M){

#ifdef DEBUG_
    printf("<%d> Tentativo di ingresso nel monitor... \t",getpid() );
#endif
     Wait_Sem(M->mutex,0);
#ifdef DEBUG_
    printf("<%d> Entrato nel monitor \n",getpid() );
#endif

}


void leave_monitor(Monitor* M){
#ifdef DEBUG_
    printf("<%d> Uscito dal monitor  \n", getpid());
#endif
     Signal_Sem(M->mutex,0);
}


void remove_monitor(Monitor* M){
    semctl(M->mutex,0,IPC_RMID,0);
    semctl(M->id_conds,M->num_var_cond,IPC_RMID,0);
    shmctl(M->id_shared,IPC_RMID,0);
#ifdef DEBUG_
    printf(" \n Il Monitor è stato rimosso ! Arrivederci \n", getpid());
#endif

}

void wait_condition(Monitor* M,int id_var){
#ifdef DEBUG_
    printf("<%d> -Monitor- invocata la wait sulla condition numero %d\n", getpid(), id_var);
#endif
      M->cond_counts[id_var]=M->cond_counts[id_var]+1;
      Signal_Sem(M->mutex,0);
      Wait_Sem(M->id_conds,id_var);
      M->cond_counts[id_var]=M->cond_counts[id_var]-1;
}

void signal_condition(Monitor* M,int id_var){
#ifdef DEBUG_
            printf("<%d> -Monitor- tentativo di signal; n.ro proc. in attesa sulla cond. n. %d = %d\n", getpid(), id_var,M->cond_counts[id_var]);
#endif     

      if(M->cond_counts[id_var]>0) {
            Signal_Sem(M->id_conds,id_var);
#ifdef DEBUG_
            printf("<%d> -Monitor- invocata la signal sulla condition numero %d\n", getpid(), id_var);
#endif
     } else {
            Signal_Sem(M->mutex,0);
#ifdef DEBUG_
            printf("<%d> -Monitor- invocata la signal sul mutex \n", getpid());
#endif
      }
 }

int queue_condition(Monitor * M, int id_var){
	return M->cond_counts[id_var];
}

/********************IMPLEMENTAZIONE DELLE PROCEDURE SEMAFORICHE***********************/

void Wait_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}

// restituisce il numero di processi in attesa sul semaforo
int Queue_Sem(int id_sem, int numsem)     {
	return (semctl(id_sem,numsem,GETNCNT,NULL));
}

void Signal_Sem (int id_sem,int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}
