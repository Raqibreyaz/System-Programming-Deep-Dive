#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 100

// tcp
// client connects to server first
// client uses servers main url to specify whom to connect
// server is listening for all connections

// udp
// no connection between server and client
// every datagram packet is independent and has addresses of both the sender and recipient

int main(int argc, char const *argv[])
{

    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE + 1];

    // creating socket for server
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("%s \n", strerror(errno));
        return 1;
    }

    // initializing 0 to every byte
    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    // assigning server address structure
    server_addr.sin_family = AF_INET;         // ipv4
    server_addr.sin_port = htons(PORT);       // htons ensures correct byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // any client allowed;

    // binding socket with server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("binding failed");
        close(server_fd);
        return 1;
    }

    char response[] = "hi from server using udp";

    printf("waiting for client request\n");

    // receive request from client
    ssize_t bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
    buffer[BUFFER_SIZE] = '\0';

    if (bytes_received < 0)
    {
        perror("recvfrom() error");
        close(server_fd);
        return 1;
    }

    printf("received data %s\n", buffer);

    printf("%d client port",client_addr.sin_port);

    // send resonse to client
    sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);

    close(server_fd);

    return 0;
}