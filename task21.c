#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define LOCK_FILE "lockfile"
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
    while (open(LOCK_FILE, O_CREAT | O_EXCL, 0444) == -1) {
        sleep(1);
    }
    write_to_file(message);
    read_from_file();
    remove(DATA_FILE);
    remove(LOCK_FILE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    lock_and_process(argv[1]);
    return 0;
}
