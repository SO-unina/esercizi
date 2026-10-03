#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "header.h"

void* Fornitore(void* arg)
{
	ArgomentiThread* a = (ArgomentiThread*)arg;

	Monitor* M = a->M;
	int* livello_scorte = a->livello_scorte;
	int* scaffali_liberi = a->scaffali_liberi;
	Magazzino* magazzino = a->magazzino;

	int i,k;
	for(k=0;k<15;k++)
	{
		i=0;

		enter_monitor(M);

		/* NOTA: semantica signal-and-continue: la condizione logica va
		   ricontrollata in un ciclo while (con il monitor di Hoare
		   bastava un semplice if) */
		while((*scaffali_liberi)==0)
			wait_condition(M,SPAZIO_D);

		while(i<NELEM && ((*magazzino)[i].stato==OCCUPATO || (*magazzino)[i].stato==IN_USO))
		{
			i++;
		}

		(*magazzino)[i].stato=IN_USO;
		(*scaffali_liberi)--;

		leave_monitor(M);


		//Fornitura
		sleep(1);
		printf("Fornitura effettuata dal Fornitore %d\n",a->id);
		(*magazzino)[i].id_fornitore=a->id;


		enter_monitor(M);

		(*magazzino)[i].stato=OCCUPATO;
		(*livello_scorte)++;

		signal_condition(M,MERCE_D);

		leave_monitor(M);
	}

	pthread_exit(NULL);
}


void* Cliente(void* arg)
{
	ArgomentiThread* a = (ArgomentiThread*)arg;

	Monitor* M = a->M;
	int* livello_scorte = a->livello_scorte;
	int* scaffali_liberi = a->scaffali_liberi;
	Magazzino* magazzino = a->magazzino;

	int i,k;
	for(k=0;k<15;k++)
	{
		i=0;

		enter_monitor(M);

		/* NOTA: semantica signal-and-continue: la condizione logica va
		   ricontrollata in un ciclo while (con il monitor di Hoare
		   bastava un semplice if) */
		while((*livello_scorte)==0)
			wait_condition(M,MERCE_D);

		while(i<NELEM && ((*magazzino)[i].stato==LIBERO || (*magazzino)[i].stato==IN_USO))
		{
			i++;
		}

		(*magazzino)[i].stato=IN_USO;
		(*livello_scorte)--;

		leave_monitor(M);


		//Acquisto
		sleep(1);
		printf("Acquisto effettuato dal Cliente %d, merce venduta dal Fornitore %d \n",a->id,(*magazzino)[i].id_fornitore);
		printf("magazzino[%d].stato=%d\n",i,(*magazzino)[i].stato);


		enter_monitor(M);

		(*magazzino)[i].stato=LIBERO;
		(*scaffali_liberi)++;

		signal_condition(M,SPAZIO_D);

		leave_monitor(M);
	}

	pthread_exit(NULL);
}
