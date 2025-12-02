#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void * thread_richiedente(void * p) {

	MonitorElaborazioni * m = (MonitorElaborazioni *)p;

	int i;
	for(i=0; i<3; i++) {

		buffer b;

		b.totale_operandi = rand() % 3 + 2;

		printf("[PRODUTTORE] inizio produzione\n\n");

		int j;
		for( j=0; j<b.totale_operandi; j++ ) {

			b.operandi[j] = rand() % 11;

		}


		produci_richiesta(m, &b);


		printf("[PRODUTTORE] op1=%d, op2=%d, op3=%d, op4=%d\n\n",
					b.operandi[0],
					b.operandi[1],
					(b.totale_operandi >= 3 ? b.operandi[2] : -1),
					(b.totale_operandi >= 4 ? b.operandi[3] : -1)
		);




		printf("[PRODUTTORE] fine produzione\n\n");
	}


	pthread_exit(0);
}


void * thread_elaboratore(void * p) {

	MonitorElaborazioni * m = (MonitorElaborazioni *)p;

	int i;
	for(i=0; i<6; i++) {

		buffer b;
		int somma;


		printf("[CONSUMATORE] inizio consumazione\n\n");


		somma = consuma_richiesta(m, &b);


		printf("[CONSUMATORE] op1=%d, op2=%d, op3=%d, op4=%d, somma=%d\n\n",
					b.operandi[0],
					b.operandi[1],
					(b.totale_operandi >= 3 ? b.operandi[2] : -1),
					(b.totale_operandi >= 4 ? b.operandi[3] : -1),
					somma
		);


		printf("[CONSUMATORE] fine consumazione\n\n");
	}


	pthread_exit(0);
}



int main() {

	MonitorElaborazioni * m = (MonitorElaborazioni *) malloc(sizeof(MonitorElaborazioni));

	pthread_t threads_richiedenti[4];
	pthread_t threads_elaboratori[2];

	pthread_attr_t attr;

	int i, ret;


	inizializza(m);

	srand(time(NULL));


	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	for(i=0; i<4; i++) {

		ret = pthread_create( &threads_richiedenti[i], &attr, thread_richiedente, (void *)m);

		if(ret) {
			printf("Errore creazione thread (%d)\n", ret);
			exit(-1);
		}

		sleep(1);
	}


	for(i=0; i<2; i++) {

		ret = pthread_create( &threads_elaboratori[i], &attr, thread_elaboratore, (void *)m);

		if(ret) {
			printf("Errore creazione thread (%d)\n", ret);
			exit(-1);
		}

		sleep(1);
	}




	for(i=0; i<4; i++) {

		ret = pthread_join( threads_richiedenti[i], NULL );

		if(ret) {
			printf("Errore join thread (%d)\n", ret);
			exit(-1);
		}
	}


	for(i=0; i<2; i++) {

		ret = pthread_join( threads_elaboratori[i], NULL );

		if(ret) {
			printf("Errore join thread (%d)\n", ret);
			exit(-1);
		}
	}


	distruggi(m);

}

