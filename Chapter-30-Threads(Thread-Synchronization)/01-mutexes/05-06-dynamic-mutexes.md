# Understanding Static and Dynamic Mutexes in Pthreads

## 1. Introduction to Mutexes
Mutexes (short for **mutual exclusion**) are synchronization primitives used to **prevent multiple threads from accessing shared resources simultaneously**. In **POSIX threads (pthreads)**, mutexes can be initialized in two ways:

1. **Static Initialization** (Compile-time initialization)
2. **Dynamic Initialization** (Runtime initialization)

Both methods have different use cases, advantages, and limitations, which we will explore with **theory and practical examples**.

---

## 2. Static Mutex Initialization
### Definition
Static mutexes are initialized **at compile-time** using the **PTHREAD_MUTEX_INITIALIZER** macro.

### Syntax
```c
#include <pthread.h>
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
```

### When to Use Static Mutexes?
- When the mutex is **statically allocated** (e.g., global or file-scope variable).
- When default mutex attributes are sufficient.
- When the mutex is **always needed** throughout the program.

### Example: Protecting a Global Counter
```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t counterMutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void *increment(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&counterMutex);
        counter++;
        pthread_mutex_unlock(&counterMutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Final Counter: %d\n", counter);
    return 0;
}
```

🔹 **Why Static Here?**
- The mutex is global and exists throughout the program execution.
- No custom attributes are needed.
- Using **PTHREAD_MUTEX_INITIALIZER** makes it easier to initialize.

---

## 3. Dynamic Mutex Initialization
### Definition
Dynamic mutexes are initialized **at runtime** using `pthread_mutex_init()`.

### Syntax
```c
#include <pthread.h>
pthread_mutex_t myMutex;
pthread_mutex_init(&myMutex, NULL);
```

### When to Use Dynamic Mutexes?
- When the mutex is **dynamically allocated** (e.g., inside a struct or heap memory).
- When **custom mutex attributes** (like recursive locks) are needed.
- When **multiple mutexes** need to be created dynamically.

### Example: Protecting a Linked List (Dynamic Allocation)
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
    pthread_mutex_t mutex; // Mutex inside each node
} Node;

Node *createNode(int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    pthread_mutex_init(&newNode->mutex, NULL);
    return newNode;
}

void deleteNode(Node *node) {
    pthread_mutex_destroy(&node->mutex);
    free(node);
}

int main() {
    Node *head = createNode(10);
    deleteNode(head);
    return 0;
}
```

🔹 **Why Dynamic Here?**
- The mutex is part of a **dynamically allocated structure** (linked list nodes).
- We cannot use `PTHREAD_MUTEX_INITIALIZER` because the nodes are created at runtime.
- We must manually **destroy** the mutex before freeing the memory.

---

## 4. Local (Stack) Mutexes
### Can We Have a Mutex on the Stack?
Yes, but **it must be dynamically initialized** inside the function.

### Example: Protecting a Local Variable in a Function
```c
void myFunction() {
    pthread_mutex_t localMutex;
    pthread_mutex_init(&localMutex, NULL);
    
    pthread_mutex_lock(&localMutex);
    printf("Protected section\n");
    pthread_mutex_unlock(&localMutex);
    
    pthread_mutex_destroy(&localMutex);
}
```

🔹 **Why Dynamic Here?**
- A local (stack) mutex **cannot be initialized statically**.
- It must be initialized at function entry and destroyed before exiting.

---

## 5. Why Do We Need to Destroy a Mutex?
### Key Reasons:
1. **Prevent Resource Leaks** 🛑
   - A dynamically allocated mutex takes memory. If not destroyed, it leads to memory leaks.

2. **Avoid Undefined Behavior** ⚠️
   - Destroying a mutex ensures no threads try to use an invalid mutex.

3. **Memory Management** 🗑️
   - If a mutex resides in dynamically allocated memory, destroying it **before freeing the memory** avoids corruption.

### Example: What Happens If We Don’t Destroy a Mutex?
```c
Node *head = createNode(10);
free(head); // Memory is freed, but mutex is not destroyed! 🛑
```
This can lead to **dangling pointers** and **undefined behavior**.

✅ **Correct Approach**:
```c
deleteNode(head); // Ensures mutex is destroyed before freeing memory
```

---

## 6. Comparison: Static vs. Dynamic Mutex
| Feature              | Static Mutex | Dynamic Mutex |
|----------------------|-------------|--------------|
| **Initialization**  | Compile-time | Runtime |
| **Syntax**         | `PTHREAD_MUTEX_INITIALIZER` | `pthread_mutex_init()` |
| **Location**       | Global, file-scope | Heap, local (stack) |
| **Supports Attributes?** | ❌ No | ✅ Yes |
| **Use Case**       | Simple, always needed mutex | Dynamic structures (linked list, heap objects) |
| **Needs Destruction?** | ❌ No | ✅ Yes (with `pthread_mutex_destroy()`) |

---

## 7. Mutex Attributes
### Definition
The `pthread_mutex_init()` function allows specifying attributes using a `pthread_mutexattr_t` object.

### Why Use Mutex Attributes?
- To define **different types of mutexes** (normal, recursive, error-checking).
- To set **protocols** for priority inheritance.
- To enable **shared mutexes** across processes.

### Common Mutex Types:
- **PTHREAD_MUTEX_NORMAL**: Default behavior.
- **PTHREAD_MUTEX_RECURSIVE**: Allows the same thread to lock multiple times.
- **PTHREAD_MUTEX_ERRORCHECK**: Detects deadlocks.

---

## 8. Conclusion
| **Key Takeaways** |
|------------------|
| ✅ Use **static mutexes** for global/static data with default attributes. |
| ✅ Use **dynamic mutexes** for heap-allocated structures and custom attributes. |
| ✅ **Local (stack) mutexes** must be dynamically initialized and destroyed. |
| ✅ **Always destroy a dynamically initialized mutex** before freeing memory! |
