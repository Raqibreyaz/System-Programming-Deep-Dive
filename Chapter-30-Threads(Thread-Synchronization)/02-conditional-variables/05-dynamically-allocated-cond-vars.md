## 📌 Dynamically Allocated Condition Variables

The `pthread_cond_init()` function is used to **dynamically initialize a condition variable**. The situations where we need this are similar to when `pthread_mutex_init()` is required — i.e., when the condition variable is:

- Automatically or dynamically allocated (e.g., inside a `malloc`-ed struct)
- Or requires **non-default attributes**

```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
// Returns 0 on success, or a positive error number on error
```

- `cond`: Pointer to the condition variable to initialize
- `attr`: Pointer to condition attribute object (`NULL` means default attributes)

🔸 **Note**: Initializing a condition variable that is already initialized results in **undefined behavior** (per SUSv3).

---

## ✅ When to Use `pthread_cond_init()`

Use it when:
- You allocate condition variables dynamically (e.g., via `malloc`)
- You use automatic (stack-based) structs that contain `pthread_cond_t`
- You need non-default attributes for the condition variable

---

## 🧼 Destroying Condition Variables

Use `pthread_cond_destroy()` when you're done with a condition variable.

```c
int pthread_cond_destroy(pthread_cond_t *cond);
// Returns 0 on success, or a positive error number on error
```

### ✅ Safe to destroy when:
- **No thread is currently waiting** on it
- It's dynamically or automatically allocated (e.g., inside a struct)

### 🚫 Not required for:
- Statically initialized condition variables (`PTHREAD_COND_INITIALIZER`)

---

## 💡 Use Cases for Dynamically Allocated Condition Variables

Here are real-world cases where dynamically allocated condition variables are necessary:

---

### 🔁 1. **Thread Pools**
Each worker may have its own `pthread_cond_t` for communication:

```c
struct Worker {
    pthread_t thread;
    pthread_cond_t cond;
    pthread_mutex_t mtx;
};
```

Useful for:
- Assigning specific work
- Graceful shutdown per thread

---

### 📦 2. **Producer-Consumer Queues**

Bounded buffer needs multiple condition variables:
- `not_empty` (consumer waits)
- `not_full` (producer waits)

If the queue is dynamically allocated, so must the cond vars be.

---

### 📨 3. **Event or Message Listener Systems**

Each listener waits on its own signal:

```c
struct Listener {
    pthread_cond_t cond;
    pthread_mutex_t mtx;
    bool eventReady;
};
```

Useful in:
- Event-driven systems
- Async notifications

---

### 🧱 4. **Reusable Concurrency Utilities or Libraries**

You want to build generic, modular APIs that manage their own state internally.

```c
typedef struct Waiter {
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} Waiter;
```

You expose:
```c
Waiter* waiter_new();
void waiter_wait(Waiter* w);
void waiter_signal(Waiter* w);
void waiter_free(Waiter* w);
```

Encapsulation and dynamic behavior both require `pthread_cond_init()` and `destroy()`.

---

### ⏱️ 5. **Task Schedulers and Timed Wait Queues**

Each task may block until a certain time using its own `cond`:

```c
struct TimedTask {
    pthread_cond_t wake;
    pthread_mutex_t lock;
    time_t wakeTime;
};
```

Especially useful for alarm queues, timers, cron systems, etc.

---

## 🔚 Summary

Use dynamic condition variables when:

| Situation | Reason |
|----------|--------|
| You allocate structs dynamically | Static initializers won't work inside `malloc`-ed memory |
| You want encapsulated concurrency | Clean, reusable abstractions need their own sync primitives |
| You have variable number of threads or waiters | Can't use fixed global statically-initialized cond vars |
| You want non-default attributes | Must use `pthread_cond_init()` |

---