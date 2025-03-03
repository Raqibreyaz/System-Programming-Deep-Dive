#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFER_SIZE 20
int main(int argc, char const *argv[])
{
    char buffer[BUFFER_SIZE + 1];

    // since read requires file descriptor so giving it fd of stdin
    ssize_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
    buffer[BUFFER_SIZE] = '\0';
    printf("%s got from user input\n", buffer);
    return 0;
}
