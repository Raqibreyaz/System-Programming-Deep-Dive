# **Understanding UNIX Domain Socket Addresses (`struct sockaddr_un`)**
When working with **UNIX domain sockets**, we need an **address** to identify the socket.  
In UNIX domain sockets, the address is a **pathname** (a string representing a file path in the filesystem).  

To work with this, the **socket address structure** is defined as:  
```c
struct sockaddr_un {
    sa_family_t sun_family;   /* Always AF_UNIX */
    char sun_path[108];       /* Null-terminated socket pathname */
};
```
Let’s break this down:

### **Fields in `sockaddr_un`**
1. **`sun_family`**:  
   - This is **always set to `AF_UNIX`**, meaning it’s a UNIX domain socket.
2. **`sun_path`**:  
   - This is the **file path** where the socket exists in the filesystem.  
   - This is a **null-terminated string** (just like a filename in UNIX).
   - The maximum length **varies** across systems:
     - **108 bytes** (Linux, most BSD systems)
     - **104 bytes** (Old BSD versions)
     - **92 bytes** (HP-UX 11)
   - Since this length is not fixed across all systems, we should **limit our usage** to **92 bytes** for maximum portability.

### **Example**
```c
struct sockaddr_un addr;
addr.sun_family = AF_UNIX;
strncpy(addr.sun_path, "/tmp/mysock", sizeof(addr.sun_path) - 1);
```
- Here, we set `sun_family` to `AF_UNIX` (because it's a UNIX domain socket).  
- We copy `"/tmp/mysock"` into `sun_path`, **leaving one byte for the null terminator**.

---

## **Binding a UNIX Domain Socket**
To use a **UNIX domain socket**, we need to:
1. **Create a socket**
2. **Bind it to a pathname**  
3. **Ensure proper cleanup (unlinking when done)**

Here’s how we **bind** a UNIX domain socket:  

### **Example Code (Binding a UNIX Domain Socket)**
```c
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKNAME "/tmp/mysock"

int main() {
    int sfd;
    struct sockaddr_un addr;

    /* Step 1: Create the socket */
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Step 2: Initialize the sockaddr_un structure */
    memset(&addr, 0, sizeof(struct sockaddr_un)); /* Zero out structure */
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) - 1);

    /* Step 3: Bind the socket */
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Socket bound to %s\n", SOCKNAME);

    /* Clean up */
    close(sfd);
    unlink(SOCKNAME); /* Remove the socket file when done */
    
    return 0;
}
```
---

## **Why We Use `memset()` to Initialize `sockaddr_un`**
In the above example, before setting values in `addr`, we **clear the structure**:
```c
memset(&addr, 0, sizeof(struct sockaddr_un));
```
### **Why is this important?**
1. **Ensures all fields are initialized to zero**  
   - `memset()` makes sure all bytes in `sockaddr_un` are set to `0`, avoiding **uninitialized memory issues**.
2. **Handles non-standard extra fields**  
   - Some UNIX systems **add extra fields** in `sockaddr_un`. By zeroing the entire structure, we ensure any **extra** fields are properly initialized.
3. **Avoids unintended behavior**  
   - Without `memset()`, uninitialized memory might cause unexpected behavior when binding.

### **Alternative: `bzero()` (Why We Don’t Use It)**
- **`bzero()`** is an older function similar to `memset()` but is considered **legacy**.
- **Why avoid `bzero()`?**
  - It was marked **legacy in SUSv3** (Single UNIX Specification version 3).
  - **SUSv4 completely removed it**.
  - **Recommended alternative:** `memset()` (which is standard and works the same way).

### **Why Not Initialize Fields Manually?**
Instead of `memset()`, we could do:
```c
struct sockaddr_un addr;
addr.sun_family = AF_UNIX;
addr.sun_path[0] = '\0';
```
**Why is this not ideal?**
- This only initializes **some** fields.  
- If there are **extra fields** (in some systems), they remain **uninitialized**.  
- `memset()` ensures that everything is **properly set to zero**.

---

## **Key Notes on Binding a UNIX Domain Socket**
1. **Cannot Bind to an Existing Path**
   - If the file (socket pathname) **already exists**, `bind()` **fails with `EADDRINUSE`**.
   - Solution: **Remove (`unlink()`) the existing socket file before binding.**
     ```c
     unlink(SOCKNAME);
     ```
   
2. **Absolute vs. Relative Pathnames**
   - **Absolute pathnames (`/tmp/mysock`) are preferred.**
   - Using a **relative pathname (`mysock`) is possible**, but:
     - The **connecting process must know the working directory** of the server.
     - This is rarely practical.

3. **Each Socket is Bound to One Pathname**
   - A socket **can only be bound to one pathname**.
   - Likewise, **a pathname can only be bound to one socket**.

4. **Cannot Use `open()` to Open a Socket**
   - Even though a UNIX domain socket **looks like a file**, you **cannot** use `open()` to open it.
   - You must use **`socket()`** and **`connect()`** for communication.

5. **Removing the Socket File When Done**
   - When the server stops, the socket file **should be deleted** to prevent issues.
   - Use:
     ```c
     unlink(SOCKNAME);
     ```
   - If you forget to remove it, **next time you run the server, `bind()` may fail** because the file already exists.

---

## **How UNIX Domain Sockets Appear in the Filesystem**
When a UNIX domain socket is created in the filesystem:
- It appears as a **special file**.
- `ls -l` shows **type `s`** (for **socket**).
- `ls -F` appends an **`=`** to indicate it’s a socket.

**Example:**
```sh
$ ls -l /tmp/mysock
srwxr-xr-x 1 user user 0 Feb 26 12:00 /tmp/mysock=
```
- **`s` at the start**: This is a socket file.
- **`=` at the end**: Indicates a socket (when using `ls -F`).

---

## **Security Concerns**
- **Avoid using `/tmp` for sockets in real-world applications.**
  - `/tmp` is a **publicly writable directory**, meaning **anyone can create a file with the same name** as your socket.
  - This can lead to **denial-of-service (DoS) attacks**.

**Better approach:**
- Create the socket in a **private, secured directory**:
  ```sh
  mkdir -p /run/myapp
  chmod 700 /run/myapp
  ```
  Then use:
  ```c
  strncpy(addr.sun_path, "/run/myapp/mysock", sizeof(addr.sun_path) - 1);
  ```

---

## **Final Thoughts**
- UNIX domain sockets **use file pathnames** as addresses.
- Always **zero out the structure** with `memset()`.
- **Unlink (`unlink()`)** the socket file before binding **to prevent errors**.
- **Use absolute paths**, not relative ones.
- **Don’t place sockets in `/tmp`** for security reasons.