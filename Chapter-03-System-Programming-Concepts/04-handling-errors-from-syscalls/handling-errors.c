#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    int fd = open("./test1.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error code: %d\n", errno);
        // printf("Error encountered: %s",str)
        perror("Error in opening file");
    }
    else
    {
        printf("File successfully opened\n");
        close(fd);
    }
    return 0;
}
