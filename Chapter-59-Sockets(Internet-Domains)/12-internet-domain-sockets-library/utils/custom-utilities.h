// - a function which terminate program by closing file-desc with print error message by errno
// - a function which terminate program and print error message given
// perror
// close
// printf

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

void fatalWithClose(int fd, const char *msg);
void fatal(const char *msg);
void exitWithMessage(const char *__restrict__ msg);
void exitAndCloseWithMessage(int fd, const char *__restrict__ msg);
void convertToBinaryIP(int fd, int domain, const char *ip, void *addr);
const char *convertBinaryIPToString(int fd, int domain, void *addr, char *buffer, socklen_t bufferSize);