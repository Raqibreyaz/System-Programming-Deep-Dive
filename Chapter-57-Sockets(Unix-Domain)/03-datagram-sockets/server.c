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
#define BACKLOG_COUNT 10
#define BUFFER_SIZE 10

int main(int argc, char const *argv[])
{

    // will remove the socket file if already exists, if not ignore
    remove(SOCKSTREAM); //-1 for not found, 0 for success

    int sfd;
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket file descriptor");
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un)); // put 0 in all

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKSTREAM, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        return 1;
    }

    if (listen(sfd, BACKLOG_COUNT) == -1)
    {
        perror("listen");
        return 1;
    }

    // while (1)
    // {
    int cfd;
    char buffer[BUFFER_SIZE + 1];
    ssize_t bytes_received;

    // accept client connection
    if ((cfd = accept(sfd, NULL, NULL)) == -1)
    {
        perror("accept");
        return 1;
    }

    // receive data from client
    while ((bytes_received = recv(cfd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        printf("\nreceived %ld bytes data\n", bytes_received);

        // writing the received data to terminal or redirected file
        if (write(STDOUT_FILENO, buffer, bytes_received) == -1)
        {
            perror("write");
            return 1;
        }
    }
    if (bytes_received == -1)
    {
        perror("receive");
    }
    // }

    close(sfd);

    return 0;
}
