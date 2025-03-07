# **🔹 What is a Socket?**  
A **socket** is an endpoint for communication between two programs. It enables communication between processes on **the same system** or over a **network** (like the Internet).  

💡 **Analogy:**  
- Imagine a **socket** like a **phone**.  
- If two people **want to talk**, they need:  
  - A **phone (socket)** 📞  
  - A **phone number (IP + Port)** ☎️  
  - A way to **call and respond (send/receive data)** 💬  

💡 **Why are sockets important?**  
Without sockets, computers wouldn't be able to:  
✅ Load web pages  
✅ Download files  
✅ Stream videos  
✅ Play multiplayer games  
✅ Communicate between services  

---

# **🔹 Why Do We Need Sockets?**
Before sockets, computers had **limited ways** to communicate:  
- **Pipes (`pipe()`)** → Can only be used between related processes on the same machine.  
- **Shared Memory (`mmap()`)** → Requires synchronization, complex to manage.  
- **Message Queues (`mq_*`)** → IPC (Inter-Process Communication), but not for networking.  

✅ **Sockets provide a simple way to communicate over a network or between programs!**  

---

# **📌 Socket Types**

### **🔹 Key Terminologies You Must Know**  
Before we explore socket types, let's break down key **networking & OS terms**:  

| **Term** | **Meaning** |
|----------|------------|
| **File Descriptor (FD)** | A number representing an open socket (like an open file). |
| **Socket Buffer** | Memory in the kernel where incoming/outgoing data is temporarily stored. |
| **Port** | A **logical communication endpoint** (e.g., 80 for HTTP, 22 for SSH). |
| **Protocol** | The rules of communication (e.g., TCP, UDP, ICMP). |
| **Address Family** | Defines how the socket interprets addresses (IPv4, IPv6, Unix domain). |
| **Transport Layer** | Layer handling actual transmission (TCP, UDP). |
| **Blocking vs Non-blocking Sockets** | Blocking waits for data, non-blocking returns immediately. |
| **Datagram** | A self-contained packet of data (used in UDP). |
| **Connection-oriented vs Connectionless** | TCP needs a handshake (connection), UDP doesn’t. |

---

# **📌 1️⃣ Stream Sockets (`SOCK_STREAM`) – Reliable Byte Stream**
### **💡 Definition:**  
- Uses **TCP (Transmission Control Protocol)**.  
- **Connection-oriented** → Requires a **3-way handshake** before data transfer.  
- **Reliable & Ordered** → Ensures data **arrives correctly and in order**.  
- **Byte Stream** → The OS treats data as a **continuous stream, not discrete packets**.  

### **🔹 Key OS Terminologies in Stream Sockets**
| **Term** | **Meaning** |
|----------|------------|
| **Three-Way Handshake** | TCP establishes a connection using `SYN`, `SYN-ACK`, `ACK`. |
| **Flow Control** | The OS prevents overwhelming the receiver by adjusting data speed. |
| **Congestion Control** | OS manages network traffic to avoid packet loss. |
| **Socket Buffers (`SO_RCVBUF`, `SO_SNDBUF`)** | Kernel memory for storing incoming/outgoing data. |
| **Backlog Queue** | Holds pending connections before they are `accept()`ed. |

### **📌 OS-Level Example: Web Server (Apache, Nginx)**
**How does the OS manage an HTTP request?**  
1. The OS **creates a listening socket** (`socket()` with `SOCK_STREAM`).  
2. A browser sends a **TCP SYN request** to open a connection.  
3. The OS **queues** the request in the **backlog queue** (managed by `listen()`).  
4. The server calls `accept()`, which removes the connection from the queue.  
5. The OS sends **data in TCP segments** stored in the **send buffer (`SO_SNDBUF`)**.  
6. The OS manages **flow control & congestion control**.  
7. When the response is complete, the server **closes** the socket (`close()`).  

💡 **Why TCP?** Because HTTP needs **complete and ordered data delivery**.

---

# **📌 2️⃣ Datagram Sockets (`SOCK_DGRAM`) – Fast, Unreliable**
### **💡 Definition:**  
- Uses **UDP (User Datagram Protocol)**.  
- **Connectionless** → No **handshake** required, just send data!  
- **Unreliable & Out of Order** → The OS **does not** guarantee delivery.  
- **Maintains Message Boundaries** → Each `send()` = one **datagram** received.  

### **🔹 Key OS Terminologies in Datagram Sockets**
| **Term** | **Meaning** |
|----------|------------|
| **Datagram** | A self-contained packet with source/destination info. |
| **Checksum** | Used for error detection in UDP packets. |
| **Packet Loss** | When a datagram never arrives at its destination. |
| **No Flow Control** | The OS does not prevent sender from overloading receiver. |

### **📌 OS-Level Example: DNS Query**
1. The OS **creates a UDP socket** (`socket(AF_INET, SOCK_DGRAM, 0)`).  
2. It sends a **single UDP datagram** to the DNS server.  
3. The OS receives the response or **retries if lost**.  
4. The OS **closes the socket after sending** (no connection needed).  

💡 **Why UDP?** Because DNS queries are **small and speed is more important than reliability**.

---

# **📌 3️⃣ Unix Domain Sockets (`AF_UNIX`) – Fast Local IPC**
### **💡 Definition:**  
- Works **inside the same OS**, not over a network.  
- **Faster than TCP/UDP** because it avoids **network overhead**.  
- Used for **Inter-Process Communication (IPC)**.  

### **🔹 Key OS Terminologies in Unix Sockets**
| **Term** | **Meaning** |
|----------|------------|
| **Inter-Process Communication (IPC)** | Mechanism for processes to communicate within OS. |
| **File Descriptor Passing** | Unix sockets allow sharing of FDs between processes. |
| **Path-Based Sockets** | Can be addressed using file paths (`/var/run/docker.sock`). |

### **📌 OS-Level Example: Docker Daemon**
1. Docker **daemon listens on a Unix socket** (`/var/run/docker.sock`).  
2. When you run `docker ps`, your shell **opens a Unix socket**.  
3. The OS **sends and receives** data between the client and daemon.  

💡 **Why Unix Sockets?** Because they avoid **network overhead**, making IPC **faster**.

---

# **📌 4️⃣ Raw Sockets (`SOCK_RAW`) – Low-Level Network Control**
### **💡 Definition:**  
- Bypasses **TCP/UDP processing** and works at **packet level**.  
- Used for **custom protocols, sniffing, or hacking tools**.  
- Requires **root privileges** due to security risks.  

### **🔹 Key OS Terminologies in Raw Sockets**
| **Term** | **Meaning** |
|----------|------------|
| **Packet Sniffing** | Capturing raw network packets (e.g., Wireshark). |
| **ICMP (Internet Control Message Protocol)** | Used for network diagnostics (e.g., `ping`). |
| **Promiscuous Mode** | OS captures all packets, not just ones meant for the machine. |

### **📌 OS-Level Example: `ping` Command**
1. The OS **creates a raw socket** (`SOCK_RAW, IPPROTO_ICMP`).  
2. It sends an **ICMP Echo Request** to the target.  
3. If the target replies, the OS **captures** the ICMP Echo Reply.  

💡 **Why Raw Sockets?** Because ICMP **does not use TCP or UDP**, so the OS must **manually handle it**.

---

# **📌 5️⃣ Connection-Oriented vs Connectionless Sockets**
| **Feature** | **Connection-Oriented (TCP)** | **Connectionless (UDP)** |
|------------|----------------------|----------------------|
| **Setup Required?** | ✅ Yes (handshake) | ❌ No |
| **Reliability** | ✅ Yes (ACKs) | ❌ No |
| **Order Guaranteed?** | ✅ Yes | ❌ No |
| **Use Case** | Web, SSH, FTP | VoIP, DNS, Gaming |

---

# **📌 Conclusion**
- **TCP (`SOCK_STREAM`)** → **Reliable**, ordered, connection-based.  
  - Used for **web servers, SSH, file transfers**.  
- **UDP (`SOCK_DGRAM`)** → **Fast**, connectionless, **unreliable**.  
  - Used for **VoIP, DNS, gaming**.  
- **Unix Sockets (`AF_UNIX`)** → **Fast local communication** (IPC).  
  - Used for **Docker, Linux GUI apps**.  
- **Raw Sockets (`SOCK_RAW`)** → **Packet-level control**.  
  - Used for **network monitoring (`tcpdump`), hacking tools**.  
---
# **🔹 The Two Main Players: Server & Client**  
A socket-based connection needs **two sides**:  
1. **The server (waits for connections) 📞**  
2. **The client (initiates the connection) ☎️**  

💡 **How does this work?**  
- The **server** listens for incoming connections.  
- The **client** connects to the server.  
- They **exchange messages** using sockets.  

---

# **🔹 Steps to Establish a Connection**
| **Step** | **Server** 🖥️ | **Client** 💻 |
|---------|--------------|--------------|
| **1** | Create a socket (`socket()`) | Create a socket (`socket()`) |
| **2** | Bind the socket to an address (`bind()`) | — |
| **3** | Listen for connections (`listen()`) | — |
| **4** | Accept connections (`accept()`) | Connect to server (`connect()`) |
| **5** | Send and receive data (`send()`, `recv()`) | Send and receive data (`send()`, `recv()`) |

---

# **🔹 Creating a Socket (`socket()`)**
A socket is created using the `socket()` system call:  
```c
int fd = socket(domain, type, protocol);
```

### **🔷 Parameters of `socket()`**
| **Parameter** | **Purpose** |
|-------------|------------|
| `domain` | Specifies the communication domain (IPv4, IPv6, local) |
| `type` | Defines how data is sent (TCP, UDP) |
| `protocol` | Usually `0` (OS selects the right protocol) |

💡 **Think of `socket()` like buying a phone:**  
- **Domain (`AF_INET`)** → Choose a local or international plan.  
- **Type (`SOCK_STREAM` or `SOCK_DGRAM`)** → Pick a calling method (reliable or fast).  
- **Protocol (`0`)** → OS automatically picks the right settings.  

---

# **🔹 Choosing the Right Address (`domain`)**
A **domain** tells the OS *where* the socket will communicate.

| **Domain**  | **Used For** | **Example** |
|------------|-------------|------------|
| **AF_UNIX** | Inside the same system | IPC (Inter-Process Communication) |
| **AF_INET** | IPv4 internet communication | Websites, APIs |
| **AF_INET6** | IPv6 internet communication | Future-proof web apps |

💡 **Analogy:**  
- **AF_UNIX** = Talking within your house (local communication).  
- **AF_INET** = Calling someone in another city (network communication).  

---

# **🔹 TCP vs. UDP (Choosing the Right `type`)**
| **Type** | **Reliability** | **Ordering** | **Speed** | **Use Case** |
|---------|--------------|------------|----------|-------------|
| **TCP (`SOCK_STREAM`)** | ✅ Reliable | ✅ Ordered | 🐢 Slower | Websites, Emails, File Transfers |
| **UDP (`SOCK_DGRAM`)** | ❌ Not Reliable | ❌ Unordered | ⚡ Faster | Live Streaming, Online Games, VoIP |

💡 **TCP is like a phone call** (ensures message delivery, but slower).  
💡 **UDP is like sending a letter** (fast, but no guarantee of delivery).  

---

# **🔹 Server Code – Step by Step**
💻 **A simple TCP server:**
```c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);  // 1. Create socket
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));  // 2. Bind socket
    listen(server_fd, 5);  // 3. Listen for connections
    
    int client_fd = accept(server_fd, NULL, NULL);  // 4. Accept connection
    send(client_fd, "Hello, Client!", 14, 0);  // 5. Send data
}
```

---

# **🔹 Client Code – Step by Step**
💻 **A simple TCP client:**
```c
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);  // 1. Create socket
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));  // 2. Connect to server
    send(client_fd, "Hello, Server!", 14, 0);  // 3. Send data
}
```

---

# **🔹 Sending & Receiving Data**
### **🔷 TCP (Reliable Communication)**
```c
send(fd, message, size, 0); // Send data
recv(fd, buffer, size, 0);  // Receive data
```

### **🔷 UDP (Fast Communication)**
```c
sendto(fd, message, size, 0, (struct sockaddr*)&addr, addr_len);
recvfrom(fd, buffer, size, 0, (struct sockaddr*)&addr, &addr_len);
```

💡 **TCP keeps a continuous connection, UDP sends and forgets.**

---

# **🔹 Summary**
1. **Sockets allow programs to talk to each other.**  
2. **A server must start before a client can connect.**  
3. **Sockets can be for local (`AF_UNIX`) or internet (`AF_INET`) communication.**  
4. **TCP (`SOCK_STREAM`) is reliable, UDP (`SOCK_DGRAM`) is fast.**  
5. **Use `fcntl()` to make sockets non-blocking.**  

---