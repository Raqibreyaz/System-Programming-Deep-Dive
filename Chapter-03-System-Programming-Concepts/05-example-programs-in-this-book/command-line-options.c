#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_file(const char *filename, int showLineNumbers)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[256];
    int lineNum = 1;
    while (fgets(line, sizeof(line), fp))
    {
        if (showLineNumbers)
        {
            printf("%d: %s", lineNum++, line);
        }
        else
        {
            printf("%s", line);
        }
    }
    fclose(fp);
}

int main(int argc, char * const argv[])
{
    // int opt = 0;
    // int showLineNumbers = 0;
    // // loop in the options to get opt
    // while ((opt = getopt(argc, argv, "l")) != -1)
    // {
    //     switch (opt)
    //     {
    //         // when l is given then will show line numbers
    //     case 'l':
    //         showLineNumbers = 1;
    //         break;

    //     default:
    //         fprintf(stderr, "Usage: %s [-l] filename\n", argv[0]);
    //         exit(EXIT_FAILURE);
    //     }
    // }
    // if (optind >= argc)
    // {
    //     fprintf(stderr, "Expected filename after options\n");
    //     exit(EXIT_FAILURE);
    // }

    // print_file(argv[optind], showLineNumbers);
    for (size_t i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }

    int opt = getopt(argc, argv, "l");
    printf("%d\n", opt);

    return 0;
}
