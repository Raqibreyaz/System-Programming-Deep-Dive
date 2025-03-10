#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 100

void terminateProgram(const char *message, int fd)
{
    perror(message);
    if (fd != -1)
        close(fd);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int fds[2];
    ssize_t bytes_read = 0, bytes_written = 0;
    char buffer[BUFFER_SIZE + 1];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == -1)
        terminateProgram("socketpair", -1);

    // child process will write and parent process will read
    pid_t pid = fork();

    // child process
    if (pid == 0)
    {
        close(fds[1]);

        printf("child is waiting for input text\n");

        if ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) == -1)
            terminateProgram("read", fds[0]);

        printf("%d bytes got in child from input\n", (int)bytes_read);

        if ((bytes_written = write(fds[0], buffer, bytes_read)) == -1)
            terminateProgram("write", fds[0]);

        printf("%d bytes sent to parent\n", (int)bytes_written);

        close(fds[0]);
    }
    // parent process
    else
    {
        close(fds[0]);

        printf("parent waiting for buffer from child\n");

        if ((bytes_read = read(fds[1], buffer, BUFFER_SIZE)) == -1)
            terminateProgram("read", fds[1]);

        buffer[bytes_read] = '\0';

        printf("%d bytes got from child\n", (int)bytes_read);
        printf("data read: %s\n", buffer);

        close(fds[1]);
    }
    return 0;
}