#include <stdio.h>
#include <fcntl.h>
int main(int argc, char const *argv[])
{
    int fd = open("./file.txt", O_WRONLY);

    // take the file flag
    int flags = fcntl(fd, F_GETFL);

    // updating the file flag
    int new_flags = flags | O_APPEND;
    fcntl(fd, F_SETFL, new_flags);

    printf("old flags: %d, new flags: %d\n", flags, new_flags);

    // create a lock variable for locking the file
    struct flock lock =
        {
            .l_type = F_WRLCK,
            .l_whence = SEEK_SET,
            .l_start = 0,
            .l_len = 0};

    // checking if locking the file is available
    fcntl(fd, F_GETLK, &lock);

    short fl = lock.l_type;

    // checking modifies l-type so re initialize it
    lock.l_type = F_WRLCK;

    if (fl == F_UNLCK)
    {
        // locking file(blocking)
        fcntl(fd, F_SETLK, &lock);
    }
    else
    {
        // lock file when available(non-blocking)
        fcntl(fd, F_SETLKW, &lock);
    }

    return 0;
}
