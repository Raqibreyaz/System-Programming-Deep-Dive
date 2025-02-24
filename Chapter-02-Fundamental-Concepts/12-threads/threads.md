# **📌 Understanding Threads in UNIX/Linux**
A **thread** is like a **mini-process** that runs inside a program. Instead of having separate memory like full processes, threads **share memory** but have their own **stack**.

---

## **💡 How to Think About Threads?**
🔹 A **process** is like a **house**, with memory as rooms.  
🔹 **Threads** are **people** living inside the house. They share the space (global variables) but have their own personal areas (stacks).  

---

## **🔹 Key Features of Threads**
1. **Shared Memory** – All threads inside a process share the same memory space.  
2. **Independent Execution** – Each thread runs separately but can coordinate with others.  
3. **Own Stack** – Each thread has its own function call history and local variables.  
4. **Fast Communication** – Threads can quickly share data using global variables instead of slow IPC (Inter-Process Communication).  

---

## **📌 Why Use Threads? (Pros ✅ & Cons ❌)**
| **Feature**        | **Threads (✅ Good)** | **Processes (❌ Slower)** |
|------------------|----------------|----------------|
| **Memory Sharing**  | ✅ Share global memory | ❌ Separate memory |
| **Speed**          | ✅ Faster communication | ❌ Needs IPC (pipes, sockets) |
| **Parallelism**    | ✅ Runs tasks in parallel | ❌ Needs extra process creation |
| **Crash Impact**   | ❌ If one thread crashes, entire process crashes | ✅ One process crash doesn't affect others |

---

## **🔹 Example: Creating Threads in C (Using `pthread`)**
### **🔷 Simple Thread Example**
```c
#include <stdio.h>
#include <pthread.h>

// Function to be executed by the thread
void* print_message(void* arg) {
    printf("Hello from the thread!\n");
    return NULL;
}

int main() {
    pthread_t thread_id;  // Declare thread
    pthread_create(&thread_id, NULL, print_message, NULL);  // Create thread
    pthread_join(thread_id, NULL);  // Wait for thread to finish
    printf("Main program ends\n");
    return 0;
}
```
✅ **Output:**
```
Hello from the thread!
Main program ends
```
👉 `pthread_create()` starts a new thread  
👉 `pthread_join()` makes sure the main program waits for the thread  

---

## **🔹 Synchronization in Threads (Why Mutex & Condition Variables?)**
🔴 **Problem:** If multiple threads modify a shared variable, it can cause **race conditions** (data corruption).  
✅ **Solution:** Use **mutex (mutual exclusion)** to lock shared resources.

### **🔷 Example: Using Mutex to Prevent Data Corruption**
```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;  
pthread_mutex_t lock;  // Declare a mutex lock

void* increment_counter(void* arg) {
    pthread_mutex_lock(&lock);  // Lock the resource
    counter++;  // Critical section (only one thread can modify)
    printf("Counter: %d\n", counter);
    pthread_mutex_unlock(&lock);  // Unlock after updating
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);  // Initialize mutex
    
    pthread_create(&t1, NULL, increment_counter, NULL);
    pthread_create(&t2, NULL, increment_counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);  // Cleanup mutex
    return 0;
}
```
✅ **Why Mutex?**  
👉 Ensures only **one thread** modifies `counter` at a time.  
👉 Prevents **data corruption** due to simultaneous modifications.  

---

## **🔹 When Should You Use Threads vs. Processes?**
| **Use Threads When...** 🚀 | **Use Processes When...** 🏗️ |
|----------------------|----------------------|
| Need **shared memory access** | Need **separate memory** for security |
| Want **fast communication** | Need **independent failure handling** |
| Have **parallel tasks** on the same data | Want processes to run **completely separately** |

---

## **🎯 Summary (TL;DR)**
✅ **Threads are lightweight mini-processes** that share memory.  
✅ **Fast & efficient** for parallel execution but need **mutex** to avoid conflicts.  
✅ **Use `pthread`** for multi-threading in C.  
✅ **Threads crash together**, while **processes are independent**.  

---