#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8080
#define RESPONSE_SIZE 100

int main(int argc, char const *argv[])
{

    int client_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // creating socket for client
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("%s \n", strerror(errno));
        return 1;
    }

    // creating server address structure
    server_addr.sin_family = AF_INET; // ipv4
    server_addr.sin_port = htons(PORT);

    // assigning server url to request
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == -1)
    {
        perror("failed to specify server address");
        return 1;
    }

    // response to receive data
    char response[RESPONSE_SIZE + 1];

    // buffer to send to server
    char buffer[] = "hi from client using udp";

    printf("sending buffer to server\n");

    sendto(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));

    recvfrom(client_fd, response, RESPONSE_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);

    response[RESPONSE_SIZE] = '\0';

    printf("received response: %s\n", response);

    close(client_fd);

    return 0;
}
