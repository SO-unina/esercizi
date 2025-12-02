#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void * produttore(void * p) {

	MonitorCoda * m = (MonitorCoda *) p;
	int valore;
	int ret;
	int i;

	for(i=0; i<4; i++) {

		while(1) {

			valore = rand() % 10;

			ret = produzione(m, valore);

			if(ret==0) { break; }

			printf("MONITOR SOVRACCARICO, ATTENDO...\n");

			sleep(3);
		}

		printf("VALORE PRODOTTO: %d\n", valore);

		sleep(1);
	}


	pthread_exit(NULL);
}


void * consumatore(void * p) {

	MonitorCoda * m = (MonitorCoda *) p;
	int i;
	int valore;

	for(i=0; i<16; i++) {

		valore = consumazione(m);

		printf("VALORE CONSUMATO: %d\n", valore);

		sleep(2);
	}


	pthread_exit(NULL);
}


int main() {

	pthread_t thread_produttori[4];
	pthread_t thread_consumatore;

	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE);


	MonitorCoda * m = (MonitorCoda *) malloc(sizeof(MonitorCoda));

	inizializza(m);


	srand(time(NULL));


	int i;

	for(i=0; i<4; i++) {

		pthread_create( &thread_produttori[i], &attr, produttore, m);
	}


	pthread_create( &thread_consumatore, &attr, consumatore, m);


	for(i=0; i<4; i++) {

		pthread_join(thread_produttori[i], NULL);
	}


	pthread_join(thread_consumatore, NULL);

	free(m);
}

