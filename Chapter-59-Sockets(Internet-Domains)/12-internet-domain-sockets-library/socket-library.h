#define _POSIX_C_SOURCE 200809
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "custom-utilities.h"
ssize_t sendMessage(int fd, int flags, const char *format, ...);

// create a socket
int createSocket(int domain, int type, int protocol);

// bind the socket with the provided address
void bindWithAddress(int sfd, struct sockaddr *addr, size_t addrLen);

// connect to local/remote server
void connectWithServer(int sfd, struct sockaddr *addr, size_t addrLen);

int createConnection(
    int domain,
    int type,
    int port,
    const char *ip,
    struct sockaddr_storage *server_addr);

// listen for specified no of clients
void listenToClient(int sfd, int nClients);

// accept client connection
int acceptClient(int sfd, struct sockaddr *addr, size_t *addrLen);
// domain - local/ipv4/ipv6
// type - tcp/udp

// Create server that supports both IPv4 and IPv6
int createServer(
    int domain,
    int type,
    int port,
    int backlog,
    const char *ip,
    struct sockaddr_storage *server_addr);