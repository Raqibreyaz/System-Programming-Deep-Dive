# 🔹 **Understanding Pthreads API (POSIX Threads) – A Beginner-Friendly Breakdown**  

## 🟢 **Background of Pthreads**  
Before 1995, many different threading APIs existed, making it hard to write **portable** (cross-platform) multi-threaded programs. To fix this, **POSIX.1c (1995)** standardized the **Pthreads API**. Later, it was merged into **SUSv3 (Single UNIX Specification, Version 3)**.  

🔹 **Key takeaway**: **Pthreads provide a standardized way to use threads in UNIX-like systems.**  

---

## 🔹 **Pthreads Data Types (Opaque Data Types Explained!)**  

Pthreads define several **special data types** that handle threading-related tasks. These types are **opaque**, meaning you can use them but **shouldn’t** directly inspect or modify their internal structure.  

📌 **Why opaque?**  
Imagine Pthreads data types as a **sealed box** 📦 that you interact with through predefined functions. You **don’t** need to know what’s inside; you just use the provided interface.  

Here are the main Pthreads data types:  

| **Data Type**         | **Meaning** |
|----------------------|------------|
| `pthread_t`         | Thread identifier |
| `pthread_mutex_t`   | Mutex (for thread synchronization) |
| `pthread_mutexattr_t` | Mutex attributes (controls behavior of a mutex) |
| `pthread_cond_t`    | Condition variable (used for signaling between threads) |
| `pthread_condattr_t` | Condition variable attributes |
| `pthread_key_t`     | Thread-specific data key |
| `pthread_once_t`    | Ensures one-time initialization |
| `pthread_attr_t`    | Thread attributes (defines properties of a thread) |

🔹 **Key takeaway**: You **can’t** use `==` to compare Pthread data types because their contents are hidden (opaque). Instead, use **Pthreads functions** to manipulate them.

---

## 🔹 **Threads and `errno` – Why Does Each Thread Need Its Own?**  

In traditional UNIX programs, `errno` is a **global variable** that stores error codes for system calls. But in a multi-threaded program, if all threads shared the same `errno`, it would cause **race conditions**—one thread could overwrite `errno` before another thread reads it!  

### ✅ **Solution: Thread-Specific `errno`**
Each thread gets **its own version of `errno`**, ensuring error codes remain consistent **per thread**.  

📌 **How is this achieved?**  
Instead of being a **true global variable**, `errno` is actually a **macro** that expands into a **function call** returning a thread-specific version of `errno`.  

### 🧐 **What Does "Macro Expands to a Function Call" Mean?**  
Instead of directly accessing `errno`, your code **calls a function that fetches the correct version of `errno`** for the current thread.  

👉 **Example:**
```c
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

void *thread_func(void *arg) {
    errno = 0;  // Each thread gets its own errno
    printf("Thread %ld has errno address: %p\n", pthread_self(), &errno);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    return 0;
}
```
🔹 **What happens?**  
Each thread prints a **different address** for `errno`, proving that each thread has its **own** `errno`.

🔹 **Key takeaway**: In a multi-threaded program, **each thread gets its own private `errno`**, preventing race conditions.

---

## 🔹 **Return Values in Pthreads API – Why Different From UNIX?**  
Traditional UNIX functions return:  
✅ `0` on success  
❌ `-1` on error and set `errno`  

However, **Pthreads functions** work differently:  
✅ `0` on success  
❌ **A positive error code** on failure  

👉 **Why?**  
Because `errno` is thread-specific and accessing it via a function call adds **overhead**, Pthreads avoid modifying `errno` directly.  

### ✅ **How to Handle Pthread Errors?**  
Instead of assigning the return value to `errno`, use an **intermediate variable**:  

```c
int s;
s = pthread_create(&thread, NULL, func, &arg);
if (s != 0)
    errExitEN(s, "pthread_create");
```
Here, `s` will hold the error code, and `errExitEN()` prints the corresponding error message.

🔹 **Key takeaway**: **Pthreads return positive error codes instead of setting `errno` to reduce function call overhead.**

---

## 🔹 **Understanding Reentrant Functions in Pthreads**  

### 🧐 **What Does "Reentrant" Mean?**  

A **reentrant function** is a function that **can be safely called by multiple threads at the same time** **without causing issues** like data corruption or race conditions.  

### ✅ **Key Features of a Reentrant Function:**  
1️⃣ **Does NOT use global or static variables**  
   - Instead, it uses **local variables (on the stack)** or **thread-local storage**.  
   
2️⃣ **Does NOT rely on shared resources without proper synchronization**  
   - If it modifies shared data, it uses **mutexes** or **other synchronization mechanisms** to avoid race conditions.  

3️⃣ **Can be paused and resumed without affecting other executions**  
   - If an **interrupt** or **signal** pauses the function and another thread calls it, both should execute correctly.  

---

### 🛑 **Why Non-Reentrant Functions Are Unsafe in Multi-Threading?**  

Non-reentrant functions use **global/static variables**, meaning if two threads call the function at the same time, they may **overwrite each other’s data**.  

### ❌ **Example of a Non-Reentrant Function:**  
```c
#include <stdio.h>
#include <string.h>

char *global_buffer;  // Global variable (not thread-safe!)

char *non_reentrant_function(const char *str) {
    global_buffer = (char *)malloc(strlen(str) + 1);
    if (global_buffer)
        strcpy(global_buffer, str);
    return global_buffer;
}
```
### 🔥 **Problem:**  
If **two threads** call this function at the same time, they will **overwrite `global_buffer`**, leading to **data corruption**.  

---

### ✅ **Example of a Reentrant Function:**  
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *reentrant_function(const char *str) {
    char *buffer = (char *)malloc(strlen(str) + 1);
    if (buffer)
        strcpy(buffer, str);
    return buffer;  // Safe because it returns a separate copy for each thread
}
```
### 🎯 **Why is this Safe?**  
- Uses **local variables (`buffer`) instead of global/static variables**  
- Each thread gets **its own separate memory allocation**  
- No shared data means **no race conditions!**  

---

## 🔹 **Why Pthreads Uses Reentrant Functions?**  

In a multi-threaded program, **multiple threads can run the same function at the same time**.  
If functions were **not reentrant**, threads could overwrite each other's data, causing **unpredictable behavior**.  

🔹 **Key takeaway:** Pthreads functions are designed to be **reentrant**, ensuring **safe multi-threaded execution**.  

---

To compile a program using Pthreads, use:  
```sh
gcc -pthread myprogram.c -o myprogram
```
This does **two** things:  
1️⃣ Defines `_REENTRANT` (enables reentrant function declarations).  
2️⃣ Links against the **Pthreads library** (`libpthread`).  

⚠️ Different OSes may use different options:  
- Linux: `-pthread`  
- Solaris/HP-UX: `-mt`  

🔹 **Key takeaway**: Always use `-pthread` when compiling Pthreads programs to enable threading support.

---

## 🔹 **Final Thoughts**  

- **Pthreads is a standardized threading API** for UNIX-like systems.
- **Pthreads types are opaque** (can’t inspect internals).
- **Each thread has its own `errno`**, preventing race conditions.
- **`errno` is a macro** that calls a function returning a thread-specific error variable.
- **Pthreads functions return error codes instead of modifying `errno`**.
- **Use `-pthread`** to compile multi-threaded programs correctly.