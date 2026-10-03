# Corso di Sistemi Operativi <img src="https://github.com/SO-unina/esercitazioni/blob/main/images/SO-unina_logo.png" width="100">

> **_N.B.:_** Prima di consultare le esercitazioni, leggere la [guida breve](git) per poter utilizzare il comando ``git``

Sommario delle esercitazioni:

- [**SO-ES01-Introduzione-Linux**](SO-ES01-Introduzione-Linux): Installazione VM Linux e comandi shell

- [**SO-ES03-Makefile-Librerie-GDB**](SO-ES03-Makefile-Librerie-GDB): Utilizzo di Makefile e librerie.

- [**SO-ES04-Syscall-fork-exec**](SO-ES04-Syscall-fork-exec): Utilizzo di system call per la gestione dei processi.

- [**SO-ES05-Shmem**](SO-ES05-Shmem): Utilizzo della shared memory POSIX per la comunicazione inter-processo.

- [**SO-ES06-Semafori**](SO-ES06-Semafori): Utilizzo dei semafori POSIX per la sincronizzazione inter-processo.

- [**SO-ES07-1_Prod-Cons**](SO-ES07-1_Prod-Cons): Utilizzo di semafori POSIX e shared memory POSIX per la soluzione al problema Produttori-Consumatori.

- [**SO-ES07-2_Lett-Scritt**](SO-ES07-2_Lett-Scritt): Utilizzo di semafori POSIX e shared memory POSIX per la soluzione al problema Lettori-Scrittori.

- [**SO-ES08-Code-di-messaggi**](SO-ES08-Code-di-messaggi): Utilizzo delle code di messaggi POSIX per la comunicazione ad ambiente locale.

- [**SO-ES09-Pthreads**](SO-ES09-Pthreads): Utilizzo della libreria ``pthread``. Esercizi su problemi produttori-consumatori e lettori-scrittori risolti tramite monitor pthread (mutex e condition variables).

- [**SO-ES10-SviluppoLinuxKernel**](SO-ES10-SviluppoLinuxKernel): Introduzione allo sviluppo nel kernel Linux.

- [**lista_completa_esercizi**](lista_completa_esercizi): Raccolta completa di esercizi con traccia e soluzione, suddivisi per ambiente globale (shared memory e semafori, monitor pthread) e ambiente locale (code di messaggi, server multithread).

## Convenzioni adottate per le risorse IPC POSIX

Le esercitazioni utilizzano le seguenti famiglie di primitive:

- shared memory POSIX: `shm_open`, `ftruncate`, `mmap`, `munmap`, `close`, `shm_unlink`;
- semafori POSIX: `sem_init`, `sem_wait`, `sem_post`, `sem_destroy` (semafori anonimi process-shared) oppure `sem_open`, `sem_close`, `sem_unlink` (semafori nominati);
- code di messaggi POSIX: `mq_open`, `mq_send`, `mq_receive`, `mq_close`, `mq_unlink`.

I nomi delle risorse POSIX iniziano con `/` e sono visibili come file (`/dev/shm` per shared memory e semafori nominati, `/dev/mqueue` per le code di messaggi). Il processo che crea una risorsa nominata è anche responsabile della sua rimozione con `*_unlink`: la chiusura di un descrittore o la terminazione di un processo non rimuovono automaticamente il nome della risorsa. Lo script [remove_all_posix_ipcs.sh](remove_all_posix_ipcs.sh) rimuove le risorse POSIX residue dell'utente corrente.

Per compilare, linkare le librerie real-time e pthread quando richiesto: `-lrt` (code di messaggi e, su glibc datate, shared memory) e `-lpthread` (semafori).

> **_N.B.:_** Vuoi usare VIM come editor testuale? Utilizza questo [cheatsheet](images/vim_cheatsheet.png) per una guida veloce sui comandi più comuni di VIM e gioca a [vim adventures](https://vim-adventures.com/) per allenarti ad usare VIM. Successivamente, esplora tutte le potenzialità di VIM utilizzando questa serie di [plugin](https://github.com/amix/vimrc)
