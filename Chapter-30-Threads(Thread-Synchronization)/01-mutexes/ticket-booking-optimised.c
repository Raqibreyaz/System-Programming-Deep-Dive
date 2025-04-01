#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define SEATS_COUNT 30
#define NAME_SIZE 100
#define USERS_COUNT 4

struct SeatInfo
{
    char userName[NAME_SIZE]; // name of the user
    int booked;               // 0 free, 1 occupied
    pthread_mutex_t lock;     // lock for this specific seat
};

// keeping track of seats info
static struct SeatInfo seats[SEATS_COUNT];

void *assignSeat(void *arg)
{
    const char *userName = (char *)arg;

    for (size_t i = 0; i < SEATS_COUNT; i++)
    {
        // if seat is booked then go forward
        // lock so that if seat is available then no other user can simulaneousy book it
        if (seats[i].booked == 1 || pthread_mutex_trylock(&seats[i].lock) != 0)
        {
            printf("failed to assign seat %ld to %s \n", i + 1, userName);
            continue;
        }

        // add the name of the user and book the seat
        strncpy(seats[i].userName, userName, NAME_SIZE);
        seats[i].booked = 1;

        printf("seat %ld assigned to %s\n\n", i + 1, userName);

        // now unlock the lock
        pthread_mutex_unlock(&seats[i].lock);
        break;
    }
    return NULL;
}

int main(int argc, char const *argv[])
{

    // initializing the seats and freespots
    for (int i = 0; i < SEATS_COUNT; i++)
    {
        seats[i].booked = 0;
        memset(seats[i].userName, 0, NAME_SIZE);
        pthread_mutex_init(&(seats[i].lock), NULL);
    }

    const char *usersName[USERS_COUNT] = {"Raquib", "Amaan", "Mama", "Nafiz"};
    pthread_t users[USERS_COUNT];

    for (size_t i = 0; i < USERS_COUNT; i++)
    {
        pthread_create(&users[i], NULL, assignSeat, (void *)usersName[i]);
    }

    for (int i = 0; i < USERS_COUNT; i++)
    {
        pthread_join(users[i], NULL);
    }
    return 0;
}
