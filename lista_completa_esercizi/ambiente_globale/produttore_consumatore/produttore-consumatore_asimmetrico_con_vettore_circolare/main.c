#include "header.h"

#define THREAD 6

int main(){
	
	pthread_t threads[THREAD];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	GestioneIO * g = (GestioneIO*)malloc(sizeof(GestioneIO));
	Inizializza(g);	
	int i;
	srand(time(NULL));
	for(i=0;i<THREAD;i++){
		if(i<4){
			//PRODUTTORE
			pthread_create(&threads[i],&attr,Produttore,(void*)g);
		}
		if(i>=4){
			//CONSUMATORE
			pthread_create(&threads[i],&attr,Consumatore,(void*)g);
		}
	}

	for(i=0;i<THREAD;i++){
		pthread_join(threads[i],NULL);
	}

	Rimuovi(g);
	free(g);
	return 0;
}
