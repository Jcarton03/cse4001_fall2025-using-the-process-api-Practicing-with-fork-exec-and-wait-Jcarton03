# Assignment: Practicing the Process API
Practicing with fork, exec, wait. 

### Overview

In this assignment, you will practice using the Process API to create processes and run programs under Linux. The goal is to gain hands-on experience with system calls related to process management. Specifically, you will practice using the unix process API functions 'fork()', 'exec()', 'wait()', and 'exit()'. 

⚠️ Note: This is not an OS/161 assignment. You will complete it directly on Linux. 

Use the Linux in your CSE4001 container. If you are using macOS, you may use the Terminal (you may need to install development tools with C/C++ compilers). 

**Reference Reading**: Arpaci-Dusseau, *Operating Systems: Three Easy Pieces*, Chapter 5 (Process API Basics)
 👉 [Chapter 5 PDF](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf)

---

### **Steps to Complete the Assignment**

1. **Accept the GitHub Classroom Invitation**
    [GitHub Link](https://classroom.github.com/a/FZh4BrQG)
2. **Set up your Repository**
   - Clone the assignment repository.
3. **Study the Reference Materials**
   - Read **Chapter 5**.
   - Download and explore the sample programs from the textbook repository:
      [OSTEP CPU API Code](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/cpu-api).
4. **Write Your Programs**
   - Adapt the provided example code to answer the assignment questions.
   - Each program should be clear, well-commented, and compile/run correctly.
   - Add your solution source code to the repository.

5. **Prepare Your Report**
   - Answer the questions in the README.md file. You must edit the README.md file and not create another file with the answers. 
   - For each question:
     - Include your **code**.
     - Provide your **answer/explanation**.
6. **Submit Your Work via GitHub**
   - Push both your **program code** to your assignment repository.
   - This push will serve as your submission.
   - Make sure all files, answers, and screenshots are uploaded and rendered properly.








---
### Questions
1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?


```cpp
// Add your code or answer here. You can also add screenshots showing your program's execution.
//The child process inherits the value of the variable before its process is called. If the value is changed in both the child and parent,
//both operations change the initial value of the variable.
//In my code, I set x = 5. I then increment x once in both the child and parent processes, and they both output 6.

//My Code:
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
```


2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

```cpp
// Add your code or answer here. You can also add screenshots showing your program's execution.
// Both child and parent processes can access the same file descriptor returned by open(), and when writing to it concurrently,
// in my example, the parent goes first, then the child.

// My Code:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); // Creating and sets the destination for write()
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
```

3. Write another program using `fork()`.The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

```cpp
// Add your code or answer here. You can also add screenshots showing your program's execution.  
// In order to not use the wait() function, I ended up using usleep() to delay the parent process by 2.5ms.

// My Code:
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
```


4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including (on Linux) `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?

```cpp
// Add your code or answer here. You can also add screenshots showing your program's execution.  
// The reason for having multiple variants of the same call of exec() is to allow for
// clarity between processes that use exec(), if every process used exec() then it would likely get confusing

// My Code: 
#define _GNU_SOURCE
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
```

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

```cpp
// Add your code or answer here. You can also add screenshots showing your program's execution.  
// In order to have the parent wait for the child process to finish you must use an exit() call that
// provides a specific status code. Then the parent process has to wait for that status code from the child.
// If the wait() is used in the child process, then it fails because it would be waiting for
// a status code from another process.

// My Code:
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

```

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

```cpp
// Add your code or answer here. You can also add screenshots showing your program's execution.  
// By using waitpid(), a process can wait for a specific process id to finish before executing which allows
// for an easier implementation of wait when waiting for another process to finish

// My code:
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
        pid_t wpid = waitpid(pid, &status, 0);  // Wait for *specific* child to finish

        if (wpid == -1) {
                perror ("waitpid");

```

7. Write a program that creates a child process, and then in the child closes standard output (STDOUT FILENO). What happens if the child calls printf() to print some output after closing the descriptor?

```cpp
// Add your code or answer here. You can also add screenshots showing your program's execution.
// After closing the descriptor, the printf() call continues, however it does not output anything visibly
// because the descriptor is closed.

// My Code:
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
```
