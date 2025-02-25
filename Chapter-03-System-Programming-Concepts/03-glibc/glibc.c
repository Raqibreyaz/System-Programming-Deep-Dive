#include <stdio.h>
#include <gnu/libc-version.h>
int main(int argc, char const *argv[])
{
    printf("glibc version: %s \n", gnu_get_libc_version());

    #if __GLIBC__ >= 2 && __GLIBC_MINOR__ >= 12
        printf("glibc version is 2.12 or newer\n");
    #else
        printf("glibc version is older than 2.12\n");
    #endif

    return 0;
}
