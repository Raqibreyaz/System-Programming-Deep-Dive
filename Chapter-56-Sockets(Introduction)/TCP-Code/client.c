#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    // creating socket for client
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1)
    {
        printf("%s \n", strerror(errno));
        return 1;
    }

    // creating server address structure
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET, // ipv4
        .sin_port = htons(PORT),
    };

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == -1)
    {
        perror("failed to specify server address");
        return 1;
    }

    // connecto to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("failed to connect with server");
        return 1;
    }

    printf("connection successfully established\n");

    close(client_fd);

    return 0;
}
