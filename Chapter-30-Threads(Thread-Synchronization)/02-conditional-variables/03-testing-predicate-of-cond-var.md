### Key Takeaways:

1. **Threads may be woken up in an unpredictable order**  
   - Multiple threads may be waiting on a condition variable.
   - Even if one thread sets the predicate (shared variable) to the expected state and signals the condition variable, **another thread could acquire the mutex first and modify the predicate before our thread gets the mutex**.
   - This means that when our thread finally gets control, the predicate might no longer hold true.

2. **Loose predicates make design easier**  
   - Instead of guaranteeing that the condition is true before signaling, it's often useful to design systems where a signal just means “there **may** be something to do.”
   - This way, the thread that wakes up is responsible for **checking the predicate itself** rather than relying on an external guarantee.

3. **Spurious wake-ups are possible**  
   - Some implementations of condition variables allow a waiting thread to **wake up even if no signal was sent**.
   - This is a rare but valid behavior on some multiprocessor systems due to efficiency-related optimizations.
   - Since a thread can wake up without an actual signal, it **must check the predicate again** before proceeding.

### Correct Usage:

```c
pthread_mutex_lock(&mutex);
while (condition_not_met) {  // Always use while, not if
    pthread_cond_wait(&cond, &mutex);
}
// Perform the action now that the condition is met
pthread_mutex_unlock(&mutex);
```

### Why **not** use `if`?
```c
pthread_mutex_lock(&mutex);
if (condition_not_met) {  // ❌ Incorrect usage
    pthread_cond_wait(&cond, &mutex);
}
// If we proceed here, the predicate might not hold true!
pthread_mutex_unlock(&mutex);
```
Using `if` is incorrect because **after waking up, the condition might not be true anymore**, but execution continues anyway, leading to race conditions or incorrect behavior.
