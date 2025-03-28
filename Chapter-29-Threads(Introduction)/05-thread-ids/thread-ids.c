#include <stdio.h>
#include <pthread.h>

void *printThreadId(void *arg)
{
    pthread_t *tId = (pthread_t *)arg;

    *tId = pthread_self();

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t1, t2;

    pthread_t t1Id, t2Id;

    pthread_create(&t1, NULL, printThreadId, &t1Id);
    pthread_create(&t2, NULL, printThreadId, &t2Id);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    if (pthread_equal(t1, t2) != 0)
    {
        printf("both ids are same\n");
    }
    else
    {
        printf("both ids are different\n");
    }

    printf("%d\n", t1 != t2);

    return 0;
}
