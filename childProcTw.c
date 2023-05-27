#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <unistd.h>

int main() {
    int n = 5;  // Number of child processes to create
    int i;
    struct tms start_time, end_time;
    clock_t start_clock, end_clock;
    int child_status;
    pid_t child_pid;

    start_clock = times(&start_time);  // Record start time

    for (i = 0; i < n; i++) {
        child_pid = fork();

        if (child_pid == -1) {
            perror("fork() failed");
            exit(1);
        } else if (child_pid == 0) {
            // Child process
            printf("Child process %d with PID %d\n", i + 1, getpid());
            sleep(i + 1);  // Simulate child process doing some work
            exit(0);
        }
    }

    // Parent process
    while ((child_pid = wait(&child_status)) > 0) {
        if (WIFEXITED(child_status)) {
            printf("Child process with PID %d terminated\n", child_pid);
        } else {
            printf("Child process with PID %d terminated abnormally\n", child_pid);
        }
    }

    end_clock = times(&end_time);  // Record end time

    // Calculate total cumulative time
    clock_t user_time = end_time.tms_utime - start_time.tms_utime;
    clock_t kernel_time = end_time.tms_stime - start_time.tms_stime;

    printf("\nTotal cumulative time spent by children:\n");
    printf("User mode time: %ld clock ticks\n", (long)user_time);
    printf("Kernel mode time: %ld clock ticks\n", (long)kernel_time);

    return 0;
}
