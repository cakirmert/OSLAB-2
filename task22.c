#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DATA_FILE "datafile"
#define MESSAGE "Hello from process "
#define MAX_ITERATIONS 10  // Limit the number of iterations

void write_to_file(const char *process_name) {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        perror("Failed to open data file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s%s\n", MESSAGE, process_name);
    fclose(file);
}

void read_from_file() {
    char buffer[256];
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        perror("Failed to open data file");
        exit(EXIT_FAILURE);
    }
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Read from file: %s", buffer);
    }
    fclose(file);
}

void lock_and_process(const char *process_name) {
    int fd = open(DATA_FILE, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("Failed to open data file");
        exit(EXIT_FAILURE);
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    while (fcntl(fd, F_SETLK, &lock) == -1) {
        sleep(1); // Wait if the file is locked
    }

    write_to_file(process_name);
    read_from_file();
    ftruncate(fd, 0);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <process_name>\n", argv[0]);
        return 1;
    }

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        lock_and_process(argv[1]);
        sleep(2); // Simulate some delay
    }

    printf("Process %s completed.\n", argv[1]);
    return 0;
}
