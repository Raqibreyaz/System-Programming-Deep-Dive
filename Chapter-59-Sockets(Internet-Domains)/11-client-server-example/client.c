#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PORT 8000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 5
#define RESPONSE_SIZE 15

int main(int argc, char const *argv[])
{
    int sfd;
    char buffer[BUFFER_SIZE + 1];
    char response[RESPONSE_SIZE];
    ssize_t bytes_received, bytes_sent;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    memset(&server_addr, 0, addr_len);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // localhost

    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return 1;
    }

    // connect with server
    if (connect(sfd, (struct sockaddr *)&server_addr, addr_len) == -1)
    {
        perror("connect");
        close(sfd);
        return 1;
    }

    while (1)
    {
        printf("enter range \n");
        ssize_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        printf("%d \n", bytes_read);
        if (buffer[bytes_read - 1] == '\n')
        {
            buffer[bytes_read - 1] = '\0';
        }

        // send the request to server
        if ((bytes_sent = send(sfd, buffer, bytes_read - 1, 0)) == -1)
        {
            perror("send");
            close(sfd);
            return 1;
        }

        // receive response from server
        bytes_received = recv(sfd, response, RESPONSE_SIZE, 0);
        response[bytes_received] = '\0';

        printf("data received: %s \n", response);
    }
    close(sfd);

    return 0;
}
