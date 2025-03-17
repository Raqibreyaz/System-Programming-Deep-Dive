// - a function which terminate program by closing file-desc with print error message by errno
// - a function which terminate program and print error message given
// perror
// close
// printf
#include "custom-utilities.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

void fatalWithClose(int fd, const char *msg)
{
    perror(msg);
    close(fd);
    exit(EXIT_FAILURE);
}
void fatal(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
void exitWithMessage(const char *msg)
{
    fprintf(stderr, msg);
    exit(EXIT_FAILURE);
}

void exitAndCloseWithMessage(int fd, const char *msg)
{
    fprintf(stderr, msg);
    close(fd);
    exit(EXIT_FAILURE);
}

void convertToBinaryIP(int fd, int domain, const char *ip, void *addr)
{
    if (inet_pton(domain, ip, addr) <= 0)
        fatalWithClose(fd, "inet_pton");
}

const char *convertBinaryIPToString(int fd, int domain, void *addr, char *buffer, socklen_t bufferSize)
{
    const char *ip = inet_ntop(domain, addr, buffer, bufferSize);
    if (ip == NULL)
        fatalWithClose(fd, "inet_ntop");
    return ip;
}