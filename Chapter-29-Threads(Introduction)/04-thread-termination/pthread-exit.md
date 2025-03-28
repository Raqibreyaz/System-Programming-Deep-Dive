## **How a Thread Terminates**
A thread can end in **four ways**:
1. **Returning from its start function**  
   - The thread **completes its task** and exits naturally.  
2. **Calling `pthread_exit()`**  
   - The thread **explicitly** exits and **returns a value**.  
3. **Being canceled with `pthread_cancel()`**  
   - Another thread **forces** it to stop.  
4. **Main thread exits (`return` or `exit()`)**  
   - If `main()` returns or calls `exit()`, **all threads** stop.

---

### **`pthread_exit()` – Exiting a Thread Manually**
**Prototype:**
```c
#include <pthread.h>

void pthread_exit(void *retval);
```
- Ends the **calling thread**.
- **`retval`**: The return value **passed to `pthread_join()`** in another thread.

---

### **Example Usage**
```c
#include <stdio.h>
#include <pthread.h>

void *thread_function(void *arg) {
    printf("Thread is exiting...\n");
    pthread_exit("Thread Finished");  // Return value to be received by pthread_join
}

int main() {
    pthread_t thread;
    void *retval;

    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, &retval);  // Capture the return value

    printf("Thread returned: %s\n", (char *)retval);
    return 0;
}
```

---

### **Key Points to Remember**
1. **Why Use `pthread_exit()`?**  
   - If a thread **wants to return a value**, it should use `pthread_exit()`.  
   - Unlike `return`, `pthread_exit()` **can be called from any function** inside the thread.  

2. **Where Should `retval` Data Be Stored?**  
   - **Don’t store `retval` on the thread’s stack** (it disappears when the thread exits).  
   - Use **heap memory** (`malloc()`) or a **global/static variable** if needed.  

3. **What Happens When the Main Thread Calls `pthread_exit()`?**  
   - **Other threads keep running** until they finish their tasks.  
   - But if `main()` returns or calls `exit()`, **all threads stop immediately**.  

---

### **Final Takeaway**
- **Use `pthread_exit()` when you want to exit a thread and return data.**  
- **Avoid returning local stack data; use heap or global variables.**  
- **If the main thread calls `pthread_exit()`, other threads continue running.**  