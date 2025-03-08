#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/unistd.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    int sfd;

    // since processes are on current computer not with network so use AF_UNIX
    // for realiable connection using streaming sockets
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket file descriptor");
        return 1;
    }

    // this will be temporary file and will be deleted after 1s
    char sockname[] = "./temp";

    // for storing and binding the address of the file
    struct sockaddr_un addr;

    // putting 0 to every property
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;

    // copying the address to sun_path
    strncpy(addr.sun_path, sockname, sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(sockname)] = '\0';

    // bind the address to socket
    if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        return 1;
    }

    printf("socket bound to address %s\n", sockname);

    close(sfd);

    sleep(1);

    unlink(sockname);

    return 0;
}
