## Identificativo di un processo: ``getpid()`` e ``getppid()``

Ogni processo ha un unico identificatore di processo chiamato PID (Process Identifier). Il PID è un intero positivo, minore di ``pid_max``: il valore di default storico è ``32768``, ma sui sistemi a 64 bit può arrivare a ``4194304`` (2^22), e le distribuzioni recenti (con ``systemd``) lo impostano proprio a questo valore. È possibile leggerlo dal file di sistema ``/proc/sys/kernel/pid_max``. E.g.:

```
$ cat /proc/sys/kernel/pid_max
4194304
```

Il PID di un processo viene assegnato dal kernel all’atto della sua creazione. E' possibile ottenere il PID di un processo attraverso la chiamata di sistema ``getpid()``. Inoltre, i processi in Unix sono organizzati gerarchicamente, ovvero, ogni processo ha un processo padre (eccetto il processo ``init``). Nel contesto di esecuzione del processo corrente, è possibile ottenere il PID del processo padre attraverso la chiamata di sistema ``getppid()``.

```c
pid_t getpid(void);
pid_t getppid(void);
```

Il seguente codice stamperà a video il PID del processo corrente e il PID del suo processo padre:

```c
#include <stdio.h>
#include <unistd.h>

int main(void)
{
 	int pid, ppid;
 	pid = getpid();
 	printf("Sono il processo pid = %d\n", pid);
 
	ppid = getppid();
 	printf("Il mio processo genitore ha pid = %d\n", ppid);

 	return 0;
}
```

Provando a compilare ed eseguire tale codice (osservare i file [``Makefile``](Makefile) e [``main.c``](main.c)) otteniamo un output del genere:

```console
$ ./getpid_ex
Sono il processo pid = 19375
Il mio processo genitore ha pid = 14511
```

In Linux, alcuni PID sono riservati a processi specifici:

- PID = 0: il processo *idle* (detto anche ``swapper``). Non è un programma su disco, ma codice del kernel: esegue quando la CPU non ha nient'altro da fare e non compare nell'output di ``ps``;
- PID = 1: il processo ``init``, il primo processo in spazio utente, creato dal kernel all'avvio: è l'antenato di tutti gli altri. Quale programma sia dipende dalla distribuzione: nella maggior parte (Debian, Ubuntu, Fedora, Arch...) è ``systemd``, ma Alpine e Gentoo usano OpenRC, Void usa runit e Devuan ancora l'``init`` di System V;
- PID = 2: il processo ``kthreadd``, anche lui parte del kernel, che crea i *thread del kernel* (quelli che ``ps`` mostra tra parentesi quadre). Tra questi c'è ``kswapd``, che sposta su disco le pagine di memoria quando la RAM scarseggia (è l'equivalente del ``pagedaemon`` dei vecchi sistemi Unix).

Per poter analizzare i PID associati ai processi attualmente sul sistema, possiamo utilizzare ancora il comando ``ps`` (vedi [lezione sui comandi shell](../../SO-ES01-Introduzione-Linux/2_comandi_shell#comandi-di-utilità-per-i-processi-ps-e-top))

Oltre al comando ``ps``, è possibile analizzare la directory ``/proc`` che, per ogni processo nel sistema, contiene una directory denominata proprio con il PID del processo, e.g.:

```console 
$ ls /proc/
1     119   1397  199   2257  259  388  751  92           kpagecgroup
10    12    14    2     2258  260  391  76   93           kpagecount
100   120   1414  20    226   261  398  766  94           kpageflags
101   1203  1433  200   2261  262  4    77   95           loadavg
102   1207  15    201   227   263  450  78   950          locks
103   121   150   202   228   264  5    788  952          mdstat
104   122   1539  203   229   265  560  789  96           meminfo
105   1225  16    204   23    266  572  79   961          misc
106   123   1647  205   230   267  575  793  968          modules
1062  124   17    206   231   268  6    8    97           mounts
107   1245  18    207   232   269  611  801  98           mtrr
108   125   189   208   233   270  612  808  99           net
1082  1251  1894  209   234   271  613  809  993          pagetypeinfo
1089  1275  19    21    235   272  615  81   acpi         partitions
109   1278  190   210   236   273  618  815  buddyinfo    pressure
1095  1279  1904  211   237   274  622  82   bus          sched_debug
11    1280  192   212   238   275  623  84   cgroups      schedstat
110   1281  1921  2127  239   276  635  848  cmdline      scsi
1101  1283  1922  213   24    277  638  85   consoles     self
111   1288  1923  2137  240   278  646  850  cpuinfo      slabinfo
1116  1289  1924  214   241   279  650  852  crypto       softirqs
112   1290  1925  215   242   299  657  857  devices      stat
113   1291  1926  2156  243   3    661  86   diskstats    swaps
1136  1294  1927  216   244   300  662  863  dma          sys
114   1297  1928  217   245   339  671  866  driver       sysrq-trigger
1140  13    1929  218   246   362  675  869  execdomains  sysvipc
1141  1300  193   219   247   363  7    87   fb           thread-self
1143  1301  1930  2191  248   368  70   870  filesystems  timer_list
1145  1307  1931  2192  249   370  703  877  fs           tty
115   1308  1932  22    250   371  709  88   interrupts   uptime
1159  1309  1933  220   251   374  71   881  iomem        version
116   134   1934  2206  252   375  72   89   ioports      version_signature
1164  1354  1935  221   253   377  720  9    irq          vmallocinfo
1168  137   194   222   254   379  724  90   kallsyms     vmstat
117   1380  195   223   255   381  73   91   kcore        zoneinfo
1175  1383  196   224   256   383  734  910  keys
118   1385  197   225   257   386  74   915  key-users
1185  1387  198   2253  258   387  75   919  kmsg
```

Sotto la directory ``/proc`` è _montato_ il cosiddetto ``procfs``, un file system virtuale creato _on-the-fly_ all'avvio del sistema. La directory ``/proc`` contiene tutta una serie di informazioni utili che riguardano i processi che sono attualmente in esecuzione nel sistema. Ad esempio, il file ``cmdline`` contiene la riga di comando con cui è stato avviato il processo (gli argomenti sono separati dal carattere nullo, che ``tr`` trasforma in spazi):

```
$ tr '\0' ' ' < /proc/1/cmdline
/sbin/init splash
```

Sulle distribuzioni che usano ``systemd``, ``/sbin/init`` è solo un collegamento simbolico a ``systemd`` (si veda l'esperimento 3).


## Esempi da provare

### 1. Chi è mio padre?

```console
$ make
gcc -o getpid_ex main.c
$ ./getpid_ex
Sono il processo pid = 215071
Il mio processo genitore ha pid = 215064
$ ./getpid_ex
Sono il processo pid = 215072
Il mio processo genitore ha pid = 215064
```

Il PID cambia a ogni esecuzione, perché ogni volta nasce un nuovo processo. Il PID del padre, invece, resta lo stesso.

> **_Domanda:_** chi è il processo 215064?

```console
$ echo $$
215064
```

La variabile speciale ``$$`` della shell contiene il PID della shell stessa: il padre di ``getpid_ex`` è ``bash``, che lo ha creato quando abbiamo premuto Invio.

### 2. L'albero dei processi

```console
$ ps -o pid,ppid,stat,comm
    PID    PPID STAT COMMAND
 215064  215061 Ss   bash
 215073  215064 R+   ps
```

Senza opzioni di selezione ``ps`` mostra i processi del terminale corrente; con ``-o`` scegliamo le colonne. Anche ``ps`` è un figlio di ``bash``. Nella colonna ``STAT`` la prima lettera è lo stato: ``R`` in esecuzione (o pronto), ``S`` in attesa di un evento (le altre lettere, come ``s`` e ``+``, qui non ci interessano).

Con ``pstree`` risaliamo la catena dei padri, dalla shell fino a ``init``:

```console
$ pstree -p -s $$
systemd(1)───systemd(1845)───gnome-terminal-(3456)───bash(215064)───pstree(215090)
```

La catena dipende dall'ambiente grafico (quella sopra è di un desktop GNOME), ma la radice è sempre ``systemd``, con PID 1. In un terminale grafico non ci sono né ``getty`` né ``login``: l'accesso lo ha gestito il *display manager*, cioè la schermata di login grafica.

### 3. I processi speciali

```console
$ ps -o pid,ppid,comm -p 1,2
    PID    PPID COMMAND
      1       0 systemd
      2       0 kthreadd
```

``systemd`` e ``kthreadd`` hanno padre 0: li ha creati direttamente il kernel. I PID 0 e 2 appartengono al kernel Linux, e sono uguali in tutte le distribuzioni; il programma che fa da ``init`` invece dipende dalla distribuzione.

> **_Domanda:_** con ``ps aux`` il processo 1 non si chiama ``systemd``. Perché?

```console
$ ps aux | head -2
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root           1  0.0  0.0  23084 14048 ?        Ss   10:39   0:01 /sbin/init splash
$ ps -p 1 -o pid,comm,args
    PID COMMAND         COMMAND
      1 systemd         /sbin/init splash
$ ls -l /sbin/init
lrwxrwxrwx 1 root root 22 Jul 28 17:04 /sbin/init -> ../lib/systemd/systemd
$ ls /etc/inittab
ls: cannot access '/etc/inittab': No such file or directory
```

- ``ps aux`` mostra la **riga di comando** con cui il processo è stato avviato (la colonna ``args``): per convenzione il kernel avvia ``/sbin/init``, e gli passa così com'è il parametro di avvio ``splash`` (su altre distribuzioni la riga può essere diversa).
- La colonna ``comm`` mostra invece il nome del **programma in esecuzione**: ``systemd``. ``/sbin/init`` infatti è solo un collegamento simbolico a ``systemd``.
- ``/etc/inittab``, il file di configurazione del vecchio ``init`` di System V, non esiste più.

```console
$ ps -o pid,ppid,comm -C kswapd0
    PID    PPID COMMAND
    142       2 kswapd0
```

``kswapd0`` è un thread del kernel, figlio di ``kthreadd`` (PID 2).

### 4. Dentro ``/proc``

```console
$ grep -E '^(Name|State|Pid|PPid)' /proc/$$/status
Name:	bash
State:	S (sleeping)
Pid:	215064
PPid:	215061
```

Il file ``status`` riassume le informazioni che il kernel tiene su un processo: nome, stato, PID, PID del padre (e molto altro, provate con ``cat``).

> **_Domanda:_** e ``/proc/self``? Di quale processo sono queste informazioni?

```console
$ grep -E '^(Name|Pid|PPid)' /proc/self/status
Name:	grep
Pid:	215086
PPid:	215064
```

``/proc/self`` indica sempre il processo che la sta leggendo: qui è ``grep``, figlio di ``bash``.

### 5. Un processo che dorme

```console
$ sleep 100 &
[1] 215088
$ ps -o pid,ppid,stat,wchan:20,comm
    PID    PPID STAT WCHAN                COMMAND
 215064  215061 Ss   do_wait              bash
 215088  215064 S    hrtimer_nanosleep    sleep
 215089  215064 R+   -                    ps
$ kill %1
```

- Con ``&`` il comando parte in *background*: la shell non lo aspetta e ci restituisce subito il prompt.
- ``sleep`` è nello stato ``S`` (*sleeping*), cioè **bloccato**: non usa la CPU finché il tempo non è scaduto. La colonna ``WCHAN`` mostra la funzione del kernel in cui il processo è fermo: ``hrtimer_nanosleep``, la system call che sta dietro la funzione ``sleep()``.
- Anche ``bash`` è bloccata, in ``do_wait``: sta aspettando che termini il figlio ``ps``. È la system call ``wait()``, che vedremo tra poco.
- ``kill %1`` termina il *job* numero 1, cioè ``sleep``.

### 6. La system call dietro la funzione

``strace`` mostra le system call eseguite da un programma (se non è installato: ``sudo apt install strace``); con ``-e trace=`` scegliamo quali.

```console
$ strace -e trace=getpid,getppid ./getpid_ex
getpid()                                = 63832
Sono il processo pid = 63832
getppid()                               = 63829
Il mio processo genitore ha pid = 63829
+++ exited with 0 +++
```

Le funzioni ``getpid()`` e ``getppid()`` della libreria C sono dei *wrapper*: ognuna esegue la system call che ha lo stesso nome e ne restituisce il risultato. È il caso più semplice; vedremo in [``3_exec``](../3_exec) che spesso il nome della funzione e quello della system call **non** coincidono.

E ``printf()``? Non è una system call: è una funzione di libreria che prepara il testo e poi, per scriverlo, esegue la system call ``write()``.

> **_Domanda:_** il programma chiama due volte ``printf()``. Quante ``write()`` ci aspettiamo?

```console
$ strace -e trace=getpid,getppid,write ./getpid_ex > /dev/null
getpid()                                = 63840
getppid()                               = 63837
write(1, "Sono il processo pid = 63840\nIl "..., 69) = 69
+++ exited with 0 +++
```

Una sola, alla fine, con tutte e due le righe. ``printf()`` accumula il testo in un *buffer* nella memoria del processo e chiama ``write()`` il meno possibile, perché ogni system call costa. Qui l'output è rediretto su un file (``/dev/null``), quindi il buffer viene svuotato solo alla fine del programma; quando l'output è il terminale viene svuotato a ogni ``\n``, e le ``write()`` sono due (provate senza ``> /dev/null``). Questo buffer tornerà fuori negli esperimenti "per casa" di [``2_fork``](../2_fork) e [``3_exec``](../3_exec).
