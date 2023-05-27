#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main() {
    char* filename = "example.txt"; // Replace with your desired file name
    struct stat file_stat;

    int file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1) {
        perror("open() failed");
        exit(1);
    }

    if (fstat(file_descriptor, &file_stat) == -1) {
        perror("fstat() failed");
        exit(1);
    }

    printf("File properties of '%s':\n", filename);
    printf("Inode number: %lu\n", file_stat.st_ino);
    printf("Number of hard links: %lu\n", file_stat.st_nlink);
    printf("File permissions: %o\n", file_stat.st_mode & 07777);
    printf("File size: %ld bytes\n", file_stat.st_size);
    printf("File access time: %s", ctime(&file_stat.st_atime));
    printf("File modification time: %s", ctime(&file_stat.st_mtime));

    close(file_descriptor);

    return 0;
}
