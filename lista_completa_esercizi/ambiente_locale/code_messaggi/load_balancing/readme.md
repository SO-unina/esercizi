# Load balancing

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** basata
su **code di messaggi UNIX**, per la simulazione di un servizio di
**Load Balancing**. Un processo, denominato **Balancer**, attende di
ricevere messaggi di richiesta di un ipotetico servizio, provenienti da
un gruppo di processi **Client**. Il **Balancer** inoltra a turno le
richieste a 3 processi **Server** diversi, con un criterio a rotazione:
la prima richiesta al **Server** 1, la seconda al **Server** 2, la terza
al **Server** 3, la quarta di nuovo al **Server** 1, etc. In questo modo
le richieste saranno distribuite equamente tra i server, i quali,
ricevuti il messaggio, si limitano a stampare un messaggio a video di
avvenuta ricezione. I messaggi di richiesta dovranno contenere il PID
del processo **Client** che ha generato il messaggio, e ciascun
**Client** dovrà generare 6 messaggi, attendendo 1 secondo tra un
messaggio e il successivo. Un unico programma principale dovrà
istanziare 2 processi **Client** il **Balancer** e i 3 processi Server,
oltre che le code di messaggi necessari a gestire la comunicazione tra i
processi.

![image](/images/ambiente_locale/code_messaggi/load_balancing.png)
