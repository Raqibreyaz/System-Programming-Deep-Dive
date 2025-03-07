# **🔹 Creating a Socket (`socket()`)**

The `socket()` **system call** is the first step in **network communication**. It creates a new **socket** that applications use to **send and receive data**.

---

## **🔷 Function Prototype**
```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```
### **🔹 Return Value**
| **Returns** | **Meaning** |
|------------|------------|
| **File Descriptor (≥ 0)** | Success (Socket created) |
| **-1** | Error (Check `errno` for details) |

💡 **The returned file descriptor behaves like a normal file descriptor** and can be used with **read, write, close**, etc.

---

## **🔷 Parameters of `socket()`**
| **Parameter** | **Purpose** | **Common Values** |
|-------------|------------|------------------|
| `domain` | Specifies the **communication domain** (protocol family). | `AF_INET`, `AF_INET6`, `AF_UNIX` |
| `type` | Specifies the **socket type** (How data is sent). | `SOCK_STREAM`, `SOCK_DGRAM`, `SOCK_RAW` |
| `protocol` | Specifies the **protocol** (0 for default). | `0`, `IPPROTO_TCP`, `IPPROTO_UDP`, `IPPROTO_RAW` |

---

## **🔷 Understanding Each Parameter**

### **1️⃣ `domain` (Address Family)**
This defines the **protocol family** used for communication.

| **Value** | **Description** |
|----------|----------------|
| `AF_INET` | IPv4 (Most Common) |
| `AF_INET6` | IPv6 |
| `AF_UNIX` | Local (Interprocess Communication) |
| `AF_PACKET` | Low-Level Packet Interface |

**Example:**
```c
int sock_fd = socket(AF_INET, SOCK_STREAM, 0);  // Creates an IPv4 socket
```

---

### **2️⃣ `type` (Socket Type)**
This determines **how data is transmitted**.

| **Value** | **Description** |
|----------|----------------|
| `SOCK_STREAM` | TCP (Reliable, Ordered, Connection-Based) |
| `SOCK_DGRAM` | UDP (Fast, Unordered, Connectionless) |
| `SOCK_RAW` | Direct Packet Access (Used for custom protocols) |
| `SOCK_SEQPACKET` | Like `SOCK_STREAM` but with fixed-size messages |

**Example:**
```c
int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);  // Creates a UDP socket
```

---

### **3️⃣ `protocol` (Protocol Selection)**
- Usually set to **`0`** (chooses the default protocol for the socket type).
- If needed, you can **explicitly specify a protocol**.

| **Value** | **Description** |
|----------|----------------|
| `0` | Default (TCP for `SOCK_STREAM`, UDP for `SOCK_DGRAM`) |
| `IPPROTO_TCP` | TCP |
| `IPPROTO_UDP` | UDP |
| `IPPROTO_RAW` | Raw Sockets (Low-Level Networking) |

**Example:**
```c
int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // Explicitly using TCP
```

---

## **🔷 Example: Creating a Simple TCP Socket**
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully! FD: %d\n", sock_fd);
    close(sock_fd);  // Close the socket
    return 0;
}
```

✅ **Explanation:**
1. `socket(AF_INET, SOCK_STREAM, 0)` → Creates an IPv4 TCP socket.
2. If `socket()` fails, `perror()` prints the error.
3. If successful, it prints the socket file descriptor.
4. `close(sock_fd)` → Closes the socket when done.

---

## **🔷 Special Flags in `socket()`**
Starting from **Linux Kernel 2.6.27**, two **nonstandard flags** can be used in the `type` parameter:

| **Flag** | **Purpose** |
|---------|------------|
| `SOCK_CLOEXEC` | Automatically closes the socket **on `exec()`**. |
| `SOCK_NONBLOCK` | Makes the socket **non-blocking** (no need for `fcntl()`). |

**🔹 Why Use These Flags?**
- `SOCK_CLOEXEC`: Prevents accidental inheritance in child processes.
- `SOCK_NONBLOCK`: Avoids extra system calls to `fcntl()` for non-blocking mode.

**Example:**
```c
int sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
```
- **Creates a TCP socket**.
- **Closes automatically if a new process is executed (`exec()`)**.
- **Non-blocking mode enabled**.

---

## **🔷 Error Handling**
If `socket()` fails, it returns `-1` and sets `errno`. Use `perror()` or `strerror(errno)` to diagnose errors.

| **Error Code** | **Meaning** |
|--------------|-------------|
| `EAFNOSUPPORT` | Address family not supported. |
| `EPROTONOSUPPORT` | Protocol not supported. |
| `EINVAL` | Invalid arguments. |
| `EMFILE` | Too many file descriptors open. |

---

## **🔹 Summary**
| **Step** | **Code** |
|---------|---------|
| Create a **TCP socket** | `socket(AF_INET, SOCK_STREAM, 0);` |
| Create a **UDP socket** | `socket(AF_INET, SOCK_DGRAM, 0);` |
| Create a **Raw socket** | `socket(AF_INET, SOCK_RAW, IPPROTO_RAW);` |
| Use **SOCK_CLOEXEC** for security | `socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);` |
| Use **SOCK_NONBLOCK** for non-blocking mode | `socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);` |

---