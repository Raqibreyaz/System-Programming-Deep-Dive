#include "utils/socket-library.h"

#define BUFFER_SIZE 100

int main(int argc, char const *argv[])
{
    // for storing server address
    struct sockaddr_in addr;

    // for storing data
    char buffer[BUFFER_SIZE];

    // for storing amount of bytes data received/sent
    ssize_t bytes_received = 0, bytes_sent = 0;

    // create a server and return the socket file descriptor and address
    int sfd = createServer(AF_INET, SOCK_STREAM, 3000, 10, "0.0.0.0", (struct sockaddr_storage *)&addr);

    while (1)
    {
        int cfd = acceptClient(sfd, NULL, NULL);

        bytes_received = recvMessage(cfd, 0, buffer, BUFFER_SIZE);
        printf("received data: %s\n%d\n", buffer, (int)bytes_received);

        bytes_sent = sendMessage(cfd, 0, "%d bytes data got at server from client   ", (int)bytes_received);
    }
    return 0;
}