#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello\n");
    } else {
        // parent goes down this path (original process)
	usleep(250); // Delays the parent process for 2.5ms so the child can go first
        printf("goodbye\n");
    }
    return 0;
}
