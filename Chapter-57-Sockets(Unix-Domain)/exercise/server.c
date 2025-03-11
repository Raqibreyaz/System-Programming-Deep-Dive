#include <stdio.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stddef.h>

#define BUFFER_SIZE 100
#define SOCK_PATH "socket-file"

// in this program i will use linux abstract socket namespace

int main(int argc, char const *argv[])
{
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

    // creating an abstract socket file
    // strncpy(&server_addr.sun_path[1], SOCK_PATH, strlen(SOCK_PATH));
    strncpy(server_addr.sun_path, SOCK_PATH, strlen(SOCK_PATH));
    
    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        return 1;
    }

    char buffer[BUFFER_SIZE + 1];
    ssize_t received_bytes = 0;

    // first receive data from first client
    received_bytes = recvfrom(sfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);

    // now connect with that client
    if (connect(sfd, (struct sockaddr *)&client_addr, addr_len) == -1)
        printf("connect failed to client");

    do
    {
        buffer[received_bytes] = '\0';
        printf("received data: %s\n", buffer);
        received_bytes = recv(sfd, buffer, BUFFER_SIZE, 0);
    } while (received_bytes > 0);

    close(sfd);

    return 0;
}
