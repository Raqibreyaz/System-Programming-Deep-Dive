### Locking and Unlocking a Mutex in Pthreads

After initializing a mutex, it is in an **unlocked** state. To synchronize access to shared resources, threads must **lock** and **unlock** the mutex.

---

### **1. Locking a Mutex (`pthread_mutex_lock`)**
```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
```
- If the mutex is **unlocked**, the function **locks** it and returns immediately.
- If the mutex is **already locked** by another thread, the calling thread **blocks** (waits) until the mutex is unlocked.
- If the calling thread **already owns** the mutex (for the default mutex type), two possible behaviors occur:
  - The thread **deadlocks** (blocks indefinitely).
  - The function returns the error **`EDEADLK`**.
- On success, the function returns `0`. On failure, it returns a positive error code.

---

### **2. Unlocking a Mutex (`pthread_mutex_unlock`)**
```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```
- Unlocks a mutex **previously locked by the calling thread**.
- If another thread is **waiting** for the mutex, it is granted ownership.
- **Errors:**
  - If the mutex is **not locked**, unlocking it causes **undefined behavior**.
  - If a thread tries to unlock a mutex that it **does not own**, the behavior is undefined.

---

### **3. Example: Using a Mutex for Safe Global Variable Access**
This program ensures that multiple threads safely increment a global variable using a mutex.

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_LOOPS 10000000

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *threadFunc(void *arg) {
    int loops = *((int *) arg);
    int j, s;

    for (j = 0; j < loops; j++) {
        s = pthread_mutex_lock(&mtx);
        if (s != 0) {
            perror("pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }

        glob++; // Critical section

        s = pthread_mutex_unlock(&mtx);
        if (s != 0) {
            perror("pthread_mutex_unlock");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    int loops = (argc > 1) ? atoi(argv[1]) : NUM_LOOPS;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value of glob: %d\n", glob);
    return 0;
}
```
#### **Output Example (Reliable Result)**
```
Final value of glob: 20000000
```
- Without a mutex, the result would be **unpredictable** due to **race conditions**.

---

## **4. Variants of `pthread_mutex_lock`**
### **a) `pthread_mutex_trylock()`**
```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```
- Tries to lock the mutex **without blocking**.
- If the mutex is already locked, it **fails immediately**, returning `EBUSY`.

#### **Example: Using `pthread_mutex_trylock()`**
```c
if (pthread_mutex_trylock(&mtx) == 0) {
    // Mutex acquired successfully
    glob++;
    pthread_mutex_unlock(&mtx);
} else {
    // Mutex was already locked
    printf("Mutex is already locked\n");
}
```

---

### **b) `pthread_mutex_timedlock()`**
```c
int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abstime);
```
- Waits to acquire the mutex **until a specified timeout**.
- If the timeout expires before acquiring the lock, it returns `ETIMEDOUT`.

#### **Example: Using `pthread_mutex_timedlock()`**
```c
#include <time.h>
struct timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);
ts.tv_sec += 2; // Set timeout for 2 seconds

if (pthread_mutex_timedlock(&mtx, &ts) == 0) {
    // Mutex acquired
    glob++;
    pthread_mutex_unlock(&mtx);
} else {
    printf("Timed out while waiting for the mutex\n");
}
```

---

### **5. When to Use These Mutex Functions**
| Function | Behavior |
|----------|-----------|
| `pthread_mutex_lock()` | Blocks until the mutex is available. |
| `pthread_mutex_trylock()` | Returns immediately; fails if mutex is locked. |
| `pthread_mutex_timedlock()` | Waits for a specified time before failing. |

---

## **6. Key Takeaways**
✅ **Always lock a mutex before modifying shared data.**  
✅ **Always unlock a mutex after modification to avoid deadlocks.**  
✅ **Using `pthread_mutex_trylock()` can help prevent blocking.**  
✅ **Use `pthread_mutex_timedlock()` if you need a timeout mechanism.**  
✅ **Never unlock a mutex you don’t own—it leads to undefined behavior.**  

Mutexes **ensure thread synchronization**, making multi-threaded programs safer and more predictable. 🚀