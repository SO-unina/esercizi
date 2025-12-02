# Una ipotetica applicazione di controllo

Si realizzi in linguaggio C/C++ una applicazione **multi-processo**
basata su **code di messaggi UNIX** che simuli il controllore di una
macchina per il pane, come indicato in figura. Il processo Interfaccia
attende dall'utente tramite lo standard input (con `scanf()`)
l'indicazione di iniziare la preparazione, dopo la quale esso invia un
messaggio di `avvio preparazione` al processo **Controllore**.

Inizialmente, il processo Controllore dovrà inviare un messaggio al
processo **Mixer** di `avvio rotazione`, attendere 3 secondi, e poi
inviare un messaggio al Mixer di `interruzione rotazione`.

In seguito, il processo Controllore dovrà inviare al processo
**Termostato** un messaggio di `avvio riscaldamento`. Una volta ricevuto
questo messaggio, il processo Termostato dovrà ogni secondo incrementare
di 3 una variabile interna "temperatura" al suo interno (inizialmente
pari a 15), e inviare un messaggio al processo Controllore di
`aggiornamento temperatura` contenente il valore attuale della variabile
"temperatura". Quando la "temperatura" raggiunge un valore uguale o
maggiore a 50, il processo Controllore dovrà inviare un messaggio di
`interruzione riscaldamento` al processo Termostato. I messaggi dal
Controllore al Termostato e al Mixer dovranno essere con send
sincrona<sup>[\[1\]](#footnote1)</sup>.

Infine, è richiesto che il Controllore invii al processo Interfaccia dei
messaggi per aggiornare l'utente sul progresso delle operazioni
(inizio/fine rotazione, inizio/fine riscaldamento, valori di
temperatura). Inoltre, al termine della preparazione, il Controllore
dovrà inviare dei messaggi di terminazione agli altri 3 processi per
forzarne l'uscita, per poi terminare a sua volta. L'applicazione dovrà
essere costituita di **due eseguibili**, uno con il codice
dell'Interfaccia, ed uno con il codice degli altri processi.

![image](/images/ambiente_locale/code_messaggi/una_ipotetica_applicazione_di_controllo.png)

<a name="footnote1">[1]</a>: Nel processo Termostato, quando il riscaldamento è attivo, si
    faccia uso del parametro `IPC_NOWAIT` nell'attendere messaggi di
    `OK TO SEND`, in modo da permettere al processo di non bloccarsi
    mentre incrementa la temperatura.
