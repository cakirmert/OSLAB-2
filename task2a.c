#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define LOCK_FILE "lockfile"
#define DATA_FILE "text.txt"

// Function to write a message to the data file
void write_to_file(const char *message) {
    FILE *file = fopen(DATA_FILE, "w");
    if (file) {
        fprintf(file, "%s\n", message);
        fclose(file);
    } else {
        perror("Failed to open data file for writing");
    }
}

// Function to read and print the content from the data file
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

// Function to manage the lock file and process the message
void lock_and_process(const char *message) {
    FILE *lock_file;
    
    // Attempt to create the lock file exclusively
    while ((lock_file = fopen(LOCK_FILE, "wx")) == NULL) {
        printf("Failed to create lock file, retrying (process: %s)\n", message);
        sleep(1); // Wait if the lock file exists
    }
    
    // Lock acquired
    printf("Lock acquired by process: %s\n", message);

    // Write message to data file
    write_to_file(message);

    // Read and print message from data file
    read_from_file();

    // Close and remove the lock file to release the lock
    fclose(lock_file);
    remove(LOCK_FILE);

    // Lock released
    printf("Lock released by process: %s\n", message);
}

int main(int argc, char *argv[]) {
    // Ensure the correct number of arguments are provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    // Repeat the lock and process operation for a few iterations
    for (int i = 0; i < 5; ++i) { // Limit the number of iterations to avoid an infinite loop
        lock_and_process(argv[1]);
        sleep(1); // Simulate some delay
    }

    return 0;
}
