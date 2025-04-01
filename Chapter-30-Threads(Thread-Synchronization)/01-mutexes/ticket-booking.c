#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SEATS_COUNT 10
#define USERS_COUNT 10

// seats of the bus
static int seats[SEATS_COUNT];

// will point on the free seat available
static int freeSpot = -1;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *assignSeat(void *arg)
{
    int userNo = *((int *)arg);

    // locking so no other user will book this
    pthread_mutex_lock(&mtx);

    // first check if there is a free seat available
    if (freeSpot < SEATS_COUNT - 1)
    {
        // assign the seat to that user
        seats[++freeSpot] = userNo;
        printf("seat %d is assigned to user %d\n", freeSpot + 1, userNo);
        sleep(1);
    }

    // unlock to allow other user to book seatsz`
    pthread_mutex_unlock(&mtx);

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t users[USERS_COUNT];
    int usersNo[USERS_COUNT] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int user = 0; user < USERS_COUNT; user++)
    {
        pthread_create(&users[user], NULL, assignSeat, &usersNo[user]);
    }

    for (int user = 0; user < USERS_COUNT; user++)
    {
        pthread_join(users[user], NULL);
    }

    return 0;
}
