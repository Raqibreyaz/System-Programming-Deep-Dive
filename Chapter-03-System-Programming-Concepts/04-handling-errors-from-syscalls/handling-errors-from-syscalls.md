### **Handling Errors from System Calls and Library Functions**  

Error handling is **crucial** in system programming. Almost every system call and library function returns a status value indicating success or failure. Ignoring these checks can lead to **unexpected behavior, security vulnerabilities, or debugging nightmares**.  

---

## **1️⃣ Checking Return Values of System Calls**
Most **system calls** return a specific value to indicate success or failure. **If a system call fails, it typically returns `-1` and sets `errno`** (a global integer that holds the error code).  

### ✅ **Example of Checking Return Values**  
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    int fd = open("file.txt", O_RDONLY); // Try to open file for reading
    
    if (fd == -1) { // Check if open() failed
        printf("Error: Failed to open file.txt. Errno: %d\n", errno);
        perror("open");  // Print the error description
        exit(EXIT_FAILURE);
    }

    printf("File opened successfully! FD: %d\n", fd);
    close(fd);
    return 0;
}
```

### **🔹 Output when file doesn’t exist:**
```
Error: Failed to open file.txt. Errno: 2
open: No such file or directory
```
Here, `errno` is `2`, which corresponds to `"No such file or directory"`.  

---

## **2️⃣ Understanding `errno` (Error Number)**
`errno` is a **global variable** that stores the last error number set by a failed system call.  
To use it, include:  
```c
#include <errno.h>
```
Each error number has a predefined constant **starting with `E`**, like:
- `EACCES` – Permission denied
- `ENOENT` – No such file or directory
- `EINTR` – System call interrupted by a signal  

📌 **Important Notes on `errno`:**  
1. **A successful function call does NOT reset `errno` to 0**.  
2. **Only check `errno` if the function returned an error!**  

---

## **3️⃣ Printing Error Messages**
We can display human-readable error messages using:  
### **🔹 `perror()` – Prints error message for `errno`**
```c
#include <stdio.h>
#include <fcntl.h>

int main() {
    int fd = open("nofile.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
    }
    return 0;
}
```
📌 **Output:**  
```
open: No such file or directory
```
👉 **`perror("open")`** prints: `[custom message]: [error message]`.  

### **🔹 `strerror(errno)` – Returns error message as a string**
```c
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
    errno = ENOENT;  // Set an error manually (for demonstration)
    printf("Error: %s\n", strerror(errno));
    return 0;
}
```
📌 **Output:**  
```
Error: No such file or directory
```
👉 `strerror(errno)` is useful when **you need to store or modify the error message**.

---

## **4️⃣ Handling Functions that Can Return `-1` on Success**
Some functions (like `getpriority()`) **return `-1` on success or failure**.  
To detect failure, **set `errno` to 0 before calling the function, then check `errno` afterward**.

### ✅ **Example: Checking `getpriority()` Return Value**
```c
#include <stdio.h>
#include <errno.h>
#include <sys/resource.h>

int main() {
    errno = 0; // Reset errno before function call
    int priority = getpriority(PRIO_PROCESS, 0);
    
    if (priority == -1 && errno != 0) {  // Check if error occurred
        perror("getpriority failed");
        return 1;
    }

    printf("Process priority: %d\n", priority);
    return 0;
}
```

---

## **5️⃣ Handling Library Function Errors**
Library functions in C have **different error handling mechanisms**:  
1️⃣ **Return `-1` and set `errno` (like system calls)**  
   - Example: `remove()`, `unlink()`  
2️⃣ **Return `NULL` and set `errno`**  
   - Example: `fopen()`  
3️⃣ **Do NOT use `errno`, return other error indicators**  
   - Example: `strtol()`, `getenv()`

### ✅ **Example: Checking `fopen()` Return Value**
```c
#include <stdio.h>
#include <errno.h>

int main() {
    FILE *fp = fopen("nofile.txt", "r");
    
    if (fp == NULL) {
        perror("fopen"); // Print error message
        return 1;
    }

    fclose(fp);
    return 0;
}
```
📌 **Output when file doesn't exist:**  
```
fopen: No such file or directory
```
👉 Here, **`fopen()` returns `NULL` instead of `-1`**, but `errno` is still set.

---

## **6️⃣ Best Practices for Error Handling**
✅ **Always check system call return values** (`open()`, `read()`, `write()`, etc.).  
✅ **Use `perror()` or `strerror(errno)` to print user-friendly error messages.**  
✅ **For functions that return `-1` on success, set `errno = 0` before calling them.**  
✅ **Check documentation (`man 2 syscall`) for proper error-handling methods.**  

---

### **🔥 Summary Table**
| Function Type        | Error Return | Uses `errno`? | Example |
|----------------------|-------------|--------------|---------|
| **System Calls**     | `-1`         | ✅ Yes       | `open()`, `read()` |
| **Library Functions (errno-based)** | `-1` or `NULL` | ✅ Yes | `fopen()`, `remove()` |
| **Library Functions (non-errno)**  | Custom value | ❌ No | `strtol()`, `getenv()` |

---
