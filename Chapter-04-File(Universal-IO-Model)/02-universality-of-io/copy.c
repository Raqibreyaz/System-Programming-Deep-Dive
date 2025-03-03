#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

// this program will copy data of one file to another file
int main(int argc, char const *argv[])
{
    // file1 and file2
    const char *file1, *file2;

    if (argc > 1)
    {
        file1 = argv[1];
    }
    if (argc > 2)
    {
        file2 = argv[2];
    }

    char buffer[100];

    int fd1 = open(file1, O_RDONLY);

    if (fd1 == -1)
    {
        perror("Error in file1");
        exit(1);
    }

    int fd2 = open(file2, O_WRONLY);

    if (fd2 == -1)
    {
        perror("Error in file2");
        exit(1);
    }

    ssize_t bytes_read;

    while ((bytes_read = read(fd1, buffer, sizeof(buffer))) > 0)
    {
        ssize_t bytes_written = write(fd2, buffer, bytes_read);
        printf("%d are written\n", (int)bytes_written);
    }

    return 0;
}
