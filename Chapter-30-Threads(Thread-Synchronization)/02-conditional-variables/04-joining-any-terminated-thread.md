# **Joining any Terminated Thread**

## **🔹 What Problem Does This Code Solve?**
Imagine you have **multiple workers** (threads) doing tasks.  
Each worker:  
1️⃣ Starts working (thread starts).  
2️⃣ Works for some time (sleeps).  
3️⃣ Finishes and reports back (terminates).  

👉 **Problem**:  
Normally, in C, you need to **manually join each thread one by one** using `pthread_join()`.  
But **you don’t know which thread will finish first**!  
This program **solves that problem** by **automatically waiting for and collecting** any finished thread.

---

# **🔹 How Does It Work?**
### **1️⃣ Create Threads**
- Each thread **sleeps** for a certain time (like doing work).  
- We track each thread’s **state** (running, finished, joined).  

### **2️⃣ Wait for Any Thread to Finish**
- The **main thread** waits for **any** thread to finish using a **condition variable**.  

### **3️⃣ Join Any Finished Thread**
- When a thread finishes, it **signals** the main thread.  
- The main thread then **joins** (collects) it.  

---

# **🔹 Code Walkthrough**
Now, let’s go through the **entire code step by step**.

---

## **1️⃣ Global Variables**
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 100

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;
```
- `threadDied` → A **condition variable** that tells the main thread when a worker (thread) has finished.  
- `threadMutex` → A **mutex** to protect shared data (because multiple threads access it).  

```c
static int totThreads = 0;  // Total number of threads created
static int numLive = 0;      // Number of active (or finished but not yet joined) threads
static int numUnjoined = 0;  // Number of finished but not yet joined threads
```
- `totThreads`: Total threads created.  
- `numLive`: Threads that are still running or not yet joined.  
- `numUnjoined`: Threads that finished but are waiting to be joined.  

---

## **2️⃣ Thread State Tracking**
```c
enum tstate { TS_ALIVE, TS_TERMINATED, TS_JOINED };
```
- `TS_ALIVE` → Thread is still running.  
- `TS_TERMINATED` → Thread finished but not yet joined.  
- `TS_JOINED` → Thread has been joined.  

```c
static struct { 
    pthread_t tid;      // Thread ID
    enum tstate state;  // Thread state
    int sleepTime;      // How long the thread sleeps before finishing
} *thread;
```
- **`tid`** → The thread’s ID.  
- **`state`** → Keeps track of whether the thread is **alive, terminated, or joined**.  
- **`sleepTime`** → How long the thread will sleep before finishing.

---

## **3️⃣ Thread Function**
```c
static void * threadFunc(void *arg)
{
    int idx = *((int *) arg);
    sleep(thread[idx].sleepTime);  // Simulate work
    printf("Thread %d terminating\n", idx);

    pthread_mutex_lock(&threadMutex);
    numUnjoined++;               // Increase count of unjoined threads
    thread[idx].state = TS_TERMINATED;  // Mark thread as finished
    pthread_mutex_unlock(&threadMutex);
    
    pthread_cond_signal(&threadDied);  // Notify main thread
    return NULL;
}
```
### **🔍 What Happens Here?**
✅ The thread **sleeps** for some time (simulating work).  
✅ It **updates its state** to `TS_TERMINATED` when it finishes.  
✅ It **signals** the main thread that it has finished.

---

## **4️⃣ Main Function**
```c
int main(int argc, char *argv[])
{
    int idx;
    thread = calloc(argc - 1, sizeof(*thread));  // Allocate memory for thread info
    totThreads = argc - 1;
    numLive = totThreads;
```
### **🔍 What Happens Here?**
✅ We allocate memory to store information about each thread.  
✅ `totThreads = argc - 1` → The number of threads is equal to the number of command-line arguments.  
✅ `numLive = totThreads` → Set the number of live threads.

---

### **5️⃣ Create Threads**
```c
    for (idx = 0; idx < totThreads; idx++) {
        thread[idx].sleepTime = atoi(argv[idx + 1]);  // Get sleep time from command-line args
        thread[idx].state = TS_ALIVE;  // Mark as running

        int *arg = malloc(sizeof(int));
        *arg = idx;
        
        if (pthread_create(&thread[idx].tid, NULL, threadFunc, arg) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
```
### **🔍 What Happens Here?**
✅ **Loop through all arguments** and create one thread for each.  
✅ Each thread **gets a sleep time** from command-line arguments.  
✅ The thread **starts running** and is marked as `TS_ALIVE`.  
✅ **Memory is allocated** to pass the index to the thread function.  

---

### **6️⃣ Main Thread Waits for Any Thread to Finish**
```c
    while (numLive > 0) {  // While there are running threads
        pthread_mutex_lock(&threadMutex);
        
        while (numUnjoined == 0) {
            pthread_cond_wait(&threadDied, &threadMutex);  // Wait for a thread to finish
        }
```
### **🔍 What Happens Here?**
✅ The main thread **waits** until at least one thread finishes.  
✅ It **uses a condition variable (`pthread_cond_wait`)** to pause until a thread finishes.  

---

### **7️⃣ Join Any Finished Thread**
```c
        for (idx = 0; idx < totThreads; idx++) {
            if (thread[idx].state == TS_TERMINATED) {
                pthread_join(thread[idx].tid, NULL);  // Join finished thread
                thread[idx].state = TS_JOINED;
                numLive--;
                numUnjoined--;
                printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
            }
        }

        pthread_mutex_unlock(&threadMutex);
    }
```
### **🔍 What Happens Here?**
✅ The main thread **scans all threads** to find one that has finished (`TS_TERMINATED`).  
✅ It **joins the finished thread** (`pthread_join()`).  
✅ It **updates the thread state** to `TS_JOINED`.  
✅ It **decreases `numLive` and `numUnjoined`**.  

---

### **8️⃣ Cleanup and Exit**
```c
    free(thread);
    exit(EXIT_SUCCESS);
}
```
✅ Free allocated memory.  
✅ Exit the program.  

---

# **🔹 Summary**
✔ **Problem**: `pthread_join()` requires a specific thread, making dynamic joining hard.  
✔ **Solution**: Use a condition variable to track **any** finished thread.  
✔ **How It Works**:  
   - Threads **update their state** when they finish.  
   - **Condition variable signals** when a thread finishes.  
   - **Main thread joins** any finished thread dynamically.  

---

# **🔹 Real-World Analogy**
Imagine a **restaurant kitchen** with multiple chefs (threads) cooking food:  
- Each chef starts cooking a dish (thread starts).  
- The manager doesn’t know which chef will finish first.  
- A chef rings a **bell (condition variable)** when done.  
- The manager checks and **collects the finished dish (joins the thread)**.  
- This repeats until all dishes are collected.  

This **avoids waiting for one chef at a time** and instead allows the manager to **dynamically collect** finished work.

---