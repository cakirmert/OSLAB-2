#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define DATA_FILE "test.txt"

void write_to_file(const char *message) {
    FILE *file = fopen(DATA_FILE, "w");
    if (file) {
        fprintf(file, "%s\n", message);
        fclose(file);
    } else {
        perror("Failed to open data file for writing");
    }
}

void read_from_file() {
    char buffer[256];
    FILE *file = fopen(DATA_FILE, "r");
    if (file) {
        if (fgets(buffer, sizeof(buffer), file)) {
            printf("Read from file: %s", buffer);
        }
        fclose(file);
    } else {
        perror("Failed to open data file for reading");
    }
}

void lock_and_process(const char *message) {
    int fd = open(DATA_FILE, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Failed to open data file");
        exit(1);
    }
    
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;

    printf("Trying to acquire lock: %s\n", message);
    while (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to acquire lock, retrying");
        sleep(1);  // Wait if the file is locked
    }
    printf("Lock acquired: %s\n", message);

    write_to_file(message);
    read_from_file();

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    printf("Lock released: %s\n", message);

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    for (int i = 0; i < 5; ++i) {  // Limit the number of iterations to avoid infinite loop
        lock_and_process(argv[1]);
        sleep(1);  // Simulate some delay
    }

    return 0;
}
