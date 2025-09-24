#define _GNU_SOURCE  // Needed for execvpe()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Helper function to fork and exec
void run_exec_variant(const char *name, void (*exec_func)(void)) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        printf("\nRunning %s:\n", name);
        exec_func();
        perror("exec failed");
        exit(1);
    } else {
        wait(NULL);  // Wait for child to finish
    }
}

// Variant implementations

void run_execl() {
    execl("/bin/ls", "ls", "-l", NULL);
}

void run_execle() {
    char *env[] = {"MYVAR=execle_example", NULL};
    execle("/bin/ls", "ls", "-l", NULL, env);
}

void run_execlp() {
    execlp("ls", "ls", "-l", NULL);  // Searches PATH
}

void run_execv() {
    char *args[] = {"ls", "-l", NULL};
    execv("/bin/ls", args);
}

void run_execvp() {
    char *args[] = {"ls", "-l", NULL};
    execvp("ls", args);  // Searches PATH
}

void run_execvpe() {
    char *args[] = {"ls", "-l", NULL};
    char *env[] = {"MYVAR=execvpe_example", NULL};
    execvpe("ls", args, env);  // Linux only
}

int main() {
    run_exec_variant("execl", run_execl);
    run_exec_variant("execle", run_execle);
    run_exec_variant("execlp", run_execlp);
    run_exec_variant("execv", run_execv);
    run_exec_variant("execvp", run_execvp);
    run_exec_variant("execvpe", run_execvpe);
    return 0;
}
