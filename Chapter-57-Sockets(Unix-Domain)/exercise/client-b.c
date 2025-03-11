#include <stdio.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define BUFFER_SIZE 100
#define SOCK_PATH "socket-file"

// in this program i will use linux abstract socket namespace

int main(int argc, char const *argv[])
{
    int cfd;
    struct sockaddr_un server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_un);

    if ((cfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket file descriptor");
        return 1;
    }

    memset(&server_addr, 0, addr_len); // put 0 in all

    server_addr.sun_family = AF_UNIX;

    // creating an abstract socket file
    strncpy(server_addr.sun_path, SOCK_PATH, strlen(SOCK_PATH));

    while (1)
    {
        char buffer[BUFFER_SIZE + 1] = "hi from client b ";

        printf("sending data to server\n");
        sendto(cfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len);

        sleep(2);
    }
    close(cfd);

    return 0;
}
