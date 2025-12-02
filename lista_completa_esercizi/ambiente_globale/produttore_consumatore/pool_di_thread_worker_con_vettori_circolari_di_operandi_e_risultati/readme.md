# Pool di thread worker, con vettori circolari di operandi e risultati

Si realizzi in linguaggio C/C++ la seguente applicazione
**multithread**, basata su monitor e sullo schema
**produttore-consumatore**. L'applicazione dovrà creare un gruppo di
thread worker (il gruppo è detto "**thread pool**"), che dovranno porsi
in attesa di ricevere degli operandi da elaborare. Inoltre,
l'applicazione dovrà creare un gruppo di thread client che generano
operandi da fare elaborare ai worker, e prelevano i risultati dei
worker.

Gli operandi (una coppia di valori interi) dovranno essere scambiati
mediante un vettore di buffer, in base allo schema
produttore-consumatore. I thread worker consumano una coppia di operandi
dal vettore; li elabora, calcolando la somma della coppia; e deposita il
risultato prodotto in un secondo vettore di buffer. Si crei un gruppo di
tre thread worker.

I thread client dovranno essere due. Il primo thread client inserisce
operandi nel primo vettore di buffer, generando coppie di valori
casuali. Il secondo thread client dovrà prelevare i risultati dal
secondo vettore di buffer e stamparli a video. Si simuli la generazione
di 6 coppie di operandi. Il secondo thread client, una volta consumati 6
risultati, dovrà forzare la terminazione dell'intero processo mediante
la primitiva `exit()`.

Si utilizzino le seguenti strutture dati. Si implementino gli opportuni
metodi del monitor per produrre e consumare elementi, per ognuno dei due
monitor.

    struct operandi { int operando1; int operando2 };

    struct MonitorOperandi {
        operandi vettore_operandi[4];
        // … inserire qui ulteriori variabili di sincronizzazione …
    };

    struct MonitorRisultati {
        int vettore_risultati[4];
        // … inserire qui ulteriori variabili di sincronizzazione …
    };

    void ProduciOperandi(struct MonitorOperandi * mop, struct operandi op);
    struct operandi ConsumaOperandi(struct MonitorOperandi * mop);
    void ProduciRisultato(struct MonitorRisultati * mris, int risultato);
    int ConsumaRisultato(struct MonitorRisultati * mris);

![image](/images/ambiente_globale/produttore_consumatore/pool_di_thread_worker_con_vettori_circolari_di_operandi_e_risultati.png)
