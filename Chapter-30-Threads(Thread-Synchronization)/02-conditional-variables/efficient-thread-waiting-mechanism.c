#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 10

enum threadState
{
    T_TERMINATED, // when thread completed execution
    T_ALIVE,      // when thread is under execution
    T_JOINED      // when thread is joined after termination
};

struct thread
{
    pthread_t id;
    enum threadState state;
    int sleepTime;
    int threadNo;
};

static pthread_cond_t threadDead = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static struct thread threads[THREAD_COUNT];

void *threadFunction(void *arg)
{
    // extract the thread no.
    int threadNo = *((int *)arg);

    // simulate thread working
    sleep(threads[threadNo].sleepTime);

    // lock so that no other thread pollute cond var
    pthread_mutex_lock(&mtx);

    // change state of the thread to termination
    threads[threadNo].state = T_TERMINATED;

    printf("%dth thread terminated\n", threadNo);

    // signal main thread to join this thread
    pthread_cond_signal(&threadDead);
    pthread_mutex_unlock(&mtx);

    return NULL;
}

int main(int argc, char const *argv[])
{

    int aliveThreads = THREAD_COUNT;

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        // assign a random sleep time
        threads[i].sleepTime = rand() % 4 + 1;

        // thread is not terminated yet
        threads[i].state = T_ALIVE;

        threads[i].threadNo = i;

        // start the thread
        pthread_create(&(threads[i].id), NULL, threadFunction, &(threads[i].threadNo));
    }

    // locking so that no other thread terminate during this time
    pthread_mutex_lock(&mtx);

    // wait for each thread until no thread remains
    while (aliveThreads > 0)
    {
        // lock will be freed on waiting so that threads can terminate
        pthread_cond_wait(&threadDead, &mtx);

        // here means a thread is terminated
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            // if the thread is terminated then join it
            if (threads[i].state == T_TERMINATED)
            {
                printf("%dth thread joined\n", i);
                threads[i].state = T_JOINED;
                pthread_join(threads[i].id, NULL);
                aliveThreads--;
            }
        }
    }
    
    // unlock to allow other threads terminate
pthread_mutex_unlock(&mtx);

    return 0;
}
