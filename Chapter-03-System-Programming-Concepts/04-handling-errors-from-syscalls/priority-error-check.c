#include <stdio.h>
#include <errno.h>
#include <sys/resource.h>
int main()
{
    errno = 0; // Reset errno before function call
    int priority = getpriority(PRIO_PROCESS, 0);

    if (priority == -1 && errno != 0)
    { // Check if error occurred
        perror("getpriority failed");
        return 1;
    }

    printf("Process priority: %d\n", priority);
    return 0;
}