#include "socket-library.h"

int main(int argc, char const *argv[])
{
    struct sockaddr_in addr;
    int sfd = createServer(AF_INET, SOCK_STREAM, 3000, 10, "0.0.0.0", (struct sockaddr_storage *)&addr);

    while (1)
    {
        int cfd = acceptClient(sfd, NULL, NULL);
        sendMessage(cfd, 0, "hi this is a message");
    }
    return 0;
}