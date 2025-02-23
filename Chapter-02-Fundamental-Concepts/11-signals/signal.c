#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Custom handler for SIGINT
void handle_sigint(int sig)
{
    printf("\nCaught signal %d (SIGINT). Ignoring it!\n", sig);
}

int main()
{
    // will catch the ctrl c signal and run the provided handler
    signal(SIGINT, handle_sigint); // Assign handler to SIGINT

    while (1)
    {
        printf("Running... Press Ctrl+C to try stopping me!\n");
        sleep(2);
    }

    return 0;
}
