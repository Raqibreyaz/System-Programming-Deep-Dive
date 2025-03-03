#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    int fd = open("file.txt", O_RDWR);
    printf("file descriptor: %d\n", fd);
    if (fd == -1)
    {
        perror("Error with file");
        exit(1);
    }

    ssize_t bytes_read;
    char buffer[10];

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("%d bytes read \n%s\n", (int)bytes_read, buffer);
    }

    if (bytes_read == -1)
    {
        perror("error reading");
    }

    close(fd);
    return 0;
}
