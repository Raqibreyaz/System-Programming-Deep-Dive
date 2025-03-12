The `inet_pton()` and `inet_ntop()` functions are used to convert between human-readable IP addresses and their binary representations. These functions support both **IPv4** and **IPv6** addresses and are preferred over older functions like `inet_aton()` and `inet_ntoa()` because they handle both IP versions and are more robust.

### **Function Overview**
#### `inet_pton()`
- Converts a **string representation** (presentation format) of an IP address to **binary format** (network byte order).
- Prototype:
  ```c
  #include <arpa/inet.h>
  int inet_pton(int domain, const char *src_str, void *addrptr);
  ```
- Parameters:
  - `domain`: Address family (`AF_INET` for IPv4, `AF_INET6` for IPv6).
  - `src_str`: IP address as a string.
  - `addrptr`: Pointer to a buffer (`struct in_addr` or `struct in6_addr`) where the binary address will be stored.

- Return values:
  - `1` if successful.
  - `0` if `src_str` is not in a valid format.
  - `-1` on error (sets `errno`).

#### `inet_ntop()`
- Converts a **binary IP address** to a **string representation**.
- Prototype:
  ```c
  #include <arpa/inet.h>
  const char *inet_ntop(int domain, const void *addrptr, char *dst_str, size_t len);
  ```
- Parameters:
  - `domain`: Address family (`AF_INET` or `AF_INET6`).
  - `addrptr`: Pointer to a `struct in_addr` or `struct in6_addr` containing the binary IP.
  - `dst_str`: Buffer where the converted string will be stored.
  - `len`: Size of `dst_str` buffer (`INET_ADDRSTRLEN` for IPv4, `INET6_ADDRSTRLEN` for IPv6).

- Return values:
  - Pointer to `dst_str` if successful.
  - `NULL` on error (sets `errno` to `ENOSPC` if buffer is too small).

---

### **Example Code**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    struct in_addr ipv4_addr;
    struct in6_addr ipv6_addr;
    char ipv4_str[INET_ADDRSTRLEN];
    char ipv6_str[INET6_ADDRSTRLEN];

    // Convert string to binary (IPv4)
    if (inet_pton(AF_INET, "192.168.1.1", &ipv4_addr) != 1) {
        perror("inet_pton failed for IPv4");
        return EXIT_FAILURE;
    }
    // Convert binary back to string (IPv4)
    if (inet_ntop(AF_INET, &ipv4_addr, ipv4_str, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop failed for IPv4");
        return EXIT_FAILURE;
    }
    printf("IPv4 Address: %s\n", ipv4_str);

    // Convert string to binary (IPv6)
    if (inet_pton(AF_INET6, "::1", &ipv6_addr) != 1) {
        perror("inet_pton failed for IPv6");
        return EXIT_FAILURE;
    }
    // Convert binary back to string (IPv6)
    if (inet_ntop(AF_INET6, &ipv6_addr, ipv6_str, INET6_ADDRSTRLEN) == NULL) {
        perror("inet_ntop failed for IPv6");
        return EXIT_FAILURE;
    }
    printf("IPv6 Address: %s\n", ipv6_str);

    return EXIT_SUCCESS;
}
```

---

### **Key Points**
1. **`inet_pton()` (string to binary)**
   - Used for converting IPv4/IPv6 addresses from text to binary.
   - Checks if the input is valid before conversion.
   - Works with `AF_INET` (IPv4) and `AF_INET6` (IPv6).

2. **`inet_ntop()` (binary to string)**
   - Used for converting IP addresses from binary to human-readable format.
   - Requires a properly sized buffer (`INET_ADDRSTRLEN` for IPv4, `INET6_ADDRSTRLEN` for IPv6).

3. **Why Use `inet_pton()` and `inet_ntop()`?**
   - **Supports both IPv4 and IPv6** (unlike `inet_aton()` and `inet_ntoa()` which are IPv4-only).
   - **Safer** and more **portable** for modern network programming.
   - **Handles invalid inputs** correctly.

These functions are essential for **socket programming**, **logging**, and **networking applications** that need to work with IP addresses in both human-readable and binary formats.