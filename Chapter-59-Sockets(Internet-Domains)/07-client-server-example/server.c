#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>

#define PORT 3000
#define BUFFER_SIZE 100

int main(int argc, char const *argv[])
{
    int server_fd;
    ssize_t received_bytes, bytes_sent;
    char send_buffer[BUFFER_SIZE + 1];
    char receive_buffer[BUFFER_SIZE + 1];
    struct sockaddr_in6 server_addr, client_addr;
    char client_addr_str[INET6_ADDRSTRLEN];
    socklen_t server_addr_len = sizeof(struct sockaddr_in6);
    socklen_t client_addr_len = sizeof(struct sockaddr_in6);

    server_fd = socket(AF_INET6, SOCK_DGRAM, 0);

    // create and initialize the structure with 0
    memset(&server_addr, 0, server_addr_len);

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(3000);
    server_addr.sin6_addr = in6addr_any; // server will run on localhost

    if (bind(server_fd, (struct sockaddr *)&server_addr, server_addr_len) == -1)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    printf("server is running...\n");

    if ((received_bytes = recvfrom(server_fd, receive_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
    {
        perror("recvfrom");
        close(server_fd);
        return 1;
    }

    // extract the client's address
    inet_ntop(AF_INET6, &server_addr.sin6_addr, client_addr_str, INET6_ADDRSTRLEN);

    printf("client address is: %s \n", client_addr_str);

    receive_buffer[received_bytes] = '\0';

    printf("%d bytes data received from client\n", (int)received_bytes);
    printf("received data: %s\n", receive_buffer);

    // convert the received data to uppercase
    for (size_t i = 0; i < received_bytes; i++)
    {
        send_buffer[i] = toupper(receive_buffer[i]);
    }

    // send the data to client
    if ((bytes_sent = sendto(server_fd, send_buffer, strlen(send_buffer), 0, (struct sockaddr *)&client_addr, client_addr_len)) == -1)
    {
        perror("sendto");
        close(server_fd);
        return 1;
    }

    close(server_fd);
    return 0;
}
