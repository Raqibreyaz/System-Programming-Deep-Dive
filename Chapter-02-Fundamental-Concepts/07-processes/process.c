#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    int status;
    if (pid < 0)
    {
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("i am the child process, PID = %d\n", getpid());
        sleep(1);
        // terminating the child process with sucess status
        _exit(1);
    }
    else
    {
        printf("I am the parent process, with PID = %d\n", getppid());
        // waiting for the child
        wait(&status);
        printf("%d\n", status);
        // checking
        if (WIFEXITED(status))
        {
            printf("child exited with status %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
