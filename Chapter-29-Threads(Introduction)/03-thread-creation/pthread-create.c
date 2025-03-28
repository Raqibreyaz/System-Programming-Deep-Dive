#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    int x;
    int y;
} args;

void *threadFunction(void *arg)
{
    args *argv = (args *)arg;

    printf("x: %d\ny: %d\n", argv->x, argv->y);

    printf("this is a thread function\n");

    sleep(4);

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t thread;
    args arg = {.x = 3, .y = 5};

    int status = pthread_create(&thread, NULL, threadFunction, &arg);
    if (status != 0)
    {
        fprintf(stderr, "failed to create new thread\n");
        return 1;
    }

    pthread_join(thread, NULL);

    return 0;
}
