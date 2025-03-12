## **1. Client-Server Example with IPV6**
The provided example modifies an existing case-conversion server and client (from an earlier section in the book) to work with **datagram sockets** in the **AF_INET6** domain (**IPv6**).  

The key points to note:
- **Datagram sockets** mean the communication is done using **UDP (User Datagram Protocol).** Unlike TCP, UDP is **connectionless**, meaning the server doesn’t establish a persistent connection with the client.
- **IPv6 support:** The example uses **AF_INET6** instead of **AF_INET** (which is used for IPv4).
- **Conversion of text to uppercase:** The server receives text, converts it to uppercase, and sends it back.

---

## **2. Header File (`i6d_ucase.h`)**
This header file contains essential **definitions** required by both the client (`i6d_ucase_cl.c`) and server (`i6d_ucase_sv.c`).  

### **Code Breakdown**
```c
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tlpi_hdr.h"
```
- **`<netinet/in.h>`** → Provides **Internet address family** structures for both **IPv4 (AF_INET)** and **IPv6 (AF_INET6).**
- **`<arpa/inet.h>`** → Contains functions for **converting** between string and binary representations of IP addresses.
- **`<sys/socket.h>`** → Includes functions and definitions for **creating and working with sockets.**
- **`<ctype.h>`** → Provides **toupper()**, which is used to convert characters to uppercase.
- **`"tlpi_hdr.h"`** → A custom header (from the book) that contains helper functions like **errExit()** for error handling.

### **Definitions**
```c
#define BUF_SIZE 10   /* Maximum size of messages exchanged between client and server */
#define PORT_NUM 50002 /* Server port number */
```
- **`BUF_SIZE`** → Sets the maximum size of messages exchanged **(10 bytes per message).**
- **`PORT_NUM`** → Defines the **port number** (50002) on which the server will **listen** for incoming datagrams.

---

## **3. Server Code (`i6d_ucase_sv.c`)**
This program implements an **IPv6 UDP server** that:
1. **Creates a socket**.
2. **Binds it to a specific port**.
3. **Waits for incoming messages** from clients.
4. **Converts received text to uppercase**.
5. **Sends the modified text back** to the client.

### **Step-by-Step Explanation**
```c
struct sockaddr_in6 svaddr, claddr;
int sfd, j;
ssize_t numBytes;
socklen_t len;
char buf[BUF_SIZE];
char claddrStr[INET6_ADDRSTRLEN];
```
- **`svaddr`** → Structure to store **server’s address**.
- **`claddr`** → Structure to store **client’s address** (received messages).
- **`sfd`** → Socket file descriptor (used to send/receive data).
- **`buf[BUF_SIZE]`** → Buffer to store **incoming data**.
- **`claddrStr[INET6_ADDRSTRLEN]`** → Stores **client’s IPv6 address in string format**.

### **Step 1: Create a Datagram (UDP) Socket**
```c
sfd = socket(AF_INET6, SOCK_DGRAM, 0);
if (sfd == -1)
    errExit("socket");
```
- **`socket(AF_INET6, SOCK_DGRAM, 0);`**  
  - **`AF_INET6`** → Specifies IPv6.
  - **`SOCK_DGRAM`** → Indicates UDP (datagram-based).
  - **`0`** → Default protocol for **UDP**.
- If socket creation **fails**, it prints an **error message** and exits.

### **Step 2: Bind the Socket**
```c
memset(&svaddr, 0, sizeof(struct sockaddr_in6));
svaddr.sin6_family = AF_INET6;
svaddr.sin6_addr = in6addr_any;
svaddr.sin6_port = htons(PORT_NUM);
```
- **`memset()`** → Clears the `svaddr` structure before use.
- **`sin6_family = AF_INET6`** → Specifies IPv6.
- **`sin6_addr = in6addr_any`** → Binds the socket to **all available IPv6 interfaces**.
- **`sin6_port = htons(PORT_NUM)`** → Converts **port number 50002** to **network byte order**.

```c
if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) == -1)
    errExit("bind");
```
- **`bind()`** → Attaches the socket to the **specified port (50002)**.
- If binding fails, it prints an **error message** and exits.

### **Step 3: Receive Messages & Convert to Uppercase**
```c
for (;;) {
    len = sizeof(struct sockaddr_in6);
    numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
    if (numBytes == -1)
        errExit("recvfrom");
```
- **`recvfrom()`** → Waits for a **UDP message**.
- **Stores sender’s address in `claddr`**.

### **Step 4: Display Client Information**
```c
if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr, INET6_ADDRSTRLEN) == NULL)
    printf("Couldn't convert client address to string\n");
else
    printf("Server received %ld bytes from (%s, %u)\n",
            (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
```
- **`inet_ntop()`** → Converts client’s IPv6 address from **binary to string**.
- **`ntohs(claddr.sin6_port)`** → Converts client’s **port number** to **host byte order**.

### **Step 5: Convert Message to Uppercase**
```c
for (j = 0; j < numBytes; j++)
    buf[j] = toupper((unsigned char) buf[j]);
```
- **Converts each character** to **uppercase**.

### **Step 6: Send Response**
```c
if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes)
    fatal("sendto");
```
- **`sendto()`** → Sends **modified message back** to the client.

---

## **4. Client Code (`i6d_ucase_cl.c`)**
The client:
1. **Creates a socket.**
2. **Sends a message to the server.**
3. **Receives the response and prints it.**

### **Step-by-Step Explanation**
```c
struct sockaddr_in6 svaddr;
int sfd, j;
size_t msgLen;
ssize_t numBytes;
char resp[BUF_SIZE];
```
- **Stores server’s address** in `svaddr`.
- **Stores received response** in `resp`.

### **Step 1: Create Socket**
```c
sfd = socket(AF_INET6, SOCK_DGRAM, 0);
if (sfd == -1)
    errExit("socket");
```
- Creates a **UDP socket**.

### **Step 2: Configure Server Address**
```c
memset(&svaddr, 0, sizeof(struct sockaddr_in6));
svaddr.sin6_family = AF_INET6;
svaddr.sin6_port = htons(PORT_NUM);
if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0)
    fatal("inet_pton failed for address '%s'", argv[1]);
```
- **`inet_pton()`** → Converts IPv6 **text address** to **binary format**.

### **Step 3: Send Messages**
```c
for (j = 2; j < argc; j++) {
    msgLen = strlen(argv[j]);
    if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) != msgLen)
        fatal("sendto");
```
- **Sends each message** to the server.

### **Step 4: Receive Response**
```c
numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
if (numBytes == -1)
    errExit("recvfrom");
printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
```
- **Receives and prints response**.

---

## **Summary**
This example shows how **IPv6 datagram sockets (UDP)** work with a **case-conversion** server and client.  
Key takeaways:
- **Connectionless**: No need for explicit `connect()`.
- **Address conversion**: `inet_pton()` and `inet_ntop()`.
- **Ephemeral ports**: Assigned automatically to clients.

Let me know if anything needs more clarification! 🚀