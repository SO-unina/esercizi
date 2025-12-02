# Lettori/scrittori su una coppia di buffer

Si realizzi in linguaggio C/C++ un'applicazione **multithread** che
effettui un calcolo distribuito secondo lo schema seguente, da
realizzare utilizzando il costrutto **monitor**.

Il thread principale dovrà creare **due buffer condivisi** (<img src="https://render.githubusercontent.com/render/math?math=B_1"> e
<img src="https://render.githubusercontent.com/render/math?math=B_2">), che conterranno ognuno un valore di tipo intero, e inizialmente
pari a 0. Il thread principale dovrà poi creare dei thread scrittori e
lettori, e attenderne la terminazione per poi terminare a sua volta.

Un gruppo di quattro thread **scrittori** (due scrittori per ogni
buffer) dovrà inserire periodicamente (una volta ogni secondo) degli
ipotetici valori di temperatura (valore casuale tra 10 e 30). Gli
scrittori dovranno iterare per 6 volte.

Un gruppo di due thread **lettori** dovrà leggere periodicamente (una
volta ogni 2 secondi) dai buffer condivisi (ogni lettore legge da
entrambi i buffer una coppia di valori). Il primo lettore dovrà
calcolare e stampare a video la media tra i due valori. Il secondo
lettore dovrà calcolare e stampare a video il massimo tra i due valori.
I lettori dovranno iterare per 6 volte.

![image](/images/ambiente_globale/lettori_scrittori/lettori-scrittori_su_una_coppia_di_buffer.png)
