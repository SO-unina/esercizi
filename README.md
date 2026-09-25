# Corso di Sistemi Operativi <img src="https://github.com/SO-unina/esercitazioni/blob/main/images/SO-unina_logo.png" width="100"> 

> **_ATTENZIONE:_** questo branch (`systemv`) contiene la versione **archiviata** delle esercitazioni, che per la comunicazione e la sincronizzazione tra processi (IPC) usano le API **System V IPC** (`shmget`, `semget`, `msgget`, ...). La versione aggiornata, basata sulle API **POSIX**, è nel branch [main](https://github.com/SO-unina/esercizi/tree/main).

> **_N.B.:_** Prima di consultare le esercitazioni, leggere la [guida breve](git) per poter utilizzare il comando ``git``

Sommario delle esercitazioni:

- [**SO-ES01-Introduzione-Linux**](SO-ES01-Introduzione-Linux): Installazione VM Linux e comandi shell

- [**SO-ES03-Makefile-Librerie-GDB**](SO-ES03-Makefile-Librerie-GDB): Utilizzo di Makefile e librerie.

- [**SO-ES04-Syscall-fork-exec**](SO-ES04-Syscall-fork-exec): Utilizzo di system call per la gestione dei processi.

- [**SO-ES05-Shmem**](SO-ES05-Shmem): Utilizzo di shared memory per la comunicazione inter-processo.

- [**SO-ES06-Semafori**](SO-ES06-Semafori): Utilizzo di semafori per la sincronizzazione inter-processo.

- [**SO-ES07-1_Prod-Cons**](SO-ES07-1_Prod-Cons): Utilizzo di semafori e shared memory per la soluzione al problema Produttori-Consumatori.

- [**SO-ES07-2_Lett-Scritt**](SO-ES07-2_Lett-Scritt): Utilizzo di semafori e shared memory per la soluzione al problema Lettori-Scrittori.

- [**SO-ES08-Code-di-messaggi**](SO-ES08-Code-di-messaggi): Utilizzo di code di messaggi per la comunicazione ad ambiente locale.

- [**SO-ES09-Monitor**](SO-ES09-Monitor): Utilizzo del costrutto monitor implementato con semafori e shared memory. Utilizzo nei problemi produttori-consumatori e lettori-scrittori.

- [**SO-ES10-Pthreads**](SO-ES10-Pthreads): Utilizzo della libreria ``pthread``. Esercizi su problemi produttori-consumatori e lettori-scrittori risolti tramite monitor pthread.



> **_N.B.:_** Vuoi usare VIM come editor testuale? Utilizza questo [cheatsheet](images/vim_cheatsheet.png) per una guida veloce sui comandi più comuni di VIM e gioca a [vim adventures](https://vim-adventures.com/) per allenarti ad usare VIM. Successivamente, esplora tutte le potenzialità di VIM utilizzando questa serie di [plugin](https://github.com/amix/vimrc)
