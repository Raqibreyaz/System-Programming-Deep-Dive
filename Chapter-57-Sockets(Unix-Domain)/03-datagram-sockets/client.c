// client
// server
// client from stdin will take input and pass it to server
// server will pass the incoming data to stdout

#include <stdio.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SOCKSTREAM "./socket-file"
#define BUFFER_SIZE 100

int main(int argc, char const *argv[])
{
    int cfd;
    if ((cfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket file descriptor");
        return 1;
    }

    // will store the address of the file
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(struct sockaddr_un)); // put 0 in all

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKSTREAM, sizeof(addr.sun_path) - 1);

    // connect to peer server
    if (connect(cfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("connect");
        return -1;
    }

    // while (1)
    // {

    // buffer for reading data
    char buffer[BUFFER_SIZE + 1];

    printf("Enter text to send to server\n");

    // reading from standard input
    ssize_t bytes_read;
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        // putting terminator
        buffer[(int)bytes_read] = '\0';

        printf("\ngot %ld bytes data\n", bytes_read);

        // sending data to server
        ssize_t bytes_sent;
        if ((bytes_sent = send(cfd, buffer, bytes_read, 0)) == -1)
        {
            perror("failed to send");
        }
        else
            printf("sent %ld bytes data\n", bytes_sent);
    }
    if (bytes_read == -1)
    {
        perror("read");
        return 1;
    }

    // }
    close(cfd);
    return 0;
}
