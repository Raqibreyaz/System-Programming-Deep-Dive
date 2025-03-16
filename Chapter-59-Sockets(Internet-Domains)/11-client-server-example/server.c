#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PORT 8000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 5
#define RESPONSE_SIZE 15

int extract_number(char *num, int len)
{
    int extracted_num = 0;
    int place_value = 1;
    printf("len: %d\n", len);

    for (int i = len - 1; i >= 0; i--)
    {
        int digit = (int)(num[i] - '0');
        printf("digit: %d\n", digit);
        extracted_num = (int)(digit * place_value + extracted_num);
        printf("extracted num: %d \n", extracted_num);
        place_value *= 10;
    }
    return extracted_num;
}

int convert_to_string(int start, int end, char *str, int max_size)
{
    return snprintf(str, max_size, "%d,%d", start, end);
}

int main(int argc, char const *argv[])
{
    int sfd;
    int counter = 0;
    char buffer[BUFFER_SIZE + 1];
    char response[RESPONSE_SIZE];
    ssize_t bytes_received, bytes_sent;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    memset(&server_addr, 0, addr_len);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // localhost

    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return 1;
    }

    if (bind(sfd, (struct sockaddr *)&server_addr, addr_len) == -1)
    {
        perror("bind");
        close(sfd);
        return 1;
    }

    if (listen(sfd, MAX_CLIENTS) == -1)
    {
        perror("listen");
        close(sfd);
        return 1;
    }

    while (1)
    {

        int cfd = accept(sfd, NULL, NULL);

        if ((bytes_received = recv(cfd, buffer, BUFFER_SIZE, 0)) == -1)
        {
            perror("recv");
            close(sfd);
            return 1;
        }
        buffer[bytes_received] = '\0';

        printf("received buffer %s\n", buffer);

        // extract the given range string as number
        int range = extract_number(buffer, bytes_received);

        printf("received range %d\n", range);

        // when requested amount of numbers are beyond limit then exit
        if (range > 10)
        {
            printf("large range %d is not allowed\n", range);
            close(sfd);
            return 1;
        }

        int start = counter + 1;
        int end = counter + range;
        counter += range;

        printf("unique sequence: [%d,%d]\n", start, end);

        // create a string using start and end
        int response_bytes = 0;

        if (range > 0)
        {
            response_bytes = convert_to_string(start, end, response, RESPONSE_SIZE);
        }
        else
        {
            strcpy(response, "");
        }

        printf("actual response size: %d\n", response_bytes);

        // sending the unique sequence range
        if ((bytes_sent = send(cfd, response, response_bytes, 0)) == -1)
        {
            perror("send");
            close(sfd);
            return 1;
        }

        printf("%d bytes sent\n", (int)bytes_sent);
    }
    close(sfd);

    return 0;
}
