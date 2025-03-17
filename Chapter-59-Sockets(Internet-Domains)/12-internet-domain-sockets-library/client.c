#include "socket-library.h"
#define BUFFER_SIZE 100
int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE + 1];
    struct sockaddr_in addr;
    int cfd = createConnection(AF_INET, SOCK_STREAM, 3000, "0.0.0.0", (struct sockaddr_storage *)&addr);

    ssize_t bytes_received = recv(cfd, buffer, BUFFER_SIZE, 0);
    buffer[bytes_received] = '\0';

    printf("received data: %s", buffer);

    return 0;
}
