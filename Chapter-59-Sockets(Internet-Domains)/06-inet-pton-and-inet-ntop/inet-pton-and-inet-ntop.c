#include <stdio.h>
#include <arpa/inet.h>
int main(int argc, char const *argv[])
{
    struct sockaddr_in ipv4_addr;
    struct sockaddr_in6 ipv6_addr;

    char ipv4_addr_str[INET_ADDRSTRLEN];
    char ipv6_addr_str[INET6_ADDRSTRLEN];

    ipv4_addr.sin_family = AF_INET;
    ipv4_addr.sin_port = htons(8000);

    ipv6_addr.sin6_family = AF_INET6;
    ipv6_addr.sin6_port = htons(8001);

    // storing the binary version of ipv4
    inet_pton(AF_INET, "127.0.0.1", &ipv4_addr.sin_addr);

    // storing the binary version of ipv6
    inet_pton(AF_INET6, "::1", &ipv6_addr.sin6_addr);

    // reconverting the binary ip to string ip
    inet_ntop(AF_INET, &ipv4_addr.sin_addr, ipv4_addr_str, INET_ADDRSTRLEN);

    inet_ntop(AF_INET6, &ipv6_addr.sin6_addr, ipv6_addr_str, INET6_ADDRSTRLEN);

    printf("ipv4: %s\n",ipv4_addr_str);
    printf("ipv6: %s\n",ipv6_addr_str);

    return 0;
}
