#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// data can be partially written so writing fully
void writtenFully(int fd, char *buffer, int buffer_size)
{
    int bytes_written = 0;
    do
    {
        bytes_written += (int)write(fd, buffer + bytes_written, buffer_size - bytes_written);
    } while (bytes_written < buffer_size);
}
int main(int argc, char const *argv[])
{
    char buffer[] = "hi bro this is a message i am  giving to print in the terminal";
    writtenFully(STDOUT_FILENO, buffer, sizeof(buffer));
    return 0;
}
