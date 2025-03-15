# **🔹 `getnameinfo()` – The Reverse of `getaddrinfo()`**
Imagine you’re given a **raw IP address** and a **port number**, and you want to find out:  
📌 What **hostname** (like `google.com`) does this IP belong to?  
📌 What **service name** (like `http`, `ssh`, or `ftp`) corresponds to the port number?  

That's exactly what `getnameinfo()` does—it takes a **binary socket address** and converts it into **human-readable** host and service names.

---

## **🔹 Why Use `getnameinfo()`?**
Let's say you’re running a **server**, and a client connects to you. You only get their **IP address** and **port** from `accept()`. But wouldn’t it be nice to display their **hostname** instead of just an IP?

👉 Instead of `192.168.1.100:443`, you’d see `example.com:https`.  
👉 Instead of `8.8.8.8:53`, you’d see `dns.google:dns`.

That’s what `getnameinfo()` helps with! 🎯  

---

## **🔹 Function Prototype**
```c
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const struct sockaddr *addr, socklen_t addrlen,
                char *host, size_t hostlen,
                char *service, size_t servlen, int flags);
```
**Returns**:  
✅ `0` on success.  
❌ A **nonzero error code** on failure (use `gai_strerror()` to get the error message).

---

## **🔹 Understanding the Parameters**
Let’s break down the function parameters **with real-world examples**:

| **Parameter** | **What It Represents** | **Real-World Example** |
|--------------|------------------------|------------------------|
| `addr` | Pointer to a **socket address structure** (IPv4 or IPv6). | `192.168.1.10:443` (IP & Port). |
| `addrlen` | Size of `addr` (e.g., `sizeof(struct sockaddr_in)`). | `16` bytes for IPv4, `28` bytes for IPv6. |
| `host` | Buffer where the **hostname** is stored. | `"google.com"` (if DNS lookup succeeds). |
| `hostlen` | Size of `host` buffer (use `NI_MAXHOST`). | `1025` bytes max. |
| `service` | Buffer where the **service name** is stored. | `"https"`, `"ssh"`, `"ftp"`. |
| `servlen` | Size of `service` buffer (use `NI_MAXSERV`). | `32` bytes max. |
| `flags` | Controls **lookup behavior** (bitwise OR). | `NI_NUMERICHOST` for raw IPs. |

---

## **🔹 Constants for Buffer Sizes**
Since we need buffers for storing hostnames and service names, we use **standard sizes**:

```c
#define NI_MAXHOST  1025  // Max hostname length
#define NI_MAXSERV  32    // Max service name length
```

📌 **Why these sizes?**  
They are large enough to store the **longest possible** hostname (`1024` chars) and service name (`31` chars).

---

## **🔹 Flags (How to Control Behavior)**
You can **customize `getnameinfo()` behavior** using these **flags**:

| **Flag**          | **Effect** |
|------------------|------------|
| `NI_DGRAM`      | Assume **UDP** instead of TCP when resolving service names. |
| `NI_NAMEREQD`   | Return an error if hostname **can't be resolved** (instead of IP). |
| `NI_NOFQDN`     | Return **only the hostname** (no domain, e.g., `myserver` instead of `myserver.example.com`). |
| `NI_NUMERICHOST` | Always return **IP address** (skip DNS lookup). |
| `NI_NUMERICSERV` | Always return **port number** (instead of a service name). |

### **📌 Example: When Flags are Useful**
1️⃣ **You don’t want slow DNS lookups** ⏳  
   ➝ Use `NI_NUMERICHOST` to force raw IPs.  
2️⃣ **You want to avoid service name resolution**  
   ➝ Use `NI_NUMERICSERV` to always get a port number.  
3️⃣ **Your app uses UDP (like a gaming server 🎮)**  
   ➝ Use `NI_DGRAM` to resolve UDP service names.

---

## **🔹 Example 1: Getting Hostname & Service from a Socket Address**
Imagine we have an **IPv4 socket address** and want to **convert it into a hostname and service**.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Pretend this is a client's address (Google DNS: 8.8.8.8:53)
    addr.sin_family = AF_INET;
    addr.sin_port = htons(53);  // DNS service
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);

    char host[NI_MAXHOST], service[NI_MAXSERV];

    int status = getnameinfo((struct sockaddr *)&addr, addr_len,
                             host, sizeof(host),
                             service, sizeof(service),
                             0);  // No special flags

    if (status != 0) {
        fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    printf("Hostname: %s\n", host);
    printf("Service: %s\n", service);

    return 0;
}
```

🔹 **Possible Output:**
```
Hostname: dns.google
Service: domain
```
📌 **Why?**  
- `8.8.8.8` resolves to **`dns.google`**  
- Port `53` corresponds to **DNS service (`domain`)**

---

## **🔹 Example 2: Force IP & Port (No DNS Lookup)**
What if we **don’t want to wait for DNS resolution**?  
We can use `NI_NUMERICHOST | NI_NUMERICSERV` to force raw IP & port:

```c
int status = getnameinfo((struct sockaddr *)&addr, addr_len,
                         host, sizeof(host),
                         service, sizeof(service),
                         NI_NUMERICHOST | NI_NUMERICSERV);
```

🔹 **Output (Faster, No DNS Lookup)**:
```
Hostname: 8.8.8.8
Service: 53
```
📌 **Why?**  
- `NI_NUMERICHOST` skips hostname resolution, so we get **raw IP**.  
- `NI_NUMERICSERV` skips service name resolution, so we get **raw port**.

---

## **🔹 Example 3: Getting Client Info in a Server**
If you're running a **TCP server**, you can **display client info** using `getnameinfo()`:

```c
struct sockaddr_storage client_addr;
socklen_t client_len = sizeof(client_addr);

// After accept(), get client details
getpeername(client_sock, (struct sockaddr *)&client_addr, &client_len);

char client_host[NI_MAXHOST], client_service[NI_MAXSERV];

if (getnameinfo((struct sockaddr *)&client_addr, client_len,
                client_host, sizeof(client_host),
                client_service, sizeof(client_service),
                0) == 0) {
    printf("Client connected: %s:%s\n", client_host, client_service);
}
```

🔹 **Possible Output (Client on Local Network)**:
```
Client connected: mylaptop.local:ssh
```
🔹 **Possible Output (Client without DNS)**:
```
Client connected: 192.168.1.50:22
```

---

## **🔹 Key Takeaways**
✅ **`getnameinfo()` is the reverse of `getaddrinfo()`**  
✅ Converts **binary IPs to hostnames & ports to service names**  
✅ Can **skip DNS** to avoid slow lookups (`NI_NUMERICHOST | NI_NUMERICSERV`)  
✅ Useful for **logging client connections in servers**  
✅ Works with **both IPv4 & IPv6**
