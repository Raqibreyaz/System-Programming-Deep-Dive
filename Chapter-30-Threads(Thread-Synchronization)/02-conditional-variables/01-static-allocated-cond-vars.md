### **Statically Allocated Condition Variables**

Just like **mutexes**, condition variables can be allocated in two ways:  
1. **Statically allocated** (predefined at compile-time).  
2. **Dynamically allocated** (created at runtime).  

We will focus on **statically allocated** condition variables here.

#### **What is a statically allocated condition variable?**  
A **statically allocated condition variable** is one that is declared at **compile time** and initialized using a predefined constant.  

#### **How to declare and initialize a statically allocated condition variable?**  
We use `pthread_cond_t` and assign it `PTHREAD_COND_INITIALIZER`:

```c
#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

This is similar to how we initialize a **mutex** with `PTHREAD_MUTEX_INITIALIZER`.

#### **Important rule: Don't copy condition variables!**  
According to **SUSv3 (Single UNIX Specification, Version 3)**,  
👉 **Copying a condition variable and then using it leads to undefined behavior!**  

Why? Because the internal state of the condition variable **depends on the original memory location**. Always operate on the **original** condition variable.

#### **Key Takeaways:**  
✔ **Statically allocated condition variables** are predefined at compile time.  
✔ Use `PTHREAD_COND_INITIALIZER` to initialize them.  
✔ Never copy condition variables; always use the original instance.  

---