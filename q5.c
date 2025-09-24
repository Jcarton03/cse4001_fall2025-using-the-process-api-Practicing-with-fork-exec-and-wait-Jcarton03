#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        printf("Child (PID %d): Running...\n", getpid());
        sleep(2);  // Simulate some work
        printf("Child (PID %d): Finished.\n", getpid());
        exit(42);  // Exit with a specific status code
    } else {
        // Parent process
        int status;
        pid_t wpid = wait(&status);  // Wait for child to finish

        printf("Parent (PID %d): Child with PID %d has finished.\n", getpid(), wpid);

        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not exit normally.\n");
        }
    }

    return 0;
}
