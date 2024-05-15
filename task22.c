#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DATA_FILE "datafile"

void write_to_file(const char *message) {
    FILE *file = fopen(DATA_FILE, "w");
    fprintf(file, "%s\n", message);
    fclose(file);
}

void read_from_file() {
    char buffer[256];
    FILE *file = fopen(DATA_FILE, "r");
    fgets(buffer, sizeof(buffer), file);
    printf("Read from file: %s", buffer);
    fclose(file);
}

void lock_and_process(const char *message) {
    int fd = open(DATA_FILE, O_CREAT | O_RDWR, 0666);
    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    while (fcntl(fd, F_SETLK, &lock) == -1) {
        sleep(1);
    }
    write_to_file(message);
    read_from_file();
    ftruncate(fd, 0);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    lock_and_process(argv[1]);
    return 0;
}
