#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
int main(int argc, char const *argv[])
{
    int fd = open("file.txt", O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC, S_IRUSR, S_IWUSR);
    write(fd, "chacha", 6);
    off_t fileLength = lseek(fd, 0, SEEK_END);
    printf("file length: %ld\n", fileLength);

    // maing it a sparse file by allocating blank
    lseek(fd, 1024 * 1024, SEEK_CUR);
    write(fd, "chacha", 6);
    return 0;
}
