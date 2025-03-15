#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    struct sockaddr_in addr;
    int status = -1;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // initialize the addr with 0
    memset(&addr, 0, addr_len);

    // will use ipv4
    addr.sin_family = AF_INET;

    // the port
    addr.sin_port = htons(53);

    // convert string ip to binary ip
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    // get the host and service name
    if ((status = getnameinfo((struct sockaddr *)&addr, addr_len, host, sizeof(host), service, sizeof(service), NI_DGRAM)) != 0)
    {
        fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    fprintf(stdout, "host: %s\nservice: %s\n", host, service);

    return 0;
}
