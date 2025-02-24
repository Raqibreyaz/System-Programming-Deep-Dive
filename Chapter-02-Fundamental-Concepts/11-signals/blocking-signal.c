#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    sigset_t new_set, old_set;

    // Initialize the set and add SIGINT to it
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);

    // Block SIGINT
    sigprocmask(SIG_BLOCK, &new_set, &old_set);
    printf("SIGINT is now blocked. Try pressing Ctrl + C... Nothing happens!\n");

    // Sleep for 5 seconds while SIGINT is blocked
    sleep(5);

    // Unblock SIGINT
    sigprocmask(SIG_SETMASK, &old_set, NULL);
    printf("SIGINT is now unblocked! Press Ctrl + C now.\n");

    // Wait for user input to keep the program running
    while (1)
    {
        sleep(1);
    }

    return 0;
}
