#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Program Name: %s\n", argv[0]);
    if (argc > 1)
    {
        printf("Additional Arguments:\n");
        for (int i = 1; i < argc; i++)
        {
            printf("  argv[%d]: %s\n", i, argv[i]);
        }
    }
    else
    {
        printf("No additional arguments were provided.\n");
    }
    return 0;
}