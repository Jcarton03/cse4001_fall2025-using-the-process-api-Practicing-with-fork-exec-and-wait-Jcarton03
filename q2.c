#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    const char *msg;
    if (pid == 0) {
        // Child process
        msg = "Child writing to file...\n";
	write(fd, msg, strlen(msg)); // Should be writing at the same time as parent
    } else {
        // Parent process
        msg = "Parent writing to file...\n";
    	write(fd, msg, strlen(msg)); // Should be writing with the child, but goes first
    }

    close(fd);
    return 0;
}
