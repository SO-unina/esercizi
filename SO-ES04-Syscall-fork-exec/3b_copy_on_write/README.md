# Copy-on-write: quanto costa davvero una ``fork()``?

Secondo la definizione, la ``fork()`` copia la memoria del padre nel figlio. Se il padre occupa 256 MB, la ``fork()`` dovrebbe copiare 256 MB. Linux invece usa il **copy-on-write**: dopo la ``fork()`` padre e figlio condividono le stesse pagine fisiche, marcate in sola lettura. Quando uno dei due prova a scrivere su una pagina, la CPU genera un'eccezione (*page fault*) e il kernel copia quella pagina, e solo quella.

Il programma [``cow.c``](cow.c) lo misura:

1. il padre alloca 256 MB e li scrive tutti, così le pagine sono davvero in RAM;
2. il padre chiama ``fork()``;
3. il figlio scrive due volte sugli stessi 256 MB.

Per ogni passo stampa il tempo impiegato e il numero di *page fault* (letto con ``getrusage()``).

## Esempio

> **_Domanda:_** quale dei passi sarà il più lento? E quanti page fault ci aspettiamo nella prima scrittura del figlio?

```console
$ make
gcc -Wall    cow.c   -o cow
$ ./cow
256 MB = 65536 pagine da 4096 byte

padre: prima scrittura                   105.5 ms   65539 page fault
figlio: fork                               5.5 ms
figlio: prima scrittura                  146.1 ms   65537 page fault
figlio: seconda scrittura                  9.5 ms       0 page fault
```

- **``fork``: pochi millisecondi.** Il kernel non copia i 256 MB: crea la ``task_struct`` del figlio e copia le *tabelle delle pagine* del padre, marcando le pagine in sola lettura.
- **Prima scrittura del figlio: circa 65536 page fault**, uno per pagina. A ogni fault il kernel alloca una pagina nuova, ci copia il contenuto di quella del padre e la rende scrivibile. È qui che si paga la copia, una pagina alla volta.
- **Seconda scrittura: 0 page fault**, ed è una ventina di volte più veloce: ormai le pagine sono del figlio.
- Anche la prima scrittura del padre genera un page fault per pagina, ma per un altro motivo: ``malloc`` ha riservato gli indirizzi, e il kernel assegna le pagine fisiche solo al primo accesso.

Se il figlio avesse chiamato subito una ``exec``, come fa quasi sempre, nessuna di queste pagine sarebbe stata copiata.

> **_N.B.:_** i tempi cambiano da macchina a macchina (e in una macchina virtuale possono essere più alti), ma le proporzioni restano le stesse. Il programma usa 256 MB, più altri 256 MB per le copie del figlio: su una macchina virtuale con poca RAM potete ridurre ``DIM`` in [``cow.c``](cow.c).
