Esercizi di Programmazione Concorrente in Linux
===============================================

Corso di Laurea Triennale in Ingegneria Informatica

Università degli Studi di Napoli Federico II

> **_N.B.:_** Gli esercizi sulla comunicazione e sincronizzazione tra processi (IPC) basati sulle API **POSIX** saranno pubblicati a breve. La versione precedente, basata sulle API **System V IPC**, è disponibile nel branch [systemv](https://github.com/SO-unina/esercizi/tree/systemv/lista_completa_esercizi).

# Indice

## Modello ad ambiente globale, produttore-consumatore e mutua esclusione

* [Una struttura dati stack thread-safe](ambiente_globale/produttore_consumatore/una_struttura_dati_stack_thread-safe)<sup><b>[con soluzione]</b></sup>
* [Produzione di un vettore variabile di elementi](ambiente_globale/produttore_consumatore/produzione_di_un_vettore_variabile_di_elementi)
* [Pool di thread worker, con vettore circolare](ambiente_globale/produttore_consumatore/pool_di_thread_worker_con_vettore_circolare)<sup><b>[con soluzione]</b></sup>
* [Pool di thread worker, con vettore di stato](ambiente_globale/produttore_consumatore/pool_di_thread_worker_con_vettore_di_stato)<sup><b>[con soluzione]</b></sup>
* [Pool di thread worker, con vettori circolari di operandi e risultati](ambiente_globale/produttore_consumatore/pool_di_thread_worker_con_vettori_circolari_di_operandi_e_risultati)
* [Allocatore di memoria](ambiente_globale/produttore_consumatore/allocatore_di_memoria)
* [Produttore/consumatore asimmetrico, con vettore di stato](ambiente_globale/produttore_consumatore/produttore-consumatore_asimmetrico_con_vettore_di_stato)<sup><b>[con soluzione]</b></sup>
* [Produttore/consumatore asimmetrico, con vettore circolare](ambiente_globale/produttore_consumatore/produttore-consumatore_asimmetrico_con_vettore_circolare)<sup><b>[con soluzione]</b></sup>
* [Produttore/consumatore con vettore di stato, e diverse elaborazioni](ambiente_globale/produttore_consumatore/produttore-consumatore_con_vettore_di_stato_e_diverse_elaborazioni)
* [Vettore di stato, con più tipologie di prodotto e condition variables](ambiente_globale/produttore_consumatore/vettore_di_stato_con_piu_tipologie_di_prodotto_e_condition_variables)
* [Vettore di condition variables](ambiente_globale/produttore_consumatore/vettore_di_condition_variables)<sup><b>[con soluzione]</b></sup>
* [Prodotto scalare con vettori dinamici](ambiente_globale/produttore_consumatore/prodotto_scalare_con_vettori_dinamici)
* [Gestione del sovraccarico](ambiente_globale/produttore_consumatore/gestione_del_sovraccarico)<sup><b>[con soluzione]</b></sup>
* [Monitor con condizioni multiple per il prelievo](ambiente_globale/produttore_consumatore/monitor_con_condizioni_multiple_per_il_prelievo)
* [Vettore di buffer di caratteri](ambiente_globale/produttore_consumatore/vettore_di_buffer_di_caratteri)
* [Pipeline di buffer singoli, con tracciamento](ambiente_globale/produttore_consumatore/pipeline_di_buffer_singoli_con_tracciamento)


## Modello ad ambiente globale, lettori-scrittori

* [Lettori/scrittori su più oggetti monitor](ambiente_globale/lettori_scrittori/lettori-scrittori_su_piu_oggetti_monitor)<sup><b>[con soluzione]</b></sup>
* [Lettori/scrittori su una coppia di buffer](ambiente_globale/lettori_scrittori/lettori-scrittori_su_una_coppia_di_buffer)
* [Lettori/scrittori multipli](ambiente_globale/lettori_scrittori/lettori-scrittori_multipli)
* [Lettori/scrittori in pipeline con produttore-consumatore](ambiente_globale/lettori_scrittori/lettori-scrittori_in_pipeline_con_produttore-consumatore)<sup><b>[con soluzione]</b></sup>
* [Lettori/scrittori con versioning](ambiente_globale/lettori_scrittori/lettori-scrittori_con_versioning)
