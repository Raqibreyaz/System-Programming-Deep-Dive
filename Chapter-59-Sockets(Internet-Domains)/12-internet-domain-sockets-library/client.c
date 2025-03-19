#include "utils/socket-library.h"

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE];
    struct sockaddr_in addr;

    int cfd = createConnection(AF_INET, SOCK_STREAM, "google.com", "http", (struct sockaddr_storage *)&addr);

    sendMessage(cfd, 0, "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n");

    printf("sent request to google.com\n");

    // ssize_t receivedBytes = recvMessage(cfd, 0, buffer, BUFFER_SIZE);
    recvAllData(cfd, buffer, BUFFER_SIZE, 0);

    printf("received data from google.com\n");

    printf("received data: %s\n", buffer);

    return 0;
}
