## **Internet Domain Sockets (IPv4 & IPv6)**
Internet domain sockets allow communication between processes **over a network** using **IP addresses and port numbers**. They are categorized based on the transport protocol they use:

1. **Stream Sockets (TCP - Transmission Control Protocol)**
2. **Datagram Sockets (UDP - User Datagram Protocol)**

---

## **1. Stream Sockets (TCP)**
✅ **Characteristics:**
- Built on **TCP (Transmission Control Protocol)**.
- **Reliable:** Guarantees data delivery.
- **Connection-oriented:** Requires a **handshake** before communication.
- **Byte-stream:** Data is sent **in order** and without duplication.

✅ **When to Use?**
- When **data integrity and order** matter (e.g., **web browsing, file transfers**).
- Example: **HTTP, SSH, FTP** use TCP.

### **TCP Server-Client Model**
| Step | Server (`bind`, `listen`, `accept`) | Client (`connect`) |
|------|----------------------------------|------------------|
| 1    | Create socket (`socket()`) | Create socket (`socket()`) |
| 2    | Bind socket to an IP and port (`bind()`) | Connect to server (`connect()`) |
| 3    | Listen for connections (`listen()`) | — |
| 4    | Accept connection (`accept()`) | — |
| 5    | Send & receive data (`send()`, `recv()`) | Send & receive data (`send()`, `recv()`) |
| 6    | Close connection (`close()`) | Close connection (`close()`) |

---

## **2. Datagram Sockets (UDP)**
✅ **Characteristics:**
- Built on **UDP (User Datagram Protocol)**.
- **Unreliable:** No guarantee of delivery, order, or duplication handling.
- **Connectionless:** No handshake, just send & receive.
- **Faster but riskier** (suitable for real-time applications).

✅ **When to Use?**
- When **speed is more important than reliability** (e.g., **video streaming, VoIP, DNS, online gaming**).
- Example: **DNS, DHCP, VoIP** use UDP.

### **Key Differences from UNIX Domain Datagram Sockets**
| Feature | UNIX Domain Datagram Sockets | Internet Domain Datagram Sockets (UDP) |
|---------|-----------------------------|---------------------------------------|
| **Reliability** | **Reliable** | **Unreliable** (Packets can be lost) |
| **Blocking** | Blocks if receiver's queue is full | Dropped silently if queue is full |
| **Connection** | Connectionless | Connectionless |

---

## **3. Handling Data Loss in UDP**
Since **UDP drops packets** when the receiver’s buffer is full, we must **handle losses manually**:
1. **Use timeouts & retransmission** (`setsockopt()` for timeouts).
2. **Acknowledge packets** (e.g., client sends a response when it receives data).
3. **Sequence numbers** (to reorder out-of-order packets).
4. **Error detection** (UDP has a checksum, but it’s weak).

✅ **Example Scenario:**
- **TCP (Reliable):** Sending an email (must not lose data).
- **UDP (Unreliable):** Streaming video (occasional lost frames are okay).

---

## **4. Summary**
| Feature  | TCP (Stream Socket) | UDP (Datagram Socket) |
|----------|-------------------|---------------------|
| **Reliability** | **Yes** | **No** |
| **Connection-Oriented?** | **Yes** (`connect()`) | **No** (`sendto()` & `recvfrom()`) |
| **Ordered Delivery?** | **Yes** | **No** (out of order possible) |
| **Use Cases** | **Web, file transfer, SSH** | **Gaming, VoIP, DNS** |
| **Blocking?** | **Yes, waits for data** | **No, drops packets silently** |
