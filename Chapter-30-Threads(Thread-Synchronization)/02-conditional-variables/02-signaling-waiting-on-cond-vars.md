### **1. Signaling Mechanisms in Condition Variables**
#### **`pthread_cond_signal()` vs. `pthread_cond_broadcast()`**
Both functions notify waiting threads that a shared variable's state has changed.

- **`pthread_cond_signal(pthread_cond_t *cond);`**
  - Wakes up **at least one** waiting thread.
  - Used when **only one thread** needs to respond to the state change.
  - More **efficient** because it avoids unnecessary wake-ups.

- **`pthread_cond_broadcast(pthread_cond_t *cond);`**
  - Wakes up **all** waiting threads.
  - Used when **each waiting thread has a different task**.
  - Ensures correctness but can lead to **wasteful wake-ups**.

---
### **2. Waiting on a Condition Variable**
#### **`pthread_cond_wait()`**
- **Syntax**:
  ```c
  int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
  ```
- **What it does**:
  1. Unlocks `mutex` (so other threads can modify shared data).
  2. Blocks the thread until `cond` is signaled.
  3. When signaled, it **automatically** re-locks `mutex`.

- **Example usage** (inside a **while loop** for safety):
  ```c
  pthread_mutex_lock(&mtx);
  while (/* shared variable not in desired state */) {
      pthread_cond_wait(&cond, &mtx);
  }
  // Now we can safely access the shared variable.
  pthread_mutex_unlock(&mtx);
  ```
  - **Why use a `while` loop instead of `if`?**  
    - A **spurious wake-up** may occur (thread wakes up without an actual signal).  
    - Another thread might modify the shared variable **before** this thread executes.  
    - A `while` loop ensures the condition is **checked again** after waking up.

---
### **3. Using `pthread_cond_timedwait()`**
- Allows a thread to wait **for a maximum duration**.
- If the timeout expires before the signal is received, it returns `ETIMEDOUT`.
- **Syntax**:
  ```c
  int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                              const struct timespec *abstime);
  ```
- **Example usage**:
  ```c
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 5; // Wait for up to 5 seconds

  int s = pthread_cond_timedwait(&cond, &mtx, &ts);
  if (s == ETIMEDOUT) {
      printf("Timeout occurred\n");
  }
  ```

---
### **4. Producer-Consumer Example with Condition Variables**
#### **Producer Code**
```c
pthread_mutex_lock(&mtx);
avail++;  // Increment shared resource count
pthread_mutex_unlock(&mtx);
pthread_cond_signal(&cond); // Wake up consumer
```

#### **Consumer Code**
```c
pthread_mutex_lock(&mtx);
while (avail == 0) {
    pthread_cond_wait(&cond, &mtx);
}
while (avail > 0) {
    avail--;  // Consume an item
}
pthread_mutex_unlock(&mtx);
```

---
### **5. Performance Considerations**
- **Unlocking `mutex` before signaling `cond`**:
  - Prevents unnecessary **context switches**.
  - If `pthread_cond_signal()` is called **before unlocking**, the woken thread may block immediately if the mutex is still locked.
  - On some implementations, unlocking first **improves performance** by using **wait morphing**.

---
### **Key Takeaways**
1. **Use `pthread_cond_signal()` if only one waiting thread needs to wake up.**
2. **Use `pthread_cond_broadcast()` if multiple waiting threads should wake up.**
3. **Always use `pthread_cond_wait()` inside a `while` loop** to handle spurious wake-ups.
4. **`pthread_cond_timedwait()` allows bounded waiting with a timeout.**
5. **Unlock the mutex before signaling to improve performance.**
