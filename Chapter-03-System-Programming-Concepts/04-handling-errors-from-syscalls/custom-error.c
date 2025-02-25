#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    // initializing with access denied code
    errno = EACCES;
    // printing the error corresponding to the code
    printf("Error: %s\n", strerror(errno));
    return 0;
}
