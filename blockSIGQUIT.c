#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    if (sig == SIGQUIT) {
        printf("Received SIGQUIT signal\n");
        // Terminate the program on the second occurrence of SIGQUIT
        exit(0);
    }
}

int main() {
    // Set up signal handler
    signal(SIGQUIT, handle_signal);

    sigset_t new_mask, old_mask, pending_mask;

    // Block SIGQUIT signal
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    printf("Blocked SIGQUIT signal for 5 seconds\n");
    sleep(5);

    // Check for pending signals
    sigpending(&pending_mask);
    if (sigismember(&pending_mask, SIGQUIT)) {
        printf("Unblocking SIGQUIT signal\n");
        sigprocmask(SIG_UNBLOCK, &new_mask, NULL);
    }

    // Wait indefinitely
    while (1) {
        sleep(1);
    }

    return 0;
}
