### Statically Allocated Mutexes

Mutexes are essential for ensuring mutual exclusion when multiple threads access shared resources. They can be **statically allocated** (declared as global or local static variables) or **dynamically allocated** (using `malloc`). Statically allocated mutexes are simpler to use and require less overhead.

#### Declaring and Initializing a Statically Allocated Mutex  
A statically allocated mutex is a variable of type `pthread_mutex_t`. Before using it, it **must be initialized**. For a static mutex, this can be done using the special initializer **`PTHREAD_MUTEX_INITIALIZER`**, as shown below:

```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // Static mutex initialization

int shared_data = 0; // Shared resource

void *threadFunc(void *arg) {
    pthread_mutex_lock(&mtx); // Lock the mutex before modifying shared resource
    shared_data++;
    printf("Thread %ld incremented shared_data to %d\n", (long)arg, shared_data);
    pthread_mutex_unlock(&mtx); // Unlock the mutex after modification
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, threadFunc, (void *)1);
    pthread_create(&t2, NULL, threadFunc, (void *)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value of shared_data: %d\n", shared_data);
    return 0;
}
```

### Key Points:
1. **Static Initialization:**  
   - A mutex can be initialized **statically** using `PTHREAD_MUTEX_INITIALIZER`.
   - This method is **only valid for statically allocated mutexes** (global, static, or file-scope variables).
   
2. **Undefined Behavior with Mutex Copies:**  
   - The mutex should **only** be used on the **original** instance.
   - Copying and using a mutex (e.g., passing it by value) leads to **undefined behavior**.

3. **Mutex Operations:**  
   - `pthread_mutex_lock(&mtx)`: Acquires the lock.
   - `pthread_mutex_unlock(&mtx)`: Releases the lock.

In cases where a mutex needs to be created at runtime, **dynamic mutex initialization** using `pthread_mutex_init()` is required, which will be covered separately.