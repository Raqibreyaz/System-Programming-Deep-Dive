#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10

int main(int argc, char const *argv[])
{
    // creating socket for server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        printf("%s \n", strerror(errno));
        return 1;
    }

    // creating server address structure
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET, // ipv4
        .sin_port = htons(PORT),
    };
    server_addr.sin_addr.s_addr = INADDR_ANY; // any client allowed;

    // binding socket with server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("binding failed");
        return 1;
    }

    if (listen(server_fd, MAX_CLIENTS) == -1)
    {
        perror("failed to listen");
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    struct sockaddr_in client_addr;
    socklen_t client_len;

    if (accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len) == -1)
    {
        perror("failed to accept connection");
        return 1;
    }

    printf("client connected \n");

    close(server_fd);

    return 0;
}
