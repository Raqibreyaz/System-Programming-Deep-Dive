# **📌 Understanding the `write()` System Call (with Examples)**

### **🛠 What is `write()`?**
- `write()` is used to write **raw bytes** from a buffer to an open file.
- It works with **regular files, pipes, FIFOs, sockets, and terminals**.
- It **does NOT** automatically add a newline (`\n`) or null terminator (`\0`).

---

## **📌 Syntax**
```c
#include <unistd.h>

ssize_t write(int fd, const void *buffer, size_t count);
```
| **Parameter** | **Description** |
|-------------|----------------|
| `fd`       | File descriptor (open file) |
| `buffer`   | Pointer to memory containing data to write |
| `count`    | Number of bytes to write |
| **Returns** | Number of bytes written, `-1` on error |

### **📌 Return Values**
- **`> 0`** → Number of bytes actually written.
- **`-1`** → Error (e.g., invalid `fd`, disk full).

### **🛑 Important Notes**
1. `write()` **may write fewer bytes** than requested.
   - Happens with **pipes, sockets, or full disks**.
   - Must **use a loop** to ensure all data is written.
2. **Data may not be written to disk immediately**.
   - The **kernel buffers writes** to optimize disk performance.
   - Use **`fsync(fd)`** or **`fdatasync(fd)`** to force data to disk.

---

## **📌 Example 1: Writing to a File**
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    const char *message = "Hello, world!\n";
    ssize_t bytesWritten = write(fd, message, 14);  // Length of message

    if (bytesWritten == -1) {
        perror("Error writing to file");
        return 1;
    }

    printf("Wrote %ld bytes to file\n", bytesWritten);

    close(fd);
    return 0;
}
```
✅ **What Happens?**
1. **Opens `output.txt`** in write mode (`O_WRONLY | O_CREAT | O_TRUNC`).
2. **Writes `"Hello, world!\n"`** (14 bytes).
3. **Closes the file** after writing.

---

## **📌 Example 2: Handling Partial Writes**
Since `write()` **may write fewer bytes**, we must **loop until all data is written**.

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void writeFully(int fd, const char *data, size_t length) {
    size_t totalWritten = 0;
    while (totalWritten < length) {
        ssize_t bytes = write(fd, data + totalWritten, length - totalWritten);
        if (bytes == -1) {
            perror("Error writing to file");
            return;
        }
        totalWritten += bytes;
    }
}

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    const char *message = "This is a test message.\n";
    writeFully(fd, message, strlen(message));

    close(fd);
    return 0;
}
```
✅ **Why Use a Loop?**
- Ensures **all bytes are written**, even if `write()` writes less than requested.

---

## **📌 Example 3: Writing to Standard Output (`stdout`)**
```c
#include <unistd.h>

int main() {
    const char *message = "Hello, terminal!\n";
    write(STDOUT_FILENO, message, 17);
    return 0;
}
```
✅ **What Happens?**
- **Writes directly to the terminal** (file descriptor `1`).
- Equivalent to **`printf()`**, but without buffering.

---

## **📌 Key Takeaways**
| **Feature**     | **Details** |
|----------------|------------|
| Writes bytes | Works with files, pipes, sockets, terminals |
| May write less | Must loop to ensure full write |
| Buffered I/O | Use `fsync(fd)` to flush to disk |

🚀 **Conclusion:** `write()` is **low-level and efficient**, but it **requires careful handling of partial writes**.