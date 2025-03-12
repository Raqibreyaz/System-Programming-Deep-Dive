#include <stdio.h>
#include <arpa/inet.h>
int main(int argc, char const *argv[])
{
    uint16_t port = 3000;     // Port number (16-bit)
    uint32_t ip = 0x7F000001; // 127.0.0.1 (32-bit)

    printf("port: %d\n", (short)port);
    printf("network byte ordered port: %d\n", (int)htons(port));

    printf("ip: 0x%x\n", (int)ip);
    printf("network byte ordered ip: 0x%x \n", (int)htonl(ip));
    return 0;
}
