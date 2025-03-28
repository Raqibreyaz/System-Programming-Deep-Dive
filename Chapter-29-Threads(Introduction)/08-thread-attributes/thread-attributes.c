#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *threadedFunction(void *)
{
    printf("running threaded function\n");
    sleep(2);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t thread;
    pthread_attr_t attr;

    // initialize thread attributes
    pthread_attr_init(&attr);

    // assigning detach state
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // create the thread using the attributes
    pthread_create(&thread, &attr, threadedFunction, NULL);

    // destroy the attributes after use
    pthread_attr_destroy(&attr);

    printf("Main thread continues\n");
    
    // will wait for completion of the thread
    sleep(3);
    
    printf("Main thread exits\n");
    return 0;
}
