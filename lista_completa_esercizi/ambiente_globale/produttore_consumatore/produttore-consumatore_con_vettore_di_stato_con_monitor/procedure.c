#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "monitor_hoare.h"
#include "header.h"

void Fornitore(Monitor* M, int* livello_scorte, int* scaffali_liberi, Magazzino* magazzino)
{
	int i,k;
	for(k=0;k<15;k++)
	{
		i=0;

		enter_monitor(M);

		if((*scaffali_liberi)==0)
			wait_condition(M,SPAZIO_D);

		while(i<NELEM && ((*magazzino)[i].stato==OCCUPATO || (*magazzino)[i].stato==IN_USO))
		{
			i++;
		}

		(*magazzino)[i].stato=IN_USO;
		(*scaffali_liberi)--;

		leave_monitor(M);


		//Fornitura
		sleep(2);
		printf("Fornitura effettuata dal Fornitore %d\n",getpid());
		(*magazzino)[i].id_fornitore=getpid();
		
		
		enter_monitor(M);

		(*magazzino)[i].stato=OCCUPATO;
		(*livello_scorte)++;

		signal_condition(M,MERCE_D);

		leave_monitor(M);
	}
}


void Cliente(Monitor* M, int* livello_scorte, int* scaffali_liberi, Magazzino* magazzino)
{
	int i,k;
	for(k=0;k<15;k++)
	{
		i=0;

		enter_monitor(M);

		if((*livello_scorte)==0)
			wait_condition(M,MERCE_D);

		while(i<NELEM && ((*magazzino)[i].stato==LIBERO || (*magazzino)[i].stato==IN_USO))
		{
			i++;
		}

		(*magazzino)[i].stato=IN_USO;
		(*livello_scorte)--;

		leave_monitor(M);


		//Acquisto
		sleep(2);
		printf("Acquisto effettuato, merce venduta dal Fornitore %d \n",(*magazzino)[i].id_fornitore);
		printf("magazzino[%d].stato=%d\n",i,(*magazzino)[i].stato);
		

		enter_monitor(M);

		(*magazzino)[i].stato=LIBERO;
		(*scaffali_liberi)++;

		signal_condition(M,SPAZIO_D);
		
		leave_monitor(M);
	}
}
