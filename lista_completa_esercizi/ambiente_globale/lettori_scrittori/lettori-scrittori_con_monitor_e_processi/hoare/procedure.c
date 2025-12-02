#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

void InizioLettura(MonitorMeteo* p){
	enter_monitor(&(p->m));

	if (p->numscrittori > 0) //se ci sono già scrittori, si deve bloccare
		wait_condition(&(p->m), CV_LETT);

	p->numlettori = p->numlettori + 1;

	signal_condition(&(p->m), CV_LETT); // sblocco di eventuali altri lettori,
							// poiché lo scrittore riattiva un solo lettore

	leave_monitor(&(p->m));
}

void FineLettura(MonitorMeteo* p) {
	enter_monitor(&(p->m));

	p->numlettori = p->numlettori - 1;

	if (p->numlettori == 0) {// se sono finiti i lettori lo segnala agli scrittori

		signal_condition(&(p->m), CV_SCRITT);
	}

	leave_monitor(&(p->m));
}



//Procedure di inizio e fine scrittura

void InizioScrittura(MonitorMeteo* p){
	enter_monitor(&(p->m));

	if (p->numlettori > 0 || p->numscrittori > 0) //se ci sono già lettori o scrittori, si deve bloccare
		wait_condition(&(p->m), CV_SCRITT);

	p->numscrittori = 1;

	leave_monitor(&(p->m));
}

void FineScrittura(MonitorMeteo* p){
	enter_monitor(&(p->m));

	p->numscrittori = 0;

	if (queue_condition(&(p->m), CV_SCRITT)>0) { // sblocco di eventuali altri scrittori

		signal_condition(&(p->m), CV_SCRITT);

	} else if (queue_condition(&(p->m), CV_LETT)>0) { // sblocco di eventuali altri lettori

		signal_condition(&(p->m), CV_LETT);

	}

	leave_monitor(&(p->m));
}



void Servizio(MonitorMeteo* p){

	srand(time(0));

	int i;
	for(i=0; i<20; i++) {

		InizioScrittura(p);

		p->meteo.temperatura = (rand()%101)-50;
		p->meteo.umidita = rand()%101;
		p->meteo.pioggia = rand()%2;

		printf("<%d> scrittura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), p->meteo.temperatura, p->meteo.umidita, (p->meteo.pioggia ? "si" : "no") );

		FineScrittura(p);

		sleep(2);
	}

}

void Utente(MonitorMeteo* p) {

	int i;
	for(i=0; i<10; i++) {

		InizioLettura(p);

		printf("<%d> lettura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), p->meteo.temperatura, p->meteo.umidita, (p->meteo.pioggia ? "si" : "no") );

		FineLettura(p);

		sleep(1);
	}

}


