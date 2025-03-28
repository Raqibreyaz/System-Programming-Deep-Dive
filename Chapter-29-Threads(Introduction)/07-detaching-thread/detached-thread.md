## **What is a Detached Thread?**
By default, when a thread is created using `pthread_create()`, it is **joinable**. This means:
- The thread’s exit status can be collected by another thread using `pthread_join()`.
- The system does not automatically clean up the thread’s resources after it finishes execution.

However, in some cases, we may not need to wait for a thread to finish or retrieve its result. Instead, we just want the system to **automatically clean up** the thread once it completes. In such cases, we can make the thread **detached**.

### **Key Characteristics of a Detached Thread**
- It **automatically cleans up** its resources when it finishes execution.
- No other thread can join it using `pthread_join()`.
- Once detached, a thread **cannot be made joinable again**.
- It **still gets terminated** if another thread calls `exit()`, or if `main()` finishes.

---

## **How to Detach a Thread**
We can detach a thread in two ways:

### **Method 1: Using `pthread_detach()`**
We can detach a thread at any point after it is created by calling:
```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```
- `thread`: The ID of the thread to detach.
- Returns `0` on success, or a **positive error number** on failure.

**Example: Detaching Another Thread**
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    printf("Thread is running...\n");
    sleep(2);
    printf("Thread is finishing...\n");
    return NULL;
}

int main() {
    pthread_t thread;
    
    // Create a new thread
    pthread_create(&thread, NULL, thread_function, NULL);

    // Detach the thread
    pthread_detach(thread);

    printf("Main thread does not wait for the detached thread.\n");

    sleep(3); // Give enough time for the detached thread to finish
    printf("Main thread exits.\n");
    
    return 0;
}
```
### **Explanation:**
1. We create a new thread.
2. We immediately **detach** it using `pthread_detach(thread)`.
3. The main thread does **not wait** for the detached thread.  
4. The detached thread automatically cleans up when it finishes execution.

---

### **Method 2: A Thread Detaching Itself**
A thread can also **detach itself** by calling:
```c
pthread_detach(pthread_self());
```
This ensures that the thread does not need another thread to detach it.

**Example: Self-Detaching Thread**
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    // The thread detaches itself
    pthread_detach(pthread_self());

    printf("Detached thread running...\n");
    sleep(2);
    printf("Detached thread finishing...\n");
    
    return NULL;
}

int main() {
    pthread_t thread;
    
    pthread_create(&thread, NULL, thread_function, NULL);

    printf("Main thread does not join the detached thread.\n");
    
    sleep(3); // Give time for the detached thread to complete
    printf("Main thread exits.\n");

    return 0;
}
```
### **Explanation:**
1. The thread **calls `pthread_detach(pthread_self())`**, making itself detached.
2. The main thread does **not join** the detached thread.
3. The detached thread cleans up automatically when it finishes.

---

## **What Happens When You Detach a Thread?**
- **Before termination**: The thread runs normally.
- **After termination**: The system **automatically cleans up** all its resources.
- **No `pthread_join()` allowed**: You cannot wait for its result.

---

## **Key Notes About Detached Threads**
🚀 **When to use detached threads?**  
- When you **don’t need the thread’s return value**.
- When you **don’t want to manually clean up** the thread’s resources.

⚠️ **When NOT to use detached threads?**  
- If you **need the thread’s result** (use `pthread_join()` instead).
- If the thread does important work that must be finished before exiting.

🛑 **Common mistakes to avoid**
- Trying to `pthread_join()` a detached thread → ❌ **Undefined behavior!**
- Assuming a detached thread is always running → ❌ It can finish anytime.

---

## **Summary**
| Feature        | Joinable Thread | Detached Thread |
|---------------|----------------|----------------|
| Can call `pthread_join()` | ✅ Yes | ❌ No |
| Automatically cleans up? | ❌ No | ✅ Yes |
| Can be made joinable again? | ✅ Yes | ❌ No |
| Useful for returning values? | ✅ Yes | ❌ No |
| Best when we don’t care about result? | ❌ No | ✅ Yes |

---

## **Final Thoughts**
- Detached threads **free resources automatically** but **cannot be joined**.
- Use `pthread_detach(thread)` **if another thread should detach it**.
- Use `pthread_detach(pthread_self())` **if a thread should detach itself**.
- Detached threads **still get terminated** if the main process exits.
