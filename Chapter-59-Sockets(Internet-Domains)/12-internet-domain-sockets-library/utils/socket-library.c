#include "socket-library.h"

// handle sending via TCP
ssize_t sendMessage(int fd, int flags, const char *format, ...)
{
    // "this is a %dth message \n",3
    va_list args;
    va_start(args, format);

    // calculating the size
    size_t size = vsnprintf(NULL, 0, format, args) + 1; // +1 for \0

    va_end(args);

    ssize_t bytes_sent = -1;

    // dynamically allocate and print
    if (size > 512)
    {
        va_start(args, format);

        char *buffer = (char *)malloc(size);

        if (vsnprintf(buffer, size, format, args) < 0)
            return -1;

        if ((bytes_sent = send(fd, buffer, size, flags) < 0))
            return -1;

        va_end(args);
    }
    // statically allocate and print
    else
    {
        va_start(args, format);

        char buffer[size];

        if (vsnprintf(buffer, size, format, args) < 0)
            return -1;

        if ((bytes_sent = write(fd, buffer, size) < 0))
            return -1;

        va_end(args);
    }

    return bytes_sent;
}

ssize_t recvMessage(int fd, int flags, char *buffer, size_t bufferSize)
{
    // receive data
    ssize_t bytes_read = recv(fd, buffer, bufferSize - 1, flags);

    // make the buffer null terminated
    buffer[bytes_read] = '\0';

    // return the final read bytes
    return bytes_read;
}

// create a socket
int createSocket(int domain, int type, int protocol)
{
    int sfd;
    if ((sfd = socket(domain, type, protocol)) == -1)
        fatal("socket");
    return sfd;
}

// bind the socket with the provided address
void bindWithAddress(int sfd, struct sockaddr *addr, socklen_t addrLen)
{
    if (bind(sfd, addr, addrLen) == -1)
        fatalWithClose(sfd, "bind");
}

// connect to local/remote server
void connectWithServer(int sfd, struct sockaddr *addr, socklen_t addrLen)
{
    if (connect(sfd, addr, addrLen) == -1)
        fatalWithClose(sfd, "connect");
}

int createConnection(
    int domain,
    int type,
    int port,
    const char *ip,
    struct sockaddr_storage *server_addr)
{
    int cfd = createSocket(domain, type, 0);
    socklen_t addrLen;

    memset(server_addr, 0, sizeof(struct sockaddr_storage));

    if (domain == AF_INET)
    {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)server_addr;
        addrLen = sizeof(struct sockaddr_in);
        addr4->sin_family = domain;
        addr4->sin_port = htons(port);
        convertToBinaryIP(cfd, domain, ip, (struct sockaddr *)addr4);
    }
    else if (domain == AF_INET6)
    {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)server_addr;
        addrLen = sizeof(struct sockaddr_in6);
        addr6->sin6_family = domain;
        addr6->sin6_port = htons(port);
        convertToBinaryIP(cfd, domain, ip, (struct sockaddr *)addr6);
    }
    else
        exitAndCloseWithMessage(cfd, "Unsupported Domain\n");

    connectWithServer(cfd, (struct sockaddr *)server_addr, addrLen);

    return cfd;
}

// listen for specified no of clients
void listenToClient(int sfd, int nClients)
{
    if (listen(sfd, nClients) == -1)
        fatalWithClose(sfd, "listen");
}

// accept client connection
int acceptClient(int sfd, struct sockaddr *__restrict__ addr, socklen_t *__restrict__ addrLen)
{
    int cfd;
    if ((cfd = accept(sfd, addr, addrLen)) == -1)
        printf("failed to accept connection\n");
    return cfd;
}

// domain - local/ipv4/ipv6
// type - tcp/udp

// Create server that supports both IPv4 and IPv6
int createServer(
    int domain,
    int type,
    int port,
    int backlog,
    const char *ip,
    struct sockaddr_storage *server_addr)
{
    // create a socket
    int sfd = createSocket(domain, type, 0);
    socklen_t addrLen;

    // initialize the address with 0
    memset(server_addr, 0, sizeof(*server_addr));

    // handle ipv4 case
    if (domain == AF_INET)
    {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)server_addr;
        addrLen = sizeof(struct sockaddr_in);
        addr4->sin_family = domain;
        addr4->sin_port = htons(port);

        convertToBinaryIP(sfd, domain, ip, (struct in_addr *)&addr4->sin_addr);
    }

    // handle ipv6 case
    else if (domain == AF_INET6)
    {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)server_addr;
        addrLen = sizeof(struct sockaddr_in6);
        addr6->sin6_family = domain;
        addr6->sin6_port = htons(port);

        convertToBinaryIP(sfd, domain, ip, (struct in6_addr *)&addr6->sin6_addr);
    }

    // throw error when unknow domain encountered
    else
        exitAndCloseWithMessage(sfd, "Unsupported Domain\n");

    // bind the socket with the address
    bindWithAddress(sfd, (struct sockaddr *)server_addr, addrLen);

    // listen is required only for TCP
    if (type == SOCK_STREAM)
        listenToClient(sfd, backlog);

    return sfd;
}