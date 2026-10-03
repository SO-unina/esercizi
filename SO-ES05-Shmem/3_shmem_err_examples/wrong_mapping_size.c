#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define NAME "/so_es05_size_check"

int main(void) {
    const size_t object_size = 128;
    const size_t requested_size = 4096*2;

    shm_unlink(NAME);

    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    if (ftruncate(fd, object_size) == -1) {
        perror("ftruncate");
        return 1;
    }

    /* La dimensione effettiva va verificata prima di usare un mapping piu grande. */
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        return 1;
    }

    printf("Oggetto: %lld byte; mapping richiesto: %zu byte\n", (long long)st.st_size, requested_size);

    // gestione corretta

    if ((size_t)st.st_size < requested_size) {
        printf("Il programma rifiuta il mapping: l'accesso oltre la dimensione dell'oggetto potrebbe generare SIGBUS.\n");
    }

    // gestione errata

    int *value = mmap(NULL, requested_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (value == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    value[0] = 1;       /* offset 0: dentro l'oggetto: ok */
    value[100] = 2;     /* offset 400: oltre i 128 byte ma nella stessa pagina: ok (!) */
    printf("accessi nella prima pagina riusciti\n");

    printf("ora accedo alla seconda pagina (offset 4096): SIGBUS atteso...\n");
    value[1024] = 3;    /* 1024 * sizeof(int) = offset 4096: pagina oltre l'oggetto -> SIGBUS */

    printf("questa riga non viene mai raggiunta\n");
    
    close(fd);
    shm_unlink(NAME);
    return 0;
}
