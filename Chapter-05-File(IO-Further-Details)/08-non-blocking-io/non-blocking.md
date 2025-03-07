- **What FIFOs are and how they work**  
- **Blocking behavior of `open()` on FIFOs**  
- **Handling `EAGAIN` when `read()` or `write()` fails**  
- **Creating FIFOs properly in code**  
- **How to ensure FIFOs exist before opening them**  
- **Example programs with proper error handling**  

---

# **📌 FIFOs (Named Pipes) - Complete Note**
## **🔹 What is a FIFO (Named Pipe)?**
A **FIFO (First In, First Out) special file**, also called a **named pipe**, is a type of **Inter-Process Communication (IPC) mechanism** that allows two processes to **communicate using a file-like interface**.  

**Key Features:**
- Unlike regular pipes (`|`), FIFOs **exist as actual files** in the filesystem.
- Any process can open a FIFO **by name** (e.g., `/tmp/myfifo`).
- Data written into a FIFO by **one process** can be read by **another process**.
- **Blocking behavior**: If one process opens a FIFO for **reading**, it waits for a writer (and vice versa).
- **Uses:** Process-to-process communication, simple IPC.

---

## **🔹 Creating a FIFO**
Before using a FIFO, we must **create** it in the filesystem.

### **✅ Method 1: Using `mkfifo` (Command Line)**
We can create a FIFO manually using the `mkfifo` command:  
```sh
mkfifo myfifo
```
This creates a **special file** called `myfifo`.

### **✅ Method 2: Using `mkfifo()` in C Code**
To **create a FIFO programmatically**, we use the `mkfifo()` system call:
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main() {
    if (mkfifo("myfifo", 0666) == -1) {
        perror("mkfifo");
    }
    return 0;
}
```
- The `0666` permission allows **read/write access** for everyone.
- If the FIFO already exists, `mkfifo()` **fails** (returns `-1`).

---

## **🔹 Opening a FIFO (`open()`)**
To use a FIFO, a process must open it using `open()`, just like a normal file.

### **Blocking Behavior of `open()` on FIFOs**
- **`O_RDONLY` (Read Only)**: The process **blocks (waits)** until another process **opens it for writing**.
- **`O_WRONLY` (Write Only)**: The process **blocks (waits)** until another process **opens it for reading**.
- **`O_RDWR` (Read/Write)**: No blocking; the process can both read and write.

### **✅ Example 1: FIFO Reader (Blocks Until Writer Connects)**
```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("myfifo", O_RDONLY);
    printf("FIFO opened for reading\n");

    char buffer[100];
    read(fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    close(fd);
    return 0;
}
```
💡 **If no writer exists**, `open("myfifo", O_RDONLY);` **blocks until a writer process appears**.

---

### **✅ Example 2: FIFO Writer (Blocks Until Reader Connects)**
```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("myfifo", O_WRONLY);
    printf("FIFO opened for writing\n");

    write(fd, "Hello, FIFO!", 12);
    close(fd);
    return 0;
}
```
💡 **If no reader exists**, `open("myfifo", O_WRONLY);` **blocks until a reader process appears**.

---

## **🔹 Non-Blocking Mode (`O_NONBLOCK`)**
To **avoid blocking**, use the `O_NONBLOCK` flag:
```c
int fd = open("myfifo", O_RDONLY | O_NONBLOCK);
```
- If there is **no writer**, `open()` **fails immediately** with `ENXIO` (No such device or address).
- If `read()` has **no data available**, it fails with **`EAGAIN`**.

---

## **🔹 Handling `EAGAIN` in `read()` and `write()`**
If a FIFO is **non-blocking**, `read()` and `write()` can return `EAGAIN`:
- **`read()` returns `EAGAIN`** if no data is available.
- **`write()` returns `EAGAIN`** if the FIFO buffer is full.

### **✅ Example: Non-Blocking Read**
```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int fd = open("myfifo", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char buffer[100];
    int bytesRead = read(fd, buffer, sizeof(buffer));

    if (bytesRead == -1 && errno == EAGAIN) {
        printf("No data available, try again later.\n");
    } else {
        printf("Received: %s\n", buffer);
    }

    close(fd);
    return 0;
}
```
✔ **If there is no data, the program doesn’t block—it prints `"No data available, try again later."` instead.**

---

## **🔹 Ensuring FIFO Exists Before Opening**
Instead of requiring a **manual `mkfifo`**, we can **check and create it inside the program**.

### **✅ Proper FIFO Handling in Code**
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"

int main() {
    // Create FIFO if it doesn’t exist
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
    }

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    write(fd, "Hello, FIFO!", 12);
    close(fd);
    return 0;
}
```
✔ **Ensures FIFO is created before opening it.**  

---

## **🔥 Summary**
| Feature | **Blocking Mode** | **Non-Blocking Mode (`O_NONBLOCK`)** |
|---------|-----------------|------------------------------|
| `open(O_RDONLY)` | Blocks until a writer connects | Fails with `ENXIO` if no writer |
| `open(O_WRONLY)` | Blocks until a reader connects | Fails with `ENXIO` if no reader |
| `read(fd, buf, size)` | Blocks until data is available | Returns `EAGAIN` if no data |
| `write(fd, buf, size)` | Blocks until buffer space is available | Returns `EAGAIN` if full |

---

## **✅ Advantages of FIFOs**
✔ **Inter-Process Communication (IPC)** between unrelated processes.  
✔ **Simple to use** (file-like interface: `open()`, `read()`, `write()`).  
✔ **Can persist in the filesystem**, unlike unnamed pipes.  

## **❌ Limitations of FIFOs**
❌ **Only for local processes** (not across networks).  
❌ **Unidirectional communication** (for two-way, two FIFOs are needed).  
❌ **Blocking behavior** unless explicitly set to `O_NONBLOCK`.  

---

## **🚀 Final Takeaway**
- **FIFOs enable IPC using named files in the filesystem.**
- **Blocking behavior can cause deadlocks if not handled properly.**
- **Use `O_NONBLOCK` to avoid waiting indefinitely.**
- **Handle `EAGAIN` properly in non-blocking mode.**
- **Always ensure the FIFO exists before opening it (`mkfifo()`).**