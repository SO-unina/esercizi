# Server per esecuzione di comandi in remoto

Si realizzi in linguaggio C/C++ la seguente applicazione
**multi-processo** basata su **code di messaggi UNIX**. Un processo
**Server** deve mettersi in attesa di ricevere in ingresso dei messaggi
da una apposita coda. Ciascun messaggio conterrà un **comando** che il
processo Server dovrà interpretare ed eseguire in maniera simile ad una
**shell di sistema**. Il comando dovrà essere contenuto in un messaggio
sotto forma di stringa (di dimensione massima a piacere), ponendo nella
stringa il nome di un programma da eseguire ed uno o più parametri
separati da spazi (ad esempio, `ls /home/`), secondo la convenzione
UNIX. Inoltre, il messaggio dovrà contenere il PID del processo da cui
proviene la richiesta.

Il processo Server, una volta ricevuto un comando, dovrà estrapolare il
nome del programma e i parametri usando la primitiva `strtok`, e creare
un nuovo processo che esegua quel comando tramite `fork` ed `exec`. Al
termine della esecuzione del comando, il processo Server dovrà
determinare tramite `wait` lo stato di terminazione del processo figlio.
Inoltre, dovrà inviare un messaggio di **risposta** al processo che ha
richiesto il comando (utilizzando il PID ricevuto nella richiesta),
includendo nella risposta lo stato di terminazione del comando sotto
forma di intero.

Si introducano inoltre 3 processi **Client**, il cui codice sia
implementato in un **eseguibile diverso da quello del Server**. I Client
dovranno inviare un comando alla volta: dopo aver inviato un comando, il
Client dovrà attendere la risposta del Server, stampare in output il
codice di terminazione del comando, e procedere con il comando
successivo. Ogni Client invii i seguenti comandi:

    mkdir prova
    rm prova/test
    touch prova/test
    ls
    cp prova/test prova/test2

![image](/images/ambiente_locale/code_messaggi/server_per_esecuzione_di_comandi_in_remoto.png)
