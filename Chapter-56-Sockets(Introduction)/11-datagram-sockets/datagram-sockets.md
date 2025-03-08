# Datagram Sockets: A Simple and Detailed Explanation

## 1. Introduction to Datagram Sockets

Datagram sockets provide a **connectionless** method of communication over a network. Unlike stream sockets (TCP), datagram sockets (UDP) do not establish a persistent connection between sender and receiver. Instead, each message (datagram) is sent independently, like mailing letters.

### Real-World Analogy: Postal System
Imagine you are sending postcards to a friend. Each postcard:
- Is **sent independently** (no established connection like a phone call).
- Might **arrive out of order** or get **lost in transit**.
- Does not require a direct connection to the recipient.
- Includes **both sender and recipient addresses** so the recipient knows where to reply.

This is exactly how UDP-based **datagram sockets** work.

---
## 2. Key Features of Datagram Sockets

1. **Connectionless Communication**  
   - No need to establish a connection before sending data.
   - Each packet is treated independently.

2. **Unreliable but Fast**  
   - Packets **may be lost, duplicated, or arrive out of order**.
   - No automatic retransmission like TCP.
   - Useful for real-time applications (video streaming, gaming).

3. **Datagrams Are Self-Contained**  
   - Each datagram includes **both the sender and receiver's address**.
   - This allows communication with multiple peers.

---
## 3. Essential System Calls for Datagram Sockets

| Function | Purpose |
|----------|---------|
| `socket()` | Creates a socket |
| `bind()` | Assigns an address to the socket (mainly for servers) |
| `sendto()` | Sends a datagram to a specific address |
| `recvfrom()` | Receives a datagram and retrieves sender information |
| `connect()` | Optionally binds a socket to a peer for simpler I/O |
| `close()` | Closes the socket |

---

![UDP](./assets/datagram-sockets.png)

## 4. Steps to Use Datagram Sockets

### **Step 1: Create a Socket**
```c
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
```
This creates a UDP socket.

### **Step 2: Bind the Socket (For Servers)**
```c
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);
server_addr.sin_addr.s_addr = INADDR_ANY;

bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
```
Servers bind to a well-known address so clients can send data to them.

### **Step 3: Send Data (Using `sendto()`)**
```c
char message[] = "Hello, Client!";
struct sockaddr_in client_addr;
client_addr.sin_family = AF_INET;
client_addr.sin_port = htons(9000);
inet_pton(AF_INET, "192.168.1.100", &client_addr.sin_addr);

sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
```
Each `sendto()` call requires the recipient’s address, just like writing a letter.

### **Step 4: Receive Data (Using `recvfrom()`)**
```c
char buffer[1024];
struct sockaddr_in sender_addr;
socklen_t addr_len = sizeof(sender_addr);

recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&sender_addr, &addr_len);
printf("Received: %s\n", buffer);
```
`recvfrom()` retrieves the sender’s address, allowing a response to be sent.

### **Step 5: Close the Socket**
```c
close(sockfd);
```

---
## 5. Understanding `connect()` with Datagram Sockets

Normally, `sendto()` requires specifying the recipient’s address for every message. However, calling `connect()` on a UDP socket allows using `write()` and `read()` instead.

```c
connect(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr));
write(sockfd, "Hello", 5);
read(sockfd, buffer, sizeof(buffer));
```
- **Simplifies the code** when communicating with one peer.
- **Does not establish a persistent connection** like TCP.
- **Only messages from the connected peer are received**.

---
## 6. Advantages and Use Cases of Datagram Sockets

### **Advantages**
✅ Low latency (ideal for real-time applications).  
✅ Supports **broadcasting and multicasting**.  
✅ Lightweight and **efficient for short messages**.  

### **Use Cases**
✔ Online gaming (fast but unreliable).  
✔ Live streaming (can tolerate lost packets).  
✔ DNS lookups (quick query-response communication).  
✔ IoT devices (small data packets, low overhead).  

---
## 7. Key Takeaways

- **UDP = Fast but unreliable communication.**
- **Each packet is independent**, like mailing letters.
- **`sendto()` and `recvfrom()` require explicit addresses.**
- **Calling `connect()` simplifies code but doesn’t create a connection.**
- **Good for real-time apps, DNS, and lightweight data transmission.**

---
## 8. Example: A Simple UDP Client and Server

### **Server Code** (Receives messages and responds)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    while (1) {
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
        printf("Client: %s\n", buffer);
        sendto(sockfd, "Hello from server", 17, 0, (struct sockaddr*)&client_addr, addr_len);
    }
    close(sockfd);
}
```

### **Client Code** (Sends messages and receives responses)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;
    char buffer[1024] = "Hello, Server!";
    socklen_t addr_len = sizeof(server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len);
    recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    printf("Server: %s\n", buffer);
    close(sockfd);
}
```

---
## 9. Conclusion
- Datagram sockets provide **fast, lightweight communication**.
- Ideal for **real-time applications** where some data loss is acceptable.
- `connect()` simplifies code but **doesn’t guarantee delivery**.
- **Perfect for applications like gaming, video streaming, and DNS lookups.**

---