#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    char* filename = "example.txt"; // Replace with your desired file name
    int file_descriptor;
    struct stat file_stat;
    char* file_contents;
    off_t file_size;
    
    // Open the file in read-only mode
    file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1) {
        perror("open() failed");
        exit(1);
    }
    
    // Get the file size
    if (fstat(file_descriptor, &file_stat) == -1) {
        perror("fstat() failed");
        exit(1);
    }
    file_size = file_stat.st_size;
    
    // Map the file into memory
    file_contents = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0);
    if (file_contents == MAP_FAILED) {
        perror("mmap() failed");
        exit(1);
    }
    
    // Print the contents of the mapped file in reverse
    for (off_t i = file_size - 1; i >= 0; i--) {
        printf("%c", file_contents[i]);
    }
    
    // Unmap the file from memory
    if (munmap(file_contents, file_size) == -1) {
        perror("munmap() failed");
        exit(1);
    }
    
    // Close the file descriptor
    close(file_descriptor);
    
    return 0;
}
