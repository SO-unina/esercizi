#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "monitor_hoare.h"
#include "header.h"

int main()
{
	Monitor M;
	int id_scorte, id_liberi, id_magazzino;
	int i, pid,status;

	int* livello_scorte;
	int* scaffali_liberi;
	Magazzino * magazzino;
	
	
	//richiesta della memoria condivisa
	id_scorte = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT | 0664);
	id_liberi = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT | 0664);
	id_magazzino = shmget(IPC_PRIVATE,sizeof(Magazzino),IPC_CREAT | 0664);
	
	//attach della memoria condivisa
	livello_scorte = (int *) shmat(id_scorte,0,0);
	scaffali_liberi = (int *) shmat(id_liberi,0,0);
	magazzino = (Magazzino *) shmat(id_magazzino,0,0);
	
	//inizializzazione della memoria condivisa;
	*livello_scorte=0;
	*scaffali_liberi=NELEM;

	for(i=0;i<NELEM;i++)
	{
		(*magazzino)[i].stato=LIBERO;	
	} 
	
	
	//inizializzazione del monitor
	init_monitor(&M,NUMVAR);
	
	//creazione di 10 fornitori
	for(i=0;i<NUMF;i++)
	{
		pid=fork();
		if(!pid)
		{
			printf("Creazione del Fornitore n.ro %d \n",i);
			Fornitore(&M,livello_scorte,scaffali_liberi,magazzino);
			_exit(0);
		}
	}
	
	//creazione di 10 clienti
	for(i=0;i<NUMC;i++)
	{
		pid=fork();
		if(!pid)
		{
			printf("Creazione del Cliente n.ro %d \n",i);
			Cliente(&M,livello_scorte,scaffali_liberi,magazzino);
			_exit(0);
		}
	}
	
	for(i=0; i<NUMPROC; i++)
	{
		pid=wait(&status);
		printf("Processo %d terminato con stato %d \n",pid,status>>8);
	}
	
	
	
	
	
	printf("Rimozione della memoria condivisa e del monitor...\n");
	//rimozione del monitor
	remove_monitor(&M);
	
	//rimozione della memoria condivisa
	shmctl(id_scorte,IPC_RMID,0);
	shmctl(id_liberi,IPC_RMID,0);
	shmctl(id_magazzino,IPC_RMID,0);
	
	
	printf("ARRIVEDERCI!!!\n");
	
	return 0;
}

