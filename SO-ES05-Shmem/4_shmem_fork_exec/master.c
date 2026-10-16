#include "shared_data.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

/* Il master e' il responsabile del CICLO DI VITA della risorsa: la crea
 * in modo esclusivo, la dimensiona e, alla fine, ne rimuove il nome.
 * Non legge e non scrive i dati: quello e' compito di writer e reader,
 * eseguibili distinti lanciati con fork + exec.
 *
 * La exec sostituisce lo spazio di indirizzamento del figlio e distrugge
 * ogni mapping ereditato: writer e reader non possono ricevere la memoria
 * condivisa dal master, devono riaprirla PER NOME con shm_open. */

int main(void) {
    /* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
    shm_unlink(SHM_NAME);

    /* Create esclusiva: se l'oggetto esiste gia', errore (EEXIST). */
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    /* La dimensione va impostata dal creatore, PRIMA di ogni mmap. */
    if (ftruncate(fd, sizeof(shared_data_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    /* Il master non accede ai dati: puo' chiudere subito il descrittore.
     * L'oggetto continua ad esistere perche' il nome e' ancora registrato. */
    close(fd);

    printf("[master %d] Risorsa %s creata\n", getpid(), SHM_NAME);

    /* Primo eseguibile: il writer scrive nella shared memory. */
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        execl("./writer", "./writer", (char *)0);

        perror("Exec writer fallita");
        exit(1);
    }

    /* Attende il writer prima di lanciare il reader, cosi' i dati sono pronti. */
    if (wait(NULL) == -1) {
        perror("wait");
        exit(1);
    }

    /* Secondo eseguibile: il reader legge dalla shared memory. */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        execl("./reader", "./reader", (char *)0);

        perror("Exec reader fallita");
        exit(1);
    }

    if (wait(NULL) == -1) {
        perror("wait");
        exit(1);
    }

    /* Solo il master, creatore della risorsa, ne rimuove il nome. */
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(1);
    }

    printf("[master %d] Risorsa %s rimossa, fine.\n", getpid(), SHM_NAME);
    return 0;
}
