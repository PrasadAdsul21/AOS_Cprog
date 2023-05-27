#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    DIR* dir;
    struct dirent* entry;
    char* output_filename = "merged.txt";
    int output_fd;

    // Open the current directory
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir() failed");
        exit(1);
    }

    // Create the output file
    output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("open() failed");
        closedir(dir);
        exit(1);
    }

    // Read files in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the file ends with ".txt"
        if (strlen(entry->d_name) >= 4 && strcmp(entry->d_name + strlen(entry->d_name) - 4, ".txt") == 0) {
            // Open the input file
            int input_fd = open(entry->d_name, O_RDONLY);
            if (input_fd == -1) {
                perror("open() failed");
                continue;
            }

            // Read from the input file and write to the output file
            char buffer[BUFFER_SIZE];
            ssize_t bytes_read;
            while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
                ssize_t bytes_written = write(output_fd, buffer, bytes_read);
                if (bytes_written == -1) {
                    perror("write() failed");
                    close(input_fd);
                    close(output_fd);
                    closedir(dir);
                    exit(1);
                }
            }

            // Close the input file
            close(input_fd);
        }
    }

    // Close the output file
    close(output_fd);

    // Close the directory
    closedir(dir);

    // Open the merged file again and return its file descriptor
    int merged_fd = open(output_filename, O_RDONLY);
    if (merged_fd == -1) {
        perror("open() failed");
        exit(1);
    }

    printf("Merged file '%s' created successfully.\n", output_filename);

    return merged_fd;
}
