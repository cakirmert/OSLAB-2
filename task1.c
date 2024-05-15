#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <glob.h>

void truncate_files(const char *pattern) {
    glob_t glob_result;
    glob(pattern, 0, NULL, &glob_result);

    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        int fd = open(glob_result.gl_pathv[i], O_WRONLY | O_TRUNC);
        if (fd < 0) {
            perror(glob_result.gl_pathv[i]);
        } else {
            close(fd);
            printf("File %s content erased.\n", glob_result.gl_pathv[i]);
        }
    }
    globfree(&glob_result);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename1> <filename2> ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        truncate_files(argv[i]);
    }

    return 0;
}