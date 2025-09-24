#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int x = 5; // This is my variable
    printf("hello, my int %d (pid:%d)\n", x, (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
	x++; // Changing value of x in child
	printf("hello, am child, incremented x by 1 = %d (pid:%d)\n", x, (int) getpid());
    } else {
        // parent goes down this path (original process)
	x++; // Changing value of x in parent
        printf("hello, am parent of %d, incremented x by 1 = %d (pid:%d)\n",
	       rc, x, (int) getpid());
    }
    return 0;
}
