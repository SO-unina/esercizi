// libmat versione 1: contiene un bug!
#include "mat.h"

const char *mat_versione(void){
	return "v1";
}

int somma(int* s,int d){
	int temp = 0;
	int i;
	for(i=1; i < d; i++){		// BUG: salta il primo elemento
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
