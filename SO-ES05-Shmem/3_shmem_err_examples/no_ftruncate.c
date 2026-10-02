#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define NAME "/so_es05_no_ftruncate"

int main(void) {
    shm_unlink(NAME);

    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    /* fstat mostra che un nuovo oggetto shared memory ha dimensione zero. */
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        return 1;
    }

    printf("Dimensione prima di ftruncate: %lld byte\n", (long long)st.st_size);
    printf("Non si accede al mapping: prima occorre dimensionare " "l'oggetto con ftruncate.\n");

    close(fd);
    shm_unlink(NAME);
    return 0;
}
