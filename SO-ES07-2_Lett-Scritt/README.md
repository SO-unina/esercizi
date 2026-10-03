# SO-ES7-2_Lett-Scritt

In questa lezione esercitativa viene affrontato il problema lettori-scrittori in applicazioni multiprocesso. Più lettori possono accedere contemporaneamente alla risorsa, mentre uno scrittore deve avere accesso esclusivo rispetto a tutti gli altri lettori e scrittori.

La risorsa, i contatori e i semafori POSIX process-shared sono collocati in una shared memory POSIX.

## Politiche considerate

- **Priorità ai lettori:** il primo lettore blocca gli scrittori e l'ultimo lettore libera la risorsa. Nuovi lettori possono entrare anche quando uno scrittore è in attesa; un flusso continuo di lettori può quindi provocare starvation degli scrittori.
- **Accesso equo tramite tornello:** lettori e scrittori attraversano un semaforo comune. Uno scrittore che acquisisce il tornello impedisce l'ingresso di nuovi lettori, limitando la possibilità di starvation.

## Sommario degli esempi

- [1_lett_scrit_starv_scrittori](1_lett_scrit_starv_scrittori): soluzione con priorità ai lettori e possibile starvation degli scrittori;
- [2_lett_scrit_starv_entrambi](2_lett_scrit_starv_entrambi): soluzione con tornello e accesso più equo;
- [3_esercizi/coppia_di_valori_condivisa](3_esercizi/coppia_di_valori_condivisa): verifica di un invariante su una coppia di valori aggiornata dagli scrittori.

## Ciclo di vita

Il processo padre crea la shared memory, inizializza i semafori con `pshared=1`, genera lettori e scrittori e ne attende la terminazione. `sem_destroy` viene invocata soltanto quando nessun processo può più accedere alla struttura.
