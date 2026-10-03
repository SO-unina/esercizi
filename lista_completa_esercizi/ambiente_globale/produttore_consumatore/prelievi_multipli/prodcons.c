#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(ProdCons * p) {

    init_monitor( &(p->m), 2);

    p->testa = 0;
    p->coda = 0;
    p->totale_elementi = 0;
}

void consuma(ProdCons * p, int id, int* val_1, int* val_2) {

    enter_monitor( &(p->m) );


    printf("[Consumatore %d] Ingresso consumatore\n", id);

    /* NOTA: con la semantica signal-and-continue la condizione logica
     *       va ricontrollata in un ciclo while: al risveglio, il thread
     *       rientra nel monitor solo quando il segnalante rilascia il
     *       mutex, e nel frattempo la condizione potrebbe non essere
     *       più vera (nel monitor di Hoare bastava un semplice if)
     */

    while( p->totale_elementi < 2 ) {

        wait_condition( &(p->m), MESS_DISP );
    }


    *val_1 = p->vettore[p->coda];
    p->coda = (p->coda + 1) % DIM;
    p->totale_elementi--;

    printf("[Consumatore %d] Prima consumazione: val_1=%d\n", id, *val_1);

    signal_condition( &(p->m), SPAZIO_DISP );


    *val_2 = p->vettore[p->coda];
    p->coda = (p->coda + 1) % DIM;
    p->totale_elementi--;

    printf("[Consumatore %d] Seconda consumazione: val_2=%d\n", id, *val_2);

    signal_condition( &(p->m), SPAZIO_DISP );



    printf("[Consumatore %d] Uscita consumatore\n", id);

    leave_monitor( &(p->m) );
}

void produci(ProdCons * p, int id, int val) {

    enter_monitor( &(p->m) );

    printf("[Produttore %d] Ingresso produttore\n", id);

    while( p->totale_elementi == DIM ) {

        wait_condition( &(p->m), SPAZIO_DISP );
    }


    p->vettore[p->testa] = val;
    p->testa = (p->testa + 1) % DIM;
    p->totale_elementi++;

    printf("[Produttore %d] Produzione: val=%d\n", id, val);

    if( p->totale_elementi >= 2 ) {

        /* NOTA: con il monitor di Hoare era *necessario* sbloccare un
         *       consumatore solo se vi erano almeno 2 elementi. Con la
         *       semantica signal-and-continue il consumatore ricontrolla
         *       comunque la condizione nel ciclo while, quindi questo
         *       controllo serve solo ad evitare risvegli inutili.
         */

        signal_condition( &(p->m), MESS_DISP );
    }

    printf("[Produttore %d] Uscita produttore\n", id);

    leave_monitor( &(p->m) );
}

void rimuovi(ProdCons * p) {

    remove_monitor( &(p->m) );
}
