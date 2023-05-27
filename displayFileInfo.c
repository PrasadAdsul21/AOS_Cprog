#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    char* filename = argv[1];
    struct stat file_stat;

    if (stat(filename, &file_stat) == -1) {
        perror("stat() failed");
        exit(1);
    }

    printf("File: %s\n", filename);

    // File type
    if (S_ISREG(file_stat.st_mode))
        printf("Type: Regular File\n");
    else if (S_ISDIR(file_stat.st_mode))
        printf("Type: Directory\n");
    else if (S_ISLNK(file_stat.st_mode))
        printf("Type: Symbolic Link\n");
    else
        printf("Type: Unknown\n");

    // File access permissions
    printf("Permissions: ");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    // User and group IDs
    struct passwd* pw = getpwuid(file_stat.st_uid);
    struct group* gr = getgrgid(file_stat.st_gid);
    printf("User ID: %d (%s)\n", file_stat.st_uid, pw ? pw->pw_name : "unknown");
    printf("Group ID: %d (%s)\n", file_stat.st_gid, gr ? gr->gr_name : "unknown");

    // File size
    printf("File Size: %ld bytes\n", file_stat.st_size);

    // File access and modification times
    printf("Access Time: %s", ctime(&file_stat.st_atime));
    printf("Modification Time: %s", ctime(&file_stat.st_mtime));

    return 0;
}
