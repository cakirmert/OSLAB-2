#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_WRONLY | O_TRUNC);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    close(fd);
    printf("File %s content erased.\n", argv[1]);
    return 0;
}
