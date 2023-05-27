#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    mode_t old_umask = umask(0000); // Set umask to 0000 to allow all permissions
    
    // Create the first file with read and write permissions for owner, group, and other users
    int fd1 = open("file1.txt", O_CREAT | O_WRONLY, 0666);
    if (fd1 == -1) {
        perror("open() failed for file1.txt");
        exit(1);
    }
    
    // Turn on group-id and turn off group execute permission for the first file
    if (fchmod(fd1, S_ISGID | 0644) == -1) {
        perror("fchmod() failed for file1.txt");
        exit(1);
    }
    
    close(fd1);
    
    // Create the second file with read and write permissions for the owner only
    int fd2 = open("file2.txt", O_CREAT | O_WRONLY, 0600);
    if (fd2 == -1) {
        perror("open() failed for file2.txt");
        exit(1);
    }
    
    // Set the read permission for all users for the second file
    if (chmod("file2.txt", S_IRUSR | S_IRGRP | S_IROTH) == -1) {
        perror("chmod() failed for file2.txt");
        exit(1);
    }
    
    close(fd2);
    
    umask(old_umask); // Restore the original umask value
    
    printf("Files created with permissions successfully.\n");
    
    return 0;
}
