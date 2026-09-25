#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mat.h"

#define DIM 10

typedef struct arr_int{
	int* arr;
	int dim;
}arr_int_t;

void popola(arr_int_t* a){
	int i;
	srand(time(NULL));
	
	for(i = 0; i < a->dim; i++){
		a->arr[i] = (rand()% 4) + 1;
	}
}

void stampa(arr_int_t* a){
	int i;
	printf("( ");
	for(i=0;i < a->dim; i++){
		printf("%d ",a->arr[i]);
	}
	printf(" ) \n");
}

int main(int argc, char** argv){
	arr_int_t a;
	int* s = malloc(DIM*sizeof(int));
	a.arr = s;
	a.dim = DIM;
	int sum;
	int p;
	popola(&a);	
	sum = somma(a.arr,a.dim);
	p = prod(a.arr,a.dim);
	stampa(&a);
	printf("Somma = %d\n",sum);
	printf("Prod = %d\n",p);
	free(s);
	return 0;
}

