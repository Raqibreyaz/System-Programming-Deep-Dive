# **Summary of Internet and UNIX Domain Sockets**

### **1. Internet Domain Sockets 🌍**

- Used for communication over a **TCP/IP network** (local or remote).
- Addressed using an **IP address (IPv4: 32-bit, IPv6: 128-bit) and a port number**.
- Two main types:
  - **Stream Sockets (TCP)** → Reliable, connection-oriented byte-stream communication.
  - **Datagram Sockets (UDP)** → Unreliable, connectionless, message-oriented communication.
- Works across different machines but requires **handling different architectures and data representations**.

---

### **2. Network Byte Order & Data Representation 🔢**

- Different CPU architectures store numbers in either **little-endian** or **big-endian** format.
- **Network Byte Order (Big-Endian)** is used for transferring data over the network.
- Functions like `htons()`, `htonl()`, `ntohs()`, `ntohl()` convert between **host and network byte order**.
- To avoid compatibility issues, many applications use **text-based encoding** (e.g., JSON, XML, CSV).

---

### **3. Internet Socket Addresses 📍**

- **IPv4:** `struct sockaddr_in` contains an IP address (`sin_addr.s_addr`) and port (`sin_port`).
- **IPv6:** `struct sockaddr_in6` supports 128-bit addresses (`sin6_addr`).
- **Generic socket address:** `struct sockaddr` (for both IPv4 & IPv6).
- Conversion functions:
  - `inet_pton()` → Convert string to binary IP.
  - `inet_ntop()` → Convert binary IP to string.

---

### **4. Host and Service Name Resolution 🌐**

- **getaddrinfo()** (modern API) converts **hostname + service name** into socket addresses.
- **getnameinfo()** performs the reverse—converts socket addresses into human-readable host/service names.
- Hostnames are resolved using the **DNS system**.
- `/etc/hosts` (local mappings) and `/etc/resolv.conf` (DNS settings) are used for hostname resolution.

---

### **5. DNS (Domain Name System) 📡**

- Distributed hierarchical system for mapping **human-readable domain names to IP addresses**.
- **Advantages:**
  - Decentralized updates by zone administrators.
  - Redundant and scalable.
  - Allows aliasing (`CNAME` records) and load balancing.
- **DNS lookup process:**
  - Client queries local DNS resolver → If not cached, it queries root servers → Recursive search until resolution.

---

### **6. /etc/services 🗂️**

- Contains mappings of **well-known port numbers** to service names (e.g., `80/tcp → http`).
- Used by `getservbyname()` and `getservbyport()` for translating service names to port numbers and vice versa.

---

### **7. Modern and Obsolete APIs for Host and Service Resolution ⚠️**

| **Function**      | **Status**  | **Purpose**                                         |
| ----------------- | ----------- | --------------------------------------------------- |
| `getaddrinfo()`   | ✅ Modern   | Convert hostname & service name to socket addresses |
| `getnameinfo()`   | ✅ Modern   | Convert socket address to hostname/service name     |
| `gethostbyname()` | ❌ Obsolete | Convert hostname to IP (IPv4 only)                  |
| `gethostbyaddr()` | ❌ Obsolete | Convert IP to hostname                              |
| `getservbyname()` | ⚠️ Legacy   | Convert service name to port                        |
| `getservbyport()` | ⚠️ Legacy   | Convert port to service name                        |

---

### **8. UNIX Domain Sockets vs. Internet Domain Sockets 🖥️**

| **Feature**           | **UNIX Domain Sockets**         | **Internet Domain Sockets**     |
| --------------------- | ------------------------------- | ------------------------------- |
| **Scope**             | Local system only               | Works over networks             |
| **Performance**       | Faster (no network overhead)    | Slower due to network stack     |
| **Security**          | Controlled via file permissions | Needs additional authentication |
| **Data Transmission** | Can pass file descriptors       | Cannot pass file descriptors    |
| **Use Case**          | IPC between local processes     | Network communication           |

- **Use UNIX domain sockets** for **local inter-process communication (IPC)**.
- **Use Internet domain sockets** for **network-based communication**.

---

### **Key Takeaways & Practical Insights 💡**

✅ **Understanding Network Byte Order** is critical when dealing with cross-platform communication.  
✅ **Using `getaddrinfo()` is preferred over obsolete functions (`gethostbyname()` and `gethostbyaddr()`).**  
✅ **Passing file descriptors** via UNIX domain sockets is a powerful mechanism for IPC.  
✅ **Text-based data formats (JSON, XML)** are common for network communication to avoid endian issues.  
✅ **Use `/etc/services` for service name-to-port mappings when writing networked applications.**

---
