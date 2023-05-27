#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void signal_handler(int signum) {
    // Empty signal handler to block the signals
}

int main() {
    // Block SIGINT and SIGQUIT signals
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);

    int pipe_fd[2];
    pid_t ls_pid, wc_pid;
    int status;

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe() failed");
        exit(1);
    }

    // Fork a child process for ls -l
    ls_pid = fork();
    if (ls_pid == -1) {
        perror("fork() failed");
        exit(1);
    } else if (ls_pid == 0) {
        // Child process: ls -l
        close(pipe_fd[0]);  // Close the read end of the pipe

        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        // Execute ls -l using exec
        execlp("ls", "ls", "-l", NULL);

        perror("execlp(ls -l) failed");
        exit(1);
    }

    // Fork another child process for wc -l
    wc_pid = fork();
    if (wc_pid == -1) {
        perror("fork() failed");
        exit(1);
    } else if (wc_pid == 0) {
        // Child process: wc -l
        close(pipe_fd[1]);  // Close the write end of the pipe

        // Redirect stdin to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        // Execute wc -l using exec
        execlp("wc", "wc", "-l", NULL);

        perror("execlp(wc -l) failed");
        exit(1);
    }

    // Parent process
    close(pipe_fd[0]);  // Close both ends of the pipe
    close(pipe_fd[1]);

    // Wait for both child processes to finish
    waitpid(ls_pid, &status, 0);
    waitpid(wc_pid, &status, 0);

    return 0;
}
