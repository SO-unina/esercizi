#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define NAME "/so_es05_stale"

int main(void) {
    /* La prima apertura crea l'oggetto oppure riusa quello gia' presente. */
    int fd1 = shm_open(NAME, O_CREAT | O_RDWR, 0600);
    if (fd1 == -1) {
        perror("primo shm_open");
        return 1;
    }

    /* O_EXCL permette di rilevare esplicitamente un oggetto stale. */
    int fd2 = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd2 == -1 && errno == EEXIST) {
        printf("O_CREAT | O_EXCL rileva correttamente un oggetto gia' esistente.\n");
    } else if (fd2 != -1) {
        close(fd2);
    }

    close(fd1);
    if (shm_unlink(NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    return 0;
}
