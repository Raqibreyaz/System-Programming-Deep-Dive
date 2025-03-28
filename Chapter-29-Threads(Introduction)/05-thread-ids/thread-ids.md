## **What is a Thread ID (TID)?**
Each thread inside a process is uniquely identified by a **thread ID**. This ID is assigned when the thread is created using `pthread_create()`, and a thread can obtain its **own** ID using `pthread_self()`.

### **Think of it like this:**
- **Processes** are like different **teams** in a company.
- **Threads** inside a process are like **employees** working on different tasks **within the same team**.
- **Thread IDs** are like **employee ID numbers**—each thread gets a unique number within its process.

---
## **How to Get a Thread's ID?**
Threads can obtain their own ID using:

```c
#include <pthread.h>
pthread_t pthread_self(void);
```
This function returns the **ID of the calling thread**.

### **Example: Getting and Printing Thread IDs**
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg) {
    printf("Thread ID: %lu\n", pthread_self());
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
```
### **Output (example, actual IDs may vary):**
```
Thread ID: 139734722869056
Thread ID: 139734714476352
```
- `pthread_self()` returns the **thread's unique ID**.
- **Each thread** gets a different **ID**.

---
## **Why Are Thread IDs Useful?**
### **1️⃣ Identifying and Managing Threads**
Some `pthread` functions need **thread IDs** to specify which thread to operate on:
- **`pthread_join(tid)`** → Waits for a specific thread to finish.
- **`pthread_cancel(tid)`** → Cancels a specific thread.
- **`pthread_kill(tid, signal)`** → Sends a signal to a specific thread.

---

### **2️⃣ Tagging Data with Thread ID**
You can **mark** data with a thread ID to track which thread created or modified it.

#### **Example: Tagging Data with Thread ID**
```c
#include <pthread.h>
#include <stdio.h>

typedef struct {
    pthread_t owner;
    int value;
} Data;

void* thread_function(void* arg) {
    Data* data = (Data*)arg;
    data->owner = pthread_self();  // Assign thread ID to the data
    data->value = 42;
    printf("Thread %lu processed data\n", data->owner);
    return NULL;
}

int main() {
    pthread_t thread;
    Data data;

    pthread_create(&thread, NULL, thread_function, &data);
    pthread_join(thread, NULL);

    printf("Data processed by thread: %lu, Value: %d\n", data.owner, data.value);
    return 0;
}
```
### **Output (example):**
```
Thread 139734722869056 processed data
Data processed by thread: 139734722869056, Value: 42
```
- The **thread ID** is stored in the `Data` structure.
- This helps track **which thread** modified the data.

---
## **Comparing Two Thread IDs**
We use `pthread_equal()` to **compare** two thread IDs.

```c
#include <pthread.h>
int pthread_equal(pthread_t t1, pthread_t t2);
```
- **Returns `nonzero` (true)** if `t1` and `t2` are the same thread.
- **Returns `0` (false)** if they are different.

### **Example: Checking If Two Threads Are the Same**
```c
#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg) {
    pthread_t tid = pthread_self();
    
    if (pthread_equal(tid, *(pthread_t*)arg)) {
        printf("Thread %lu matches argument thread ID.\n", tid);
    } else {
        printf("Thread %lu does NOT match argument thread ID.\n", tid);
    }

    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, &thread);
    pthread_join(thread, NULL);

    return 0;
}
```
### **Output:**
```
Thread 139734722869056 matches argument thread ID.
```
- `pthread_equal()` **compares** `pthread_self()` with `thread`.
- If they are **the same**, it prints a match.

---
## **Can We Print a Thread ID Directly?**
### ❌ **Wrong Way**
```c
pthread_t thr;
printf("Thread ID = %ld\n", (long) thr); // ❌ Not portable
```
- `pthread_t` **may not always be an integer** (it could be a pointer or structure).
- This code **may fail** on some systems.

### ✅ **Correct Way**
```c
printf("Thread ID = %lu\n", pthread_self());
```
---
## **Are Thread IDs Unique Across Processes?**
- In **Linux**, **each thread has a unique ID** across all processes.
- On **other systems**, **thread IDs might only be unique inside a single process**.
- **Thread IDs can be reused** after a thread has terminated and been joined.

---
## **Difference Between `pthread_t` and `gettid()`**
| Function | Description |
|----------|------------|
| `pthread_self()` | Returns the **POSIX thread ID** (managed by the threading library). |
| `gettid()` | Returns the **kernel thread ID** (managed by the Linux kernel). |

### **Example: Comparing Both IDs**
```c
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>  // Required for syscall(SYS_gettid)

void* thread_function(void* arg) {
    printf("POSIX Thread ID: %lu, Kernel Thread ID: %ld\n",
           pthread_self(), syscall(SYS_gettid));
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);

    return 0;
}
```
### **Output (example):**
```
POSIX Thread ID: 139734722869056, Kernel Thread ID: 12345
```
- `pthread_self()` → Returns **POSIX thread ID**.
- `gettid()` → Returns **Linux kernel thread ID** (which is like a process ID for threads).

---
## **Key Takeaways**
✅ **Thread IDs (`pthread_t`) uniquely identify threads inside a process.**  
✅ **Use `pthread_self()` to get the current thread's ID.**  
✅ **Use `pthread_equal(t1, t2)` to compare thread IDs instead of `==`.**  
✅ **Thread IDs can be reused after a thread exits and is joined.**  
✅ **POSIX thread IDs (`pthread_t`) are different from Linux kernel thread IDs (`gettid()`).**  
