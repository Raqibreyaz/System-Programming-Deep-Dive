#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;
pthread_mutex_t lock;

void *incrementCounter(void *arg)
{
    // lock the resource so only 1 resource can modify
    pthread_mutex_lock(&lock);

    // increase the counter
    for (int i = 0; i < 10; i++)
    {
        counter++;
        printf("Counter: %d\n", counter);
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

int main()
{
    // 2 threads, each has assigned task to increase the counter
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL); // initialize mutex

    pthread_create(&t1, NULL, incrementCounter, NULL);
    pthread_create(&t2, NULL, incrementCounter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock); // cleanup mutex
    return 0;
}