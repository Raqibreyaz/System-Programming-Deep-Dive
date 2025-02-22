#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
int main(int argc, char const *argv[])
{
    int fd = open("./file.txt", O_RDONLY);
    printf("opened file descriptor %d \n", fd);

    char buffer[100] = "hello world";

    write(fd, buffer, sizeof(buffer));

    printf("%s\n", buffer);

    exit(0);
}
