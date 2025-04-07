#include <stdio.h>
#include <pthread.h>

#define CONSUMERS_COUNT 5
#define PRODUCERS_COUNT 5

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// a shared variable
static int glob = 0;

void *producer(void *arg)
{
    int threadNo = *((int *)arg);

    // locking first so that no other thread will interfare
    pthread_mutex_lock(&mtx);

    // increment glob
    glob++;

    printf("global var incremented to: %d by thread %d\n", glob, threadNo);

    // unlock the mutex to allow other threads
    pthread_mutex_unlock(&mtx);

    // signal the waiting threads to wake up
    pthread_cond_signal(&cond);
    return NULL;
}

void *consumer(void *arg)
{
    int threadNo = *((int *)arg);

    // locking mutex first so that we get original value of glob
    pthread_mutex_lock(&mtx);

    // wait for glob availability
    // lock will be freed on waiting
    while (glob == 0)
    {
        printf("waiting for global increment\n");
        pthread_cond_wait(&cond, &mtx);
    }

    // decrease glob when available
    glob--;

    printf("global var consumed to: %d by thread %d\n", glob, threadNo);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t c[CONSUMERS_COUNT], p[PRODUCERS_COUNT];

    int threadsCount = CONSUMERS_COUNT > PRODUCERS_COUNT ? CONSUMERS_COUNT : PRODUCERS_COUNT;

    int threadNumber[threadsCount];

    for (size_t i = 0; i < threadsCount; i++)
    {
        threadNumber[i] = i + 1;
    }

    // creating threads
    for (size_t i = 0; i < CONSUMERS_COUNT || i < PRODUCERS_COUNT; i++)
    {
        if (i < CONSUMERS_COUNT)
        {
            pthread_create(&c[i], NULL, consumer, &threadNumber[i]);
        }

        if (i < PRODUCERS_COUNT)
        {
            pthread_create(&p[i], NULL, producer, &threadNumber[i]);
        }
    }

    // joining threads
    for (size_t i = 0; i < CONSUMERS_COUNT || i < PRODUCERS_COUNT; i++)
    {
        if (i < CONSUMERS_COUNT)
        {
            pthread_join(c[i], NULL);
        }

        if (i < PRODUCERS_COUNT)
        {
            pthread_join(p[i], NULL);
        }
    }

    return 0;
}
