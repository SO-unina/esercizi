#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(ProdCons * p) {

    init_monitor( &(p->m), 2);

    p->testa = 0;
    p->coda = 0;
    p->totale_elementi = 0;
}

void consuma(ProdCons * p, int* val_1, int* val_2) {

    enter_monitor( &(p->m) );


    printf("[%d] Ingresso consumatore\n", getpid());

    if( p->totale_elementi < 2 ) {

        wait_condition( &(p->m), MESS_DISP );
    }

    
    *val_1 = p->vettore[p->coda];
    p->coda = (p->coda + 1) % DIM;
    p->totale_elementi--;

    printf("[%d] Prima consumazione: val_1=%d\n", getpid(), *val_1);

    signal_condition( &(p->m), SPAZIO_DISP );


    *val_2 = p->vettore[p->coda];
    p->coda = (p->coda + 1) % DIM;
    p->totale_elementi--;

    printf("[%d] Seconda consumazione: val_2=%d\n", getpid(), *val_2);

    signal_condition( &(p->m), SPAZIO_DISP );



    printf("[%d] Uscita consumatore\n", getpid());

    leave_monitor( &(p->m) );
}

void produci(ProdCons * p, int val) {

    enter_monitor( &(p->m) );

    printf("[%d] Ingresso produttore\n", getpid());

    if( p->totale_elementi == DIM ) {

        wait_condition( &(p->m), SPAZIO_DISP );
    }


    p->vettore[p->testa] = val;
    p->testa = (p->testa + 1) % DIM;
    p->totale_elementi++;

    printf("[%d] Produzione: val=%d\n", getpid(), val);

    if( p->totale_elementi >= 2 ) {

        /* NOTA: è importante sbloccare un consumatore *solo*
         *       se vi sono almeno 2 elementi, poiché stiamo
         *       utilizzando un monitor di Hoare
         */

        signal_condition( &(p->m), MESS_DISP );
    }

    printf("[%d] Uscita produttore\n", getpid());

    leave_monitor( &(p->m) );
}

void rimuovi(ProdCons * p) {

    remove_monitor( &(p->m) );
}
