### **1. Joining a Thread (`pthread_join`)**
- `pthread_join(pthread_t thread, void **retval);`
- Waits for a specific thread to terminate.
- If the thread has already terminated, `pthread_join()` returns immediately.
- If `retval` is non-NULL, it stores the return value of the terminated thread.
- Threads are **not hierarchical**, so any thread can join with another known thread.
- **Zombie Threads**: If a thread is not detached and not joined, it becomes a zombie, wasting system resources.
- Unlike `waitpid(-1, ...)`, `pthread_join()` cannot wait for "any" thread.

### **2. Detaching a Thread (`pthread_detach`)**
- `pthread_detach(pthread_t thread);`
- Marks the thread as detached, meaning:
  - The system automatically cleans up the thread once it terminates.
  - You **cannot** `pthread_join()` a detached thread.
  - Prevents zombie threads.
- A thread can detach itself using:
  ```c
  pthread_detach(pthread_self());
  ```

### **3. Key Differences from Process Management**
| **Feature**  | **Threads (`pthread`)**  | **Processes (`fork`)**  |
|-------------|----------------------|--------------------|
| Join Method | `pthread_join(thread_id, &retval)` | `waitpid(child_pid, &status, 0)` |
| Join with Any? | ❌ (Must specify thread ID) | ✅ (`waitpid(-1, &status, 0)`) |
| Hierarchy? | ❌ (Threads are peers) | ✅ (Parent-child relationship) |
| Cleanup | `pthread_detach()` to avoid zombies | Orphaned child adopted by `init` |
