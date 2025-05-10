#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = inotify_init();

    if (fd == -1)
    {
        perror("inotify_init");
        return 1;
    }

    int wd = inotify_add_watch(fd, "./check.txt", IN_MODIFY);

    if (wd == -1)
    {
        perror("inotify_add_watch");
        return 1;
    }

    char buffer[4096];

    while (1)
    {
        printf("waiting for file changes...\n");
        size_t bytes_read = read(fd, buffer, sizeof(buffer));
        if (bytes_read == -1)
        {
            perror("read");
            return 1;
        }

        char *ptr = buffer;
        struct inotify_event *event = NULL;
        while (ptr < buffer + bytes_read)
        {
            // extracting the event
            event = (struct inotify_event *)ptr;
            printf("wd=%d\ncookie=%x\nlen=%x\nmask=%x\nname=%s\n\n", event->wd, event->cookie, event->len, event->mask, event->name);

            // pointing to next event
            ptr = ptr + sizeof(struct inotify_event) + event->len;
        }
    }
    return 0;
}
