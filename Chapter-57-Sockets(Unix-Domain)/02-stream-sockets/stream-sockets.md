## **Stream Sockets in the UNIX Domain**  

This example demonstrates **inter-process communication (IPC)** using **stream sockets in the UNIX domain** (as opposed to internet domain sockets like TCP/IP). The UNIX domain is used for local communication between processes on the same machine.  

The example consists of:  
1. **Server (`us_xfr_sv.c`)** – Listens for connections and writes received data to `stdout`.  
2. **Client (`us_xfr_cl.c`)** – Reads data from `stdin` and sends it to the server.  

---

### **Understanding the Header File (`us_xfr.h`)**
```c
#include <sys/un.h>    // UNIX domain sockets
#include <sys/socket.h> // Socket API
#include "tlpi_hdr.h"  // Error-handling functions (assumed from TLPI book)

#define SV_SOCK_PATH "/tmp/us_xfr" // Server socket path in the file system
#define BUF_SIZE 100               // Buffer size for data transfer
```
- **`sys/un.h`** → Defines structures for UNIX domain sockets.
- **`sys/socket.h`** → Contains socket-related system calls.
- **`SV_SOCK_PATH`** → File path used for the UNIX domain socket (instead of an IP address).
- **`BUF_SIZE`** → Buffer size for reading and writing data.

---

## **Server (`us_xfr_sv.c`) – Step-by-Step Breakdown**
```c
#define BACKLOG 5 // Max pending connections
```
- Defines how many client connections can be queued before being processed.

### **1. Create a UNIX domain socket**
```c
sfd = socket(AF_UNIX, SOCK_STREAM, 0);
if (sfd == -1)
    errExit("socket");
```
- `AF_UNIX` → Specifies a UNIX domain socket (not an internet socket).
- `SOCK_STREAM` → Uses a stream-based socket (TCP-like, but local).
- `0` → Default protocol for stream sockets.

### **2. Remove existing socket file (if any)**
```c
if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    errExit("remove-%s", SV_SOCK_PATH);
```
- If a previous server instance left a socket file at `/tmp/us_xfr`, `bind()` would fail.
- `remove()` ensures no leftover socket file.

### **3. Bind socket to a file path**
```c
memset(&addr, 0, sizeof(struct sockaddr_un)); // Zero out struct
addr.sun_family = AF_UNIX; // Set address family
strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1); // Set socket path

if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
    errExit("bind");
```
- Initializes `sockaddr_un` structure.
- Sets `sun_family` to `AF_UNIX` for UNIX domain sockets.
- `bind()` associates the socket with `/tmp/us_xfr`.

### **4. Listen for incoming connections**
```c
if (listen(sfd, BACKLOG) == -1)
    errExit("listen");
```
- Marks the socket as a **listening** socket.
- `BACKLOG` defines how many pending connections the kernel should queue.

### **5. Accept connections in a loop**
```c
for (;;) { // Infinite loop to handle clients
    cfd = accept(sfd, NULL, NULL);
    if (cfd == -1)
        errExit("accept");
```
- `accept()` waits for a client and returns a new socket `cfd` for communication.

### **6. Read data from client and write to stdout**
```c
while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
    if (write(STDOUT_FILENO, buf, numRead) != numRead)
        fatal("partial/failed write");
```
- Reads from `cfd` (client) and writes to `stdout` (console or redirected file).

### **7. Close client connection**
```c
if (close(cfd) == -1)
    errMsg("close");
```
- Closes the connected client socket.
- The server continues waiting for new clients.

---

## **Client (`us_xfr_cl.c`) – Step-by-Step Breakdown**

### **1. Create a UNIX domain socket**
```c
sfd = socket(AF_UNIX, SOCK_STREAM, 0);
if (sfd == -1)
    errExit("socket");
```
- Same as the server, but used as a **client socket**.

### **2. Connect to the server**
```c
memset(&addr, 0, sizeof(struct sockaddr_un)); // Zero out struct
addr.sun_family = AF_UNIX;
strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
    errExit("connect");
```
- Specifies the same **socket file** (`/tmp/us_xfr`) used by the server.
- `connect()` attempts to establish a connection.

### **3. Send data from stdin to server**
```c
while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    if (write(sfd, buf, numRead) != numRead)
        fatal("partial/failed write");
```
- Reads from `stdin` and writes to the socket (`sfd`).
- The server reads this data and writes it to its `stdout`.

### **4. Exit after reaching EOF**
```c
exit(EXIT_SUCCESS);
```
- Closes the socket automatically when the process exits.

---

## **Running the Programs**
### **1. Start the server**
```sh
$ ./us_xfr_sv > output.txt &
```
- Runs in the background (`&`).
- Redirects received data to `output.txt`.

### **2. Start the client**
```sh
$ ./us_xfr_cl < input.txt
```
- Reads data from `input.txt` and sends it to the server.

### **3. Verify data transfer**
```sh
$ diff input.txt output.txt
```
- If there’s no output, the files match.

### **4. Clean up**
```sh
$ kill %1  # Stop the server
```
- Terminates the server.

---

## **Key Takeaways**
1. **UNIX domain sockets** allow **fast** IPC between processes on the same machine.
2. The **server** creates, binds, and listens on a **socket file**.
3. The **client** connects using the same socket file.
4. **Data is transferred using `read()` and `write()`**.
5. The **server removes old socket files** to avoid `bind()` errors.

---
