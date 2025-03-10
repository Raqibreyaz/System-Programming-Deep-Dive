## **1. What is `socketpair()`?**
`socketpair()` is a **system call** that creates **two connected sockets** inside a single process. These sockets can be used for **bidirectional communication**, meaning data can be sent in both directions.

Instead of manually creating, binding, and connecting two sockets, `socketpair()` does it in **one step**.

It is useful when:
- **A parent and child process need to communicate** after `fork()`.
- **Two threads need to exchange data efficiently** within the same process.
- **It replaces pipes** (`pipe()`) when you need **full-duplex** (two-way) communication.

---

## **2. `socketpair()` Syntax**
```c
#include <sys/socket.h>

int socketpair(int domain, int type, int protocol, int sockfd[2]);
```
- `domain` → Must be **AF_UNIX** (local UNIX domain, since both sockets are on the same machine).
- `type` → Can be:
  - **SOCK_STREAM** (like TCP) → reliable, ordered data stream.
  - **SOCK_DGRAM** (like UDP) → connectionless, fast.
- `protocol` → Always **0** (automatically selects the right protocol).
- `sockfd[2]` → An array where `socketpair()` stores **two connected socket file descriptors**.

On **success**, it returns `0`. On failure, it returns `-1`.

---

## **3. How `socketpair()` Works**
When `socketpair()` is called:
1. It creates **two sockets**.
2. These sockets are **already connected**.
3. **Writing to one socket allows reading from the other**.

This is different from regular sockets, where you:
- Call `socket()`, `bind()`, `listen()`, and `accept()` manually.
- Need a separate **server and client**.

Here, **no server is needed**—both sockets are instantly connected.

---

## **4. Why Do We Need Two Sockets in One Process?**
You might wonder: *Why create two connected sockets in the same process?*  
The answer is **Inter-Process Communication (IPC)**.

### **Example Use Cases**
1. **Parent and Child Communication** (after `fork()`).
2. **Thread Communication** in the same process.
3. **Replacing pipes (`pipe()`)** when **full-duplex communication** is needed.
4. **Secure Communication**, since UNIX domain sockets are **not exposed** to external processes.

---

## **5. How Writing to `sockfd[0]` Allows Reading from `sockfd[1]`**
When `socketpair()` creates two connected sockets:
- **Anything written to `sockfd[0]` can be read from `sockfd[1]`.**
- **Anything written to `sockfd[1]` can be read from `sockfd[0]`.**

### **Example: Data Flow**
```c
int sockfd[2];
socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd);

write(sockfd[0], "Hello", 5);  // Writing to socket 0
char buffer[10];
read(sockfd[1], buffer, 5);    // Reading from socket 1
```
✅ The `"Hello"` written to `sockfd[0]` is immediately available in `sockfd[1]`.

Similarly:
```c
write(sockfd[1], "World", 5);  // Writing to socket 1
read(sockfd[0], buffer, 5);    // Reading from socket 0
```
✅ The `"World"` sent from `sockfd[1]` is available in `sockfd[0]`.

This is **why socket pairs are useful**—each side acts as **both sender and receiver**.

---

## **6. Example: Parent-Child Communication Using `socketpair()`**
Here's how a **parent and child process** can communicate using `socketpair()`.

### **Code Example**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main() {
    int sockfd[2];  // Array to store the two socket descriptors
    char buffer[100];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd) == -1) {
        perror("socketpair failed");
        exit(1);
    }

    pid_t pid = fork(); // Create child process

    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {  // Child process
        close(sockfd[0]); // Close unused socket

        // Read from parent
        read(sockfd[1], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Send reply to parent
        write(sockfd[1], "Hello Parent!", 13);
        
        close(sockfd[1]); // Close socket
    } else {  // Parent process
        close(sockfd[1]); // Close unused socket

        // Send message to child
        write(sockfd[0], "Hello Child!", 12);

        // Read reply from child
        read(sockfd[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(sockfd[0]); // Close socket
    }

    return 0;
}
```

### **Explanation**
1. **`socketpair()` creates two connected sockets.**
2. **`fork()` creates a child process.**
3. **Parent writes "Hello Child!" to `sockfd[0]`.**
4. **Child reads from `sockfd[1]`.**
5. **Child replies "Hello Parent!" via `sockfd[1]`.**
6. **Parent reads the response from `sockfd[0]`.**
7. **Both processes close unused sockets.**

### **Output**
```
Child received: Hello Child!
Parent received: Hello Parent!
```

---

## **7. Key Differences: `socketpair()` vs. `pipe()`**
| Feature            | `pipe()`                     | `socketpair()`                 |
|--------------------|----------------------------|-------------------------------|
| Communication     | Unidirectional (one-way)    | Bidirectional (two-way)       |
| Connection       | Parent-child only           | Can be used in threads too   |
| Data Type        | Byte stream (raw data)      | Stream or datagram           |
| UNIX Domain Only? | No                          | Yes (`AF_UNIX` only)         |

If you need **two-way communication**, `socketpair()` is better than `pipe()`.

---

## **8. Advanced Features (`SOCK_CLOEXEC` and `SOCK_NONBLOCK`)**
Starting with Linux **2.6.27**, two flags can be OR-ed with the `type` argument:
1. **`SOCK_CLOEXEC`**  
   - Enables the **close-on-exec** flag, so file descriptors do **not** leak into exec'd programs.
   - Equivalent to calling `fcntl(fd, F_SETFD, FD_CLOEXEC)`.
2. **`SOCK_NONBLOCK`**  
   - Makes sockets **non-blocking** immediately.
   - Equivalent to calling `fcntl(fd, F_SETFL, O_NONBLOCK)`.

Example:
```c
socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0, sockfd);
```
This **saves extra `fcntl()` calls**.

---

## **9. Summary**
- `socketpair()` creates **two connected sockets** in **one process**.
- It is used for **parent-child communication** after `fork()`.
- **Writing to one socket makes data available in the other**.
- Unlike regular sockets, **no binding or listening is needed**.
- It provides **bidirectional communication**, unlike pipes (`pipe()`).
- Linux 2.6.27+ adds **`SOCK_CLOEXEC` and `SOCK_NONBLOCK`** for efficiency.

---

## **1. How Can the Child Use the Parent’s File Descriptor?**
### **Key Concept: File Descriptor Table and `fork()`**
- Every **process** has its own **file descriptor (FD) table**.
- When a process calls `fork()`, the **child process gets a copy** of the parent's **file descriptor table**.
- This means **the child process gets the same FDs as the parent**, pointing to the **same underlying file/socket**.

#### **Important:**
- The **FD numbers are different for each process**, but they **refer to the same open file/socket in the OS**.
- The **OS maintains an internal file table** that tracks which file/socket an FD refers to.

---

## **2. Why Can the Child Read/Write to the Parent’s FD?**
When `socketpair()` creates two sockets, it returns **two FDs**:
```c
int sockfd[2];  
socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd);
```
Now:
- `sockfd[0]` → One end of the socket.
- `sockfd[1]` → Other end of the socket.
- The **parent and child get the same FD numbers in their own file descriptor tables**.

After `fork()`, **both the parent and child have access to `sockfd[0]` and `sockfd[1]`.**  

#### **Example (Before Closing Any FD)**
| Process | `sockfd[0]` | `sockfd[1]` |
|---------|------------|------------|
| Parent  | ✅ Open    | ✅ Open    |
| Child   | ✅ Open    | ✅ Open    |

Since both processes have the **same file descriptors**, if the **child writes to `sockfd[1]`**, the **parent can read from `sockfd[0]`**.

---

## **3. Why Do We Close Unused File Descriptors?**
### **Parallel Execution: Does Closing Affect the Other Process?**
You asked:  
*"Both processes run in parallel, so why does the parent close `sockfd[1]`, which the child uses?"*

### **Key Point: Closing a File Descriptor Only Affects the Calling Process**
When the **parent closes `sockfd[1]`**, it **only** removes it from the **parent’s file descriptor table**.  
The **child still has `sockfd[1]` open** in its own table.

Similarly, when the **child closes `sockfd[0]`**, it only affects the child’s table—the parent still has `sockfd[0]`.

#### **Example (After Closing Unused FDs)**
| Process | `sockfd[0]` | `sockfd[1]` |
|---------|------------|------------|
| Parent  | ✅ Open    | ❌ Closed  |
| Child   | ❌ Closed  | ✅ Open    |

- The **parent keeps `sockfd[0]`** (reading end).
- The **child keeps `sockfd[1]`** (writing end).
- They can now **exchange messages without confusion**.

### **Why Close?**
1. **Avoid Resource Leaks**  
   - If we don’t close unused FDs, they stay open until the process exits, which wastes resources.
   
2. **Prevents Accidental Reads/Writes**  
   - The parent **shouldn’t accidentally write to `sockfd[1]`** if it only needs to read.  
   - The child **shouldn’t accidentally read from `sockfd[0]`** if it only needs to write.  

3. **EOF (End-of-File) Detection**  
   - If the parent closes `sockfd[1]`, and the child tries to read from it, it gets **EOF** (`read()` returns `0`), so it knows the parent won’t send more data.
   - This helps with **clean process termination**.

---

## **4. Summary**
- **After `fork()`, both parent and child have copies of the same file descriptors**.
- **Closing a file descriptor in one process does NOT close it in the other**.
- **We close unused FDs to avoid confusion, save resources, and detect EOF properly**.
