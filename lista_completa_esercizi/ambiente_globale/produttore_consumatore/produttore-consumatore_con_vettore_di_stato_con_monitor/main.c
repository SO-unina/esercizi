#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "header.h"

int main()
{
	Monitor M;
	int i;

	pthread_t threads[NUMTHREAD];
	ArgomentiThread argomenti[NUMTHREAD];

	/* le variabili condivise non richiedono più una memoria condivisa:
	   sono normali variabili allocate nel main, visibili a tutti i
	   thread attraverso i puntatori passati come argomento */
	int livello_scorte;
	int scaffali_liberi;
	Magazzino magazzino;

	//inizializzazione delle variabili condivise
	livello_scorte=0;
	scaffali_liberi=NELEM;

	for(i=0;i<NELEM;i++)
	{
		magazzino[i].stato=LIBERO;
		magazzino[i].id_fornitore=0;
	}


	//inizializzazione del monitor
	init_monitor(&M,NUMVAR);

	//creazione di 10 fornitori
	for(i=0;i<NUMF;i++)
	{
		printf("Creazione del Fornitore n.ro %d \n",i+1);

		argomenti[i].id = i+1;
		argomenti[i].M = &M;
		argomenti[i].livello_scorte = &livello_scorte;
		argomenti[i].scaffali_liberi = &scaffali_liberi;
		argomenti[i].magazzino = &magazzino;

		pthread_create(&threads[i], NULL, Fornitore, (void*)&argomenti[i]);
	}

	//creazione di 10 clienti
	for(i=0;i<NUMC;i++)
	{
		printf("Creazione del Cliente n.ro %d \n",i+1);

		argomenti[NUMF+i].id = i+1;
		argomenti[NUMF+i].M = &M;
		argomenti[NUMF+i].livello_scorte = &livello_scorte;
		argomenti[NUMF+i].scaffali_liberi = &scaffali_liberi;
		argomenti[NUMF+i].magazzino = &magazzino;

		pthread_create(&threads[NUMF+i], NULL, Cliente, (void*)&argomenti[NUMF+i]);
	}

	for(i=0; i<NUMTHREAD; i++)
	{
		pthread_join(threads[i],NULL);
		printf("Thread n.ro %d terminato \n",i);
	}


	printf("Rimozione del monitor...\n");
	//rimozione del monitor
	remove_monitor(&M);


	printf("ARRIVEDERCI!!!\n");

	return 0;
}
