#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

    struct sockaddr_in6 addr6;

    memset(&addr6, 0, sizeof(struct sockaddr_in6));
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(8000);

    // setting the ipv6 string as uint32_t
    inet_pton(AF_INET6, "2001:db8::1",&addr6.sin6_addr);

    return 0;
}
