#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int fd = open('./file.txt', O_RDONLY);
    char buffer[11];
    ssize_t bytes_read = pread(fd, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes_read] = '\0';
    printf("%d bytes are written: %s \n", (int)bytes_read, buffer);
    return 0;
}
