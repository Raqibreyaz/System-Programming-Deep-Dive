#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>

#define PORT 3000
#define BUFFER_SIZE 100

int main(int argc, char const *argv[])
{
    int client_fd;
    char receive_buffer[BUFFER_SIZE + 1];
    char send_buffer[BUFFER_SIZE + 1] = "this is a message!";
    struct sockaddr_in6 server_addr;
    ssize_t received_bytes, bytes_sent;
    socklen_t addr_len = sizeof(struct sockaddr_in6);

    client_fd = socket(AF_INET6, SOCK_DGRAM, 0);

    // create and initialize the structure with 0
    memset(&server_addr, 0, addr_len);

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(3000);
    server_addr.sin6_addr = in6addr_loopback; // server will run on localhost

    // send the data to server
    if ((bytes_sent = sendto(client_fd, send_buffer, strlen(send_buffer), 0, (struct sockaddr *)&server_addr, addr_len)) == -1)
    {
        perror("sendto");
        close(client_fd);
        return 1;
    }

    // receive data from server
    if ((received_bytes = recvfrom(client_fd, receive_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len)) == -1)
    {
        perror("recvfrom");
        close(client_fd);
        return 1;
    }

    receive_buffer[received_bytes] = '\0';

    printf("%d bytes data received from server\n", (int)received_bytes);
    printf("received data: %s\n", receive_buffer);
    
    close(client_fd);
    return 0;
}
