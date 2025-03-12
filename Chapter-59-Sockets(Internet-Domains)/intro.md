## **Sockets: Internet Domains (AF_INET & AF_INET6)**
Now that we’ve covered **generic socket concepts** and the **TCP/IP protocol suite**, let’s dive into **programming sockets for IPv4 (AF_INET) and IPv6 (AF_INET6)**.

### **1. Internet Domain Sockets (AF_INET & AF_INET6)**
Sockets in the **Internet domain** use **IP addresses and port numbers** for communication.  
- **IPv4:** Uses `AF_INET` (Address Family - Internet).
- **IPv6:** Uses `AF_INET6` (Address Family - Internet v6).

Each **Internet domain socket** requires:
1. An **IP address** (to identify a machine on a network).
2. A **port number** (to identify a specific service on that machine).

---

### **2. Why Do We Use Hostnames Instead of IP Addresses?**
Computers understand IP addresses in **binary**, but humans prefer **readable names** (like `google.com` instead of `142.250.183.206`).  
We need **mechanisms** to convert between:
- **Hostnames → IP addresses**
- **IP addresses → Hostnames**

✅ **Example:**  
Instead of manually specifying an IP, we can look up `google.com` and get its IP dynamically.

---

### **3. How Are Hostnames Resolved? (DNS - Domain Name System)**
The **Domain Name System (DNS)** is a distributed database that maps:
- **Hostnames → IP addresses** (Forward Lookup)
- **IP addresses → Hostnames** (Reverse Lookup)

💡 **How DNS Works**
1. You type `example.com` in a browser.
2. The computer queries the **DNS server** for the IP.
3. DNS returns the IP (e.g., `93.184.216.34`).
4. The browser uses this IP to **connect to the website**.

✅ **Why DNS Matters in Socket Programming?**
Instead of hardcoding an IP (`192.168.1.10`), we use:
```c
struct addrinfo hints, *res;
getaddrinfo("example.com", "http", &hints, &res);
```
💡 **This ensures flexibility and portability!**

---

### **4. Identifying Services by Port Numbers**
Just like hostnames map to IPs, **service names** map to **port numbers**.

**Examples:**
| Service | Port |
|---------|------|
| HTTP    | 80   |
| HTTPS   | 443  |
| SSH     | 22   |
| FTP     | 21   |

Instead of specifying `port 80`, we can use:
```c
getaddrinfo(NULL, "http", &hints, &res);
```
💡 **This makes code more readable and maintainable!**

---
