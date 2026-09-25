// libmat versione 2: bug corretto
#include "mat.h"

const char *mat_versione(void){
	return "v2";
}

int somma(int* s,int d){
	int temp = 0;
	int i;
	for(i=0; i < d; i++){		// corretto: si parte da 0
		temp += s[i];
	}
	return temp;
}

int prod(int* s,int d){
	int temp = 1;
	int i;
	for(i=0; i < d; i++){
		temp *= s[i];
	}
	return temp;
}
