# **📌 Overview of Host and Service Conversion Functions**
### **🔹 Why do we need conversions?**
- **Computers** use **binary** IP addresses and port numbers.
- **Humans** prefer **symbolic names** (e.g., `google.com` instead of `142.250.183.78`).
- **Symbolic names** provide flexibility: The IP address of a server might change, but the hostname remains the same.

### **🔹 Representation of Host Addresses and Ports**
- A **host address** can be:
  1. **Binary (raw memory representation)**
  2. **Symbolic hostname (`google.com`)**
  3. **Presentation format** (IPv4: `192.168.1.1`, IPv6: `2001:db8::1`)
  
- A **port** can be:
  1. **Binary (e.g., 80 stored as `uint16_t`)**
  2. **Symbolic service name (`http`, `ssh`)**

💡 **Conversion functions exist to translate between these representations.**

---

## **📌 Converting IPv4 Addresses Between Binary & Human-Readable Forms**
### **(Obsolete) `inet_aton()` and `inet_ntoa()`**
- **`inet_aton()`** → Converts an IPv4 **dotted-decimal string** (`192.168.1.1`) to **binary**.
- **`inet_ntoa()`** → Converts an IPv4 **binary** address back to **dotted-decimal**.

🚨 **Obsolete:**  
- **These functions only support IPv4** and are **not recommended** for new code.

---

## **📌 Converting IPv4 and IPv6 Addresses**
### **✅ Modern: `inet_pton()` and `inet_ntop()`**
- **`inet_pton(AF_INET, "192.168.1.1", &addr.sin_addr);`**  
  ➝ Converts **string → binary**.
- **`inet_ntop(AF_INET, &addr.sin_addr, str, sizeof(str));`**  
  ➝ Converts **binary → string**.

✅ **Supports both IPv4 and IPv6**.  
✅ **Safe & recommended**.

💡 **Use cases:**
- `inet_pton()` → Convert human-readable IP to binary before using in `sockaddr_in`.
- `inet_ntop()` → Convert binary IP to human-readable format for logging/debugging.

---

## **📌 Converting Host and Service Names to Binary**
### **(Obsolete) `gethostbyname()` and `getservbyname()`**
- **`gethostbyname("example.com")`** → Returns the binary **IP address** of a hostname.
- **`getservbyname("http", "tcp")`** → Returns the **port number** of a service.

🚨 **Obsolete because:**
1. **Only supports IPv4**.
2. **Not thread-safe**.
3. **Replaced by `getaddrinfo()` and `getnameinfo()` (modern APIs).**

---

## **📌 Modern: `getaddrinfo()` and `getnameinfo()`**
### ✅ **`getaddrinfo()`**
- Replaces **both `gethostbyname()` and `getservbyname()`**.
- Resolves **hostnames + service names** into **binary IP + port**.
- **Supports IPv4 & IPv6**.

**Example:**
```c
struct addrinfo hints, *res;
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_UNSPEC;  // Supports both IPv4 & IPv6
hints.ai_socktype = SOCK_STREAM;

getaddrinfo("example.com", "http", &hints, &res);
```

---

### ✅ **`getnameinfo()`**
- Converts **binary IP + port → hostname + service name**.
- Reverse of `getaddrinfo()`.

**Example:**
```c
char host[NI_MAXHOST], service[NI_MAXSERV];
getnameinfo((struct sockaddr *)&addr, sizeof(addr), host, sizeof(host), service, sizeof(service), 0);
printf("Hostname: %s, Service: %s\n", host, service);
```

---

## **📌 DNS and `/etc/services`**
- **DNS** → Maps hostnames (`example.com`) to IP addresses (`93.184.216.34`).
- **`/etc/services`** → Maps **port numbers** to **service names** (`80 → http`).

These provide an **abstraction layer**, so we don’t have to deal with raw numbers.

---

## **📌 Summary**
| **Function**           | **Purpose**                                  | **IPv4/IPv6?** | **Modern?** |
|-----------------------|----------------------------------|------------|----------|
| `inet_aton()` / `inet_ntoa()`  | Convert IPv4 between binary & string | IPv4 only | ❌ (Obsolete) |
| `inet_pton()` / `inet_ntop()`  | Convert IPv4/IPv6 between binary & string | IPv4 + IPv6 | ✅ (Modern) |
| `gethostbyname()` / `gethostbyaddr()`  | Convert hostname ↔ binary IP | IPv4 only | ❌ (Obsolete) |
| `getservbyname()` / `getservbyport()`  | Convert service name ↔ port | IPv4 only | ❌ (Obsolete) |
| `getaddrinfo()`  | Convert hostname + service name → binary IP + port | IPv4 + IPv6 | ✅ (Modern) |
| `getnameinfo()`  | Convert binary IP + port → hostname + service name | IPv4 + IPv6 | ✅ (Modern) |

---

## **📌 Final Takeaways**
- ✅ **Use `inet_pton()` / `inet_ntop()`** for **IP ↔ binary conversion**.
- ✅ **Use `getaddrinfo()` / `getnameinfo()`** for **hostname/service ↔ binary conversion**.
- 🚨 **Avoid obsolete functions (`inet_aton()`, `gethostbyname()`, etc.)**.

---