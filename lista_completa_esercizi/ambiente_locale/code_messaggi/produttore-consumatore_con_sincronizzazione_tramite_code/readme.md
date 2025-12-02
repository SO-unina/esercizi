# Produttore/consumatore con sincronizzazione tramite code

Si realizzi in linguaggio C/C++ una applicazione **multi-processo**
basato su **code di messaggi UNIX e shared memory UNIX**, secondo lo
schema produttore-consumatore descritto di seguito.

Si creino due processi, rispettivamente **Produttore** e
**Consumatore**. Il Consumatore invia **richieste di produzione** usando
una prima coda di messaggi (**coda produzioni**), e si mette in attesa
di ricevere messaggi da una seconda coda (**coda consumazioni**).
Viceversa, il Produttore riceve richieste dalla prima coda, ed invia
**richieste di consumazione** sulla seconda coda.

Inizialmente, il Consumatore invia una richiesta di produzione, mentre
il Produttore è in attesa sulla coda produzioni. All'arrivo della una
richiesta di produzione, il Produttore preleva il messaggio, attende un
secondo, e produce all'interno della memoria condivisa un valore (un
intero scelto casualmente). Infine, il Produttore invia al Consumatore
una richiesta di consumazione sulla coda consumazioni. All'arrivo di una
richiesta di consumazione, il Consumatore preleva il messaggio, attende
un secondo, e stampa il valore nella memoria condivisa.

Il Consumatore e il Produttore dovranno iterare questo schema per 5
volte. Il codice del Produttore e del Consumatore dovrà risiedere in
**due eseguibili distinti**. Si crei inoltre un terzo eseguibile che
avvii gli altri due.

![image](/images/ambiente_locale/code_messaggi/produttore-consumatore_con_sincronizzazione_tramite_code.png)
