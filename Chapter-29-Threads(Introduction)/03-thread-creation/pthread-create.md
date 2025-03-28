## **Thread Creation in Pthreads API**

### Introduction
When a program starts, it runs as a single-threaded process, where this initial thread is referred to as the **main thread**. However, to achieve concurrency, additional threads can be created within the process. This section explains how to create threads using the `pthread_create()` function in the **Pthreads API**.

### `pthread_create()` Function
The `pthread_create()` function is used to create a new thread within a process. Here’s the function signature:

```c
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start)(void *), void *arg);
```

- **Returns**: `0` on success; otherwise, returns a positive error number on failure.
- **Parameters**:
  - `pthread_t *thread`: Pointer to a buffer where the thread’s unique identifier will be stored.
  - `pthread_attr_t *attr`: Pointer to a thread attribute object (optional, `NULL` for default attributes).
  - `void *(*start)(void *)`: Function pointer to the start routine executed by the new thread.
  - `void *arg`: Argument passed to the start routine (optional, can be `NULL`).

### How It Works
- The new thread begins execution by calling the function specified in `start`, passing `arg` as an argument.
- The calling thread (main thread or any other existing thread) **continues execution without waiting**.
- The newly created thread can perform its designated task independently.

### Example Usage
Here’s a simple example of creating a thread:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *threadFunction(void *arg) {
    printf("Hello from new thread!\n");
    return NULL;
}

int main() {
    pthread_t thread;
    int s = pthread_create(&thread, NULL, threadFunction, NULL);
    if (s != 0) {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }
    pthread_join(thread, NULL);
    return 0;
}
```

### Key Details
1. **Passing Arguments**:
   - The `arg` parameter is of type `void *`, allowing it to pass any data type.
   - If multiple arguments are needed, use a **struct** to bundle them together.

2. **Returning Values**:
   - The `start` function should return a `void *`.
   - This return value can be retrieved by another thread using `pthread_join()`.

3. **Race Conditions & Scheduling**:
   - Once `pthread_create()` is called, execution order **is not guaranteed**.
   - On a multi-core processor, both threads **can run in parallel**.
   - On a single-core system, **thread scheduling is determined by the OS**.

### Special Considerations
- **Thread ID Management**:
  - The `thread` argument stores a unique thread ID.
  - A thread can obtain its own ID using `pthread_self()`.

- **Thread Synchronization**:
  - If order of execution matters, synchronization techniques (e.g., **mutexes**, **condition variables**) must be used.

- **Thread Attributes**:
  - The `attr` argument can be used to set thread attributes (e.g., **detached state**, **stack size**).
  - Default attributes are used if `attr` is `NULL`.

### Potential Pitfalls
1. **Returning Integer Values**:
   - Be careful when returning integer values directly cast to `void *`.
   - `PTHREAD_CANCELED` is often defined as an integer cast to `void *`, so conflicts may arise.

2. **Initialization Order**:
   - The `thread` buffer may not be initialized before the new thread starts execution.
   - Use `pthread_self()` if the new thread needs to obtain its own ID.

3. **Error Handling**:
   - Instead of assigning `errno` directly, Pthreads functions return an **error code**, which should be checked explicitly.

### Conclusion
Thread creation using `pthread_create()` is an essential part of multi-threaded programming in C. Proper argument handling, synchronization, and careful memory management are necessary to avoid race conditions and undefined behavior. When used correctly, **Pthreads allow efficient concurrent execution, improving performance in multi-core systems**.

