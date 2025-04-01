 # Mutex Types in Pthreads

## Introduction
Mutexes in Pthreads come in different types, each defining specific behaviors when locking and unlocking. Understanding these types helps in choosing the right mutex for a given scenario and avoiding unintended deadlocks or undefined behavior.

SUSv3 (Single UNIX Specification, Version 3) defines the following **mutex types**:

---

## 1. **PTHREAD_MUTEX_NORMAL**
### Behavior:
- No **deadlock detection**.
- If a thread **locks the same mutex twice**, a deadlock occurs.
- Unlocking a mutex that is not locked or locked by another thread results in **undefined behavior**.
- **Linux-specific behavior**: Unlocking an unowned or unlocked mutex may **succeed without an error**, which is non-portable behavior.

### Use Case:
- Best for **simple mutual exclusion**, where recursive locking is not required.
- Suitable when threads are correctly programmed to follow lock/unlock discipline.

---

## 2. **PTHREAD_MUTEX_ERRORCHECK**
### Behavior:
- **Error checking enabled**.
- If a thread tries to:
  - Lock a mutex it already owns → **Returns an error** (avoiding deadlock).
  - Unlock a mutex it doesn't own or isn't locked → **Returns an error**.
- **Safer but slower** due to extra checks.

### Use Case:
- Useful for **debugging** to detect incorrect mutex usage.
- Helps avoid accidental **recursive locking** deadlocks.

### Example: Creating an Error-Checking Mutex
```c
pthread_mutex_t mtx;
pthread_mutexattr_t mtxAttr;
int s;

s = pthread_mutexattr_init(&mtxAttr);
if (s != 0)
    errExitEN(s, "pthread_mutexattr_init");

s = pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_ERRORCHECK);
if (s != 0)
    errExitEN(s, "pthread_mutexattr_settype");

s = pthread_mutex_init(&mtx, &mtxAttr);
if (s != 0)
    errExitEN(s, "pthread_mutex_init");

pthread_mutexattr_destroy(&mtxAttr);
```

---

## 3. **PTHREAD_MUTEX_RECURSIVE**
### Behavior:
- Allows the same thread to **lock the mutex multiple times**.
- Maintains a **lock count**:
  - First lock → **Lock count = 1**.
  - Subsequent locks by the same thread → **Lock count increments**.
  - Unlocking decrements the count.
  - Mutex is only fully released when the count reaches **0**.
- Unlocking an **unlocked mutex** or one locked by another thread **fails**.

### Use Case:
- Necessary when **recursive functions** use mutexes.
- Used when a thread **must re-lock a mutex** before unlocking it.

---

## 4. **PTHREAD_MUTEX_DEFAULT**
### Behavior:
- This is the **default type** when using `PTHREAD_MUTEX_INITIALIZER` or passing `NULL` in `pthread_mutex_init()`.
- The exact behavior is **implementation-defined**.
- On **Linux**, it behaves like **PTHREAD_MUTEX_NORMAL** (no deadlock detection).

### Use Case:
- When you don't explicitly set a mutex type.
- Behavior depends on the system, so it should be used **cautiously**.

---

## Linux-Specific Static Initializers
Linux provides **non-standard** static initializers for these mutex types:
- `PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP`
- `PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP`

🔹 **Portable applications should avoid these** and instead initialize mutexes dynamically using `pthread_mutex_init()`.

---

## Summary Table
| **Mutex Type** | **Deadlock Detection** | **Recursive Locking** | **Unlocking by Another Thread** | **Use Case** |
|--------------|----------------|----------------|----------------------|----------|
| **NORMAL** | ❌ No | ❌ No | ⚠️ Undefined Behavior (May Succeed on Linux) | Simple mutex without recursion |
| **ERRORCHECK** | ✅ Yes | ❌ No | ❌ Returns Error | Debugging incorrect usage |
| **RECURSIVE** | ✅ Yes | ✅ Yes | ❌ Returns Error | Recursive functions needing re-locking |
| **DEFAULT** | ❓ Undefined | ❓ Undefined | ❓ Undefined | System-dependent behavior |

---

## Conclusion
- **Choose `PTHREAD_MUTEX_NORMAL` for standard locking needs.**
- **Use `PTHREAD_MUTEX_ERRORCHECK` during debugging to detect improper usage.**
- **Use `PTHREAD_MUTEX_RECURSIVE` when functions require re-locking a mutex.**
- **Avoid relying on `PTHREAD_MUTEX_DEFAULT` due to its undefined behavior across systems.**

Understanding these types helps in writing **efficient, bug-free, and deadlock-resistant** multi-threaded applications. 🚀

