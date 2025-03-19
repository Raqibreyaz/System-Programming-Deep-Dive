#define _POSIX_C_SOURCE 200809
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    struct addrinfo hints, *res, *temp;
    char host[] = "google.com";
    char service[] = "http";
    int status;

    memset(&hints, 0, sizeof(struct addrinfo));

    // will be both ipv4 & ipv6
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // there can be many resolved addresses for that host so it gives a linked list
    if ((status = getaddrinfo("google.com", "http", &hints, &res)) != 0)
    {
        fprintf(stdout, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    fprintf(stdout, "IP addresses for %s:\n", host);

    // traverse the linked list to get the resolved addresses
    for (temp = res; temp != NULL; temp = temp->ai_next)
    {
        void *addr = NULL;
        char ipversion[4] = "";

        // when it is ipv4
        if (temp->ai_family == AF_INET)
        {
            // typecasting it from generic socket to ipv4 for extracting ipv4 address
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)temp->ai_addr;

            // getting the binary address
            addr = &(ipv4->sin_addr);
            strncpy(ipversion, "IPv4", 4);
        }
        else
        {
            // typecasting it from generic socket to ipv6 for extracting ipv6 address
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)temp->ai_addr;

            // getting the binary address
            addr = &(ipv6->sin6_addr);
            strncpy(ipversion, "IPv6", 4);
        }

        // for storing the address string
        char ipstr[100] = "";

        // extracting the address string from binary address
        inet_ntop(temp->ai_family, addr, ipstr, sizeof(ipstr));

        fprintf(stdout, "%s: %s\n", ipversion, ipstr);
    }

    // free the linked list
    freeaddrinfo(res);

    return 0;
}
