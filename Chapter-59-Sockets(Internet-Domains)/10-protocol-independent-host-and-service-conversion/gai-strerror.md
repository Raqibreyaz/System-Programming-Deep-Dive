### **🔹 Diagnosing Errors with `gai_strerror()`**
When `getaddrinfo()` fails, it **does not set `errno`** like many other system calls.  
Instead, it returns a **nonzero error code** that indicates the reason for the failure.

---

### **🔹 How to Handle Errors Properly**
Whenever `getaddrinfo()` fails, you should:
1. Check the **return value** (if it's nonzero, an error occurred).
2. Use `gai_strerror(error_code)` to print the **human-readable error message**.

---

### **🔹 Example Usage**
Here’s a simple example that **handles errors properly**:
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
    struct addrinfo hints, *res;
    int status;

    // Set up hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Incorrect host name to force an error
    status = getaddrinfo("invalid_host_name", "http", &hints, &res);

    if (status != 0) // If nonzero, an error occurred
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1; // Exit with error
    }

    freeaddrinfo(res); // Free memory if successful
    return 0;
}
```
---

### **🔹 Common Error Codes & Their Meaning**
| **Error Code**     | **Meaning** |
|--------------------|------------|
| `EAI_AGAIN`       | Temporary failure in name resolution (try again later). |
| `EAI_BADFLAGS`    | Invalid flag in `hints.ai_flags`. |
| `EAI_FAIL`        | Unrecoverable failure in name resolution. |
| `EAI_FAMILY`      | Address family (`ai_family`) not supported. |
| `EAI_MEMORY`      | Memory allocation failure. |
| `EAI_NODATA`      | No address associated with the hostname. |
| `EAI_NONAME`      | Unknown host or service, or both host and service were `NULL`. |
| `EAI_OVERFLOW`    | Argument buffer overflow. |
| `EAI_SERVICE`     | Service not supported for `ai_socktype`. |
| `EAI_SOCKTYPE`    | `hints.ai_socktype` not supported. |
| `EAI_SYSTEM`      | System error (check `errno`). |

---

### **🔹 Key Takeaways**
✅ **Always check `getaddrinfo()` return value**  
✅ **Use `gai_strerror()` to get a readable error message**  
✅ **`EAI_SYSTEM` errors require checking `errno` separately**  
✅ **Temporary errors (`EAI_AGAIN`) may be retried after a delay**  
