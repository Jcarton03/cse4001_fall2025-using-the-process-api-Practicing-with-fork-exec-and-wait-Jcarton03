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
        printf("Child: Before closing STDOUT\n");

        close(STDOUT_FILENO);  // Close standard output (file descriptor 1)

        // This will NOT appear, because stdout is closed
        printf("Child: After closing STDOUT\n");

        // Flush manually in case buffering interferes
        fflush(stdout);

        exit(0);
    } else {
        // Parent process
        wait(NULL);
        printf("Parent: Child has finished.\n");
    }

    return 0;
}
