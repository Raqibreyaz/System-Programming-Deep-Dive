#include <stdio.h>
#include <pthread.h>

static int glob = 0;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *threadFunc(void *)
{
    pthread_mutex_lock(&mtx);
    for (int i = 0; i < 20000; i++)
    {
        glob++;
    }
    pthread_mutex_unlock(&mtx);

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t thread1, thread2;

    // assign task to thread1
    pthread_create(&thread1, NULL, threadFunc, NULL);

    // assign task to thread2
    pthread_create(&thread1, NULL, threadFunc, NULL);

    // wait for threads to finish

    printf("waiting for thread1\n");
    pthread_join(thread1, NULL);

    printf("waiting for thread2\n");
    pthread_join(thread2, NULL);

    printf("global-variable: %d\n", glob);

    return 0;
}
