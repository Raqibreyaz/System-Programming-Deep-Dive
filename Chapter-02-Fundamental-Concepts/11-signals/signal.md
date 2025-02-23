## **Signals in Linux (Step-by-Step)**  

### **1️⃣ What Are Signals?**  
Think of **signals** as **alerts** or **notifications** sent to a process. These alerts tell a process that **something has happened**.  

💡 Example:  
- When you press **Ctrl + C**, your terminal sends a **signal** to stop the running program.  
- When a program crashes, the system **automatically sends a signal** to terminate it.  

---

### **2️⃣ How Are Signals Sent?**  
There are **3 ways** a process can receive a signal:  

| Who Sends the Signal?  | Example |
|------------------------|---------|
| **Kernel (OS)** | Pressing **Ctrl + C** sends `SIGINT` (Interrupt Signal). |
| **Another Process** | The `kill` command can send signals to processes. |
| **Itself** | A program can send signals to itself using `raise()`. |

---

### **3️⃣ Common Signals & Their Meaning**  

| Signal Name | Signal Number | What It Does | Example |
|------------|--------------|-------------|---------|
| **SIGINT** | `2` | Interrupts (stops) a program (like **Ctrl + C**). | Pressing **Ctrl + C** in a running program. |
| **SIGKILL** | `9` | Forcefully kills a process (cannot be ignored). | `kill -9 <pid>` |
| **SIGTERM** | `15` | Requests a process to terminate (can be handled). | `kill <pid>` |
| **SIGSTOP** | `19` | Pauses a process. | Pressing **Ctrl + Z** |
| **SIGCONT** | `18` | Resumes a paused process. | `fg` or `bg` command. |
| **SIGSEGV** | `11` | Sent when a program accesses an invalid memory location (Segmentation Fault). | Trying to access an unallocated memory address. |

---

### **4️⃣ Example 1: Sending Signals Using `kill` Command**  
Linux has a **built-in command** called `kill` to send signals to processes.  

#### **Steps to Try This**  
1. Open a terminal and run a program that takes time (like `sleep`):  
   ```bash
   sleep 1000
   ```
2. Find its **Process ID (PID)** using:  
   ```bash
   ps
   ```
3. Kill it using **SIGTERM**:  
   ```bash
   kill <PID>
   ```
4. If it **doesn't stop**, use **SIGKILL**:  
   ```bash
   kill -9 <PID>
   ```

📌 **Key Difference:**  
- `kill <PID>` sends **SIGTERM** (graceful exit).  
- `kill -9 <PID>` sends **SIGKILL** (forceful exit, cannot be ignored).  

---

### **5️⃣ Example 2: Handling Signals in a C Program**  
A process can **handle signals** using a **signal handler**. Let's write a **simple C program** that:  
- **Catches `SIGINT` (`Ctrl + C`)** and prints a message instead of exiting.  
- **Exits normally when `Ctrl + C` is pressed twice**.  

#### **Code Example: Handling SIGINT (`Ctrl + C`)**  
```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int counter = 0;

// Custom signal handler function
void handle_sigint(int sig) {
    counter++;
    printf("Caught SIGINT (%d)! Press Ctrl+C %d more times to exit.\n", sig, 2 - counter);
    
    // Exit after 2 presses
    if (counter >= 2) {
        printf("Exiting now!\n");
        exit(0);
    }
}

int main() {
    // Set the custom handler for SIGINT
    signal(SIGINT, handle_sigint);
    
    while (1) {
        printf("Running... Press Ctrl+C to test signal handling.\n");
        sleep(2);
    }
    
    return 0;
}
```

#### **How It Works**  
1. **`signal(SIGINT, handle_sigint);`** → Tells the program to **call `handle_sigint` when `SIGINT` (Ctrl + C) is received**.  
2. When you **press `Ctrl + C`**, it **prints a message instead of exiting**.  
3. If you press `Ctrl + C` **twice**, it **exits normally**.  

📌 **Try It Out:** Compile & run the code!  
```bash
gcc signal_handler.c -o signal_handler
./signal_handler
```
Press **Ctrl + C** and watch the behavior! 😊  

---

### **6️⃣ Pending & Blocking Signals (What Happens If a Signal Is Delayed?)**  

#### **What Is a Pending Signal?**  
A signal is **pending** if it was sent but **not yet delivered** to the process.  
For example, if a process is **blocked** from receiving a signal, the signal will be **pending** until it is unblocked.  

#### **What Is Blocking a Signal?**  
Blocking a signal **prevents** it from being delivered until it's **unblocked**.  
For example, a process might block `SIGINT` so that **Ctrl + C doesn't interrupt it** at critical moments.  

#### **Example: Blocking Signals in a C Program**
Let's block `SIGINT` (Ctrl + C) for 5 seconds and **then unblock it**.

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t new_set, old_set;

    // Initialize the set and add SIGINT to it
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);

    // Block SIGINT
    sigprocmask(SIG_BLOCK, &new_set, &old_set);
    printf("SIGINT is now blocked. Try pressing Ctrl + C... Nothing happens!\n");

    // Sleep for 5 seconds while SIGINT is blocked
    sleep(5);

    // Unblock SIGINT
    sigprocmask(SIG_SETMASK, &old_set, NULL);
    printf("SIGINT is now unblocked! Press Ctrl + C now.\n");

    // Wait for user input to keep the program running
    while (1) {
        sleep(1);
    }

    return 0;
}
```

#### **How This Works**  
1. `sigprocmask(SIG_BLOCK, &new_set, &old_set);` → **Blocks `SIGINT` (Ctrl + C)**.  
2. The program **sleeps for 5 seconds** while ignoring `SIGINT`.  
3. `sigprocmask(SIG_SETMASK, &old_set, NULL);` → **Unblocks `SIGINT`**, allowing `Ctrl + C` to work again.  

📌 **Try It Out:** Compile & run it. Then press **Ctrl + C** before & after 5 seconds.  

---

### **7️⃣ Key Takeaways**  

✅ **Signals are notifications** sent to a process about events.  
✅ **Sent by kernel, another process, or itself**.  
✅ **Processes can ignore, handle, or be killed by signals**.  
✅ **Pending signals** are signals waiting to be delivered.  
✅ **Blocking signals** prevents them from being delivered until unblocked.  
✅ **Signal handlers** let processes define custom responses to signals.  

---