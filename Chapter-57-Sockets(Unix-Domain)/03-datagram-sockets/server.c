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
#define BUFFER_SIZE 10

int main(int argc, char const *argv[])
{

    // will remove the socket file if already exists, if not ignore
    remove(SOCKSTREAM); //-1 for not found, 0 for success

    int sfd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_un);

    if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket file descriptor");
        return 1;
    }

    memset(&server_addr, 0, addr_len); // put 0 in all
    memset(&client_addr, 0, addr_len);

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKSTREAM, sizeof(server_addr.sun_path) - 1);

    server_addr.sun_path[strlen(server_addr.sun_path)] = '\0';

    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        return 1;
    }

    while (1)
    {
        char buffer[BUFFER_SIZE + 1];

        printf("waiting for client data\n");

        // receive data from client
        ssize_t received_bytes = recvfrom(sfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);

        printf("got %d bytes data\n", (int)received_bytes);

        buffer[received_bytes] = '\0';

        if (write(STDOUT_FILENO, buffer, received_bytes) == -1)
        {
            perror("bind");
        }
    }

    close(sfd);

    return 0;
}
