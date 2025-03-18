#include "utils/socket-library.h"
#define BUFFER_SIZE 100
int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE];
    struct sockaddr_in addr;
    int cfd = createConnection(AF_INET, SOCK_STREAM, 3000, "0.0.0.0", (struct sockaddr_storage *)&addr);

    ssize_t receivedBytes = recvMessage(cfd, 0, buffer, BUFFER_SIZE);
    printf("%d\n", (int)receivedBytes);
    printf("received data: %s", buffer);

    return 0;
}
