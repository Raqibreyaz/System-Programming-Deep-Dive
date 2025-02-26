#include <stdio.h>
#include <sys/types.h>

struct sembuf
{
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};

int main(int argc, char const *argv[])
{
    __intmax_t maxval = 4566;
    pid_t t = 5;
    struct sembuf x = {.sem_num = 32, .sem_op = 3, .sem_flg = 1};
    printf("size of object is %zd\n",sizeof(x));
    return 0;
}
