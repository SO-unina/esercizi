# SO-ES8-Code-di-messaggi

In questa lezione esercitativa viene affrontato il problema della comunicazione ad ambiente locale tramite **code di messaggi POSIX** in applicazioni **multiprocesso**. Si vedrà come risolvere tale problema grazie all'utilizzo delle primitive `mq_open`, `mq_send`, `mq_receive`, `mq_close` e `mq_unlink`.

Sommario degli esempi:

- [**1_intro_code_di_messaggi**](1_intro_code_di_messaggi): Introduzione sulle code di messaggi POSIX in Linux;
- [**2_comunicazione_async**](2_comunicazione_async): Utilizzo delle code di messaggi per una comunicazione tramite send asincrona;
- [**3_comunicazione_sync**](3_comunicazione_sync): Utilizzo delle code di messaggi per una comunicazione tramite send sincrona (rendezvous su code di servizio);
- [**4_comunicazione_multi_eseguibile**](4_comunicazione_multi_eseguibile): Esercizio sull'utilizzo delle code di messaggi con applicazioni su più eseguibili;
- [**5_esercitazione_chat**](5_esercitazione_chat): Esercitazione sull'uso delle code di messaggi per implementare una *chat*;
- [**6_messaggi_priorita**](6_messaggi_priorita): Utilizzo delle priorità dei messaggi, una caratteristica specifica delle code POSIX.

> **Nota:** le code POSIX sono visibili come file in `/dev/mqueue`. Per compilare è necessario linkare la libreria real-time con `-lrt`.
