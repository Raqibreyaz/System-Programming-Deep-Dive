#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int simple_function() { return 42; } // Function defined outside main()

int main()
{
    struct timespec start, end;
    long long ns;

    // Measure system call (getpid)
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < 1000000; i++)
        getpid(); // System call
    clock_gettime(CLOCK_MONOTONIC, &end);
    ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("System Call Time: %lld ns\n", ns);

    // Measure function call
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < 1000000; i++)
        simple_function(); // Normal function call
    clock_gettime(CLOCK_MONOTONIC, &end);
    ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("Function Call Time: %lld ns\n", ns);

    return 0;
}
