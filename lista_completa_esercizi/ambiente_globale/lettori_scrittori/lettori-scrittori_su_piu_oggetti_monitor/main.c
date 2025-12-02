#include "header.h"

void * Capotreno (void * p){

	struct monitor* m = (struct monitor*) p;

	int i;
	int s;

	s=leggi_stazione(m);

	for(i=0; i<10; i++){

		s=s+1;

		scrivi_stazione(m,s);

		printf("[CAPOSTAZIONE] Posizione del treno %d modificata. La nuova posizione è: %d\n", m->id_treno, s);

		sleep(3);
	}

	pthread_exit(NULL);
}

void * Viaggiatori (void * p){

	struct monitor* m = (struct monitor*) p;

	int i;
	int ris;

	for(i=0;i<3;i++){

		sleep(rand()%6+1);

		ris=leggi_stazione(m);

		printf("[VIAGGIATORE] Il treno n.ro %d è nella %da stazione\n", m->id_treno, ris);
	}

	pthread_exit(NULL);
}

int main() {

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	srand(time(NULL));

	struct monitor* m[4];	// array di 4 oggetti-monitor, per gestire i 4 treni

	pthread_t capo[4];		// 4 capitreno
	pthread_t viagg[10];	// 10 viaggiatori

	int i;


	//allocazione dei 4 monitor

	for(i=0;i<4;i++){
		m[i] = (struct monitor*) malloc(sizeof(struct monitor));
	}


	//inizializzazione

	for(i=0;i<4;i++){
		inizializza(m[i]);
	}

	// assegno un id ad ogni treno
	m[0]->id_treno=1;
	m[1]->id_treno=2;
	m[2]->id_treno=3;
	m[3]->id_treno=4;

	// creazione dei 4 threads capitreno
	for(i=0;i<4;i++){
		pthread_create(&capo[i],NULL,Capotreno, (void*)  m[i]);
	}

	// creazione dei 10 threads viaggiatore
	for(i=0;i<10;i++){
		pthread_create(&viagg[i],NULL,Viaggiatori, (void*) m[rand() %4]);
	}


	// join dei threads capitreno
	for(i=0;i<4;i++){
		pthread_join(capo[i],NULL);
		printf("Capotreno %d terminato\n",i+1);
	}

	// join dei threads viaggiatori
	for(i=0;i<10;i++){
		pthread_join(viagg[i],NULL);
		printf("Viaggiatore  %d terminato\n",i+1);
	}

	// rimozione
	for(i=0;i<4;i++){
		rimuovi(m[i]);
	}

	// deallocazione monitor
	for(i=0;i<4;i++){
		free(m[i]);
	}

	return 0;
}
