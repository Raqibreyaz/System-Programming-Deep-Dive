# **Understanding `readv()` and `writev()` (Scatter-Gather I/O)**

The `readv()` and `writev()` system calls allow a process to read or write data across multiple buffers in a **single system call**, improving efficiency.

---

## **1. What is Scatter-Gather I/O?**
Instead of using **multiple `read()` or `write()` calls**, scatter-gather I/O allows transferring **multiple buffers** at once:
- **Scatter Input (`readv()`)**: Reads data from a file descriptor and distributes it across multiple buffers.
- **Gather Output (`writev()`)**: Collects data from multiple buffers and writes it as a contiguous block.

### **Why use scatter-gather I/O?**
✅ **Efficiency**: Avoids multiple system calls, reducing overhead.  
✅ **Atomicity**: Ensures all data is read/written in one operation.  
✅ **Convenience**: No need to manually copy data into a single buffer.

---

## **2. `struct iovec` - Describing Buffers**
Scatter-gather I/O uses an array of `struct iovec`, where each element represents a buffer:
```c
struct iovec {
    void *iov_base;  // Pointer to buffer
    size_t iov_len;  // Number of bytes to read/write
};
```

---

## **3. `readv()` - Scatter Input**
Reads data from a file and **scatters** it across multiple buffers.

### **Prototype**
```c
#include <sys/uio.h>
ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
```
- `fd`: File descriptor.
- `iov`: Array of `iovec` structures.
- `iovcnt`: Number of elements in `iov`.

### **Behavior**
1. Reads data **sequentially** into `iov[0]`, then `iov[1]`, etc.
2. Returns the **total number of bytes read**.
3. **Stops at EOF** or when buffers are full.

### **Example**
```c
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    struct iovec iov[3];
    char header[10], body[20], footer[10];

    iov[0].iov_base = header;
    iov[0].iov_len = sizeof(header);
    iov[1].iov_base = body;
    iov[1].iov_len = sizeof(body);
    iov[2].iov_base = footer;
    iov[2].iov_len = sizeof(footer);

    ssize_t bytesRead = readv(fd, iov, 3);
    if (bytesRead == -1) {
        perror("readv");
        exit(EXIT_FAILURE);
    }

    printf("Read %ld bytes:\n", bytesRead);
    printf("Header: %.*s\n", (int)iov[0].iov_len, header);
    printf("Body: %.*s\n", (int)iov[1].iov_len, body);
    printf("Footer: %.*s\n", (int)iov[2].iov_len, footer);

    close(fd);
    return 0;
}
```
**Example Output:**
```
Read 40 bytes:
Header: [10 bytes of data]
Body: [20 bytes of data]
Footer: [10 bytes of data]
```

---

## **4. `writev()` - Gather Output**
Gathers multiple buffers and writes them **as one contiguous block**.

### **Prototype**
```c
#include <sys/uio.h>
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
```
- `fd`: File descriptor.
- `iov`: Array of `iovec` structures.
- `iovcnt`: Number of elements in `iov`.

### **Behavior**
1. **Concatenates** data from buffers.
2. Writes everything in a **single system call**.
3. Ensures **atomicity** when writing to files.

### **Example**
```c
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    struct iovec iov[3];
    char header[] = "Header\n";
    char body[] = "Body of the file\n";
    char footer[] = "Footer\n";

    iov[0].iov_base = header;
    iov[0].iov_len = sizeof(header) - 1;
    iov[1].iov_base = body;
    iov[1].iov_len = sizeof(body) - 1;
    iov[2].iov_base = footer;
    iov[2].iov_len = sizeof(footer) - 1;

    ssize_t bytesWritten = writev(fd, iov, 3);
    if (bytesWritten == -1) {
        perror("writev");
        exit(EXIT_FAILURE);
    }

    printf("Wrote %ld bytes to file\n", bytesWritten);
    close(fd);
    return 0;
}
```
**Content of `output.txt`:**
```
Header
Body of the file
Footer
```

---

## **5. `preadv()` and `pwritev()`**
These are **non-standard** Linux extensions that add **random access**.

### **Prototype**
```c
ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
```
- **Unlike `readv()` and `writev()`**, these system calls **do not change** the file offset.

### **Use Case: Multithreading**
✅ **Multiple threads** can read/write **at different offsets** **without interfering**.

---

## **6. Summary**
| Function | Operation | Atomic? | File Offset |
|----------|-----------|---------|-------------|
| `readv()`  | Read into multiple buffers | ✅ Yes | Updates offset |
| `writev()` | Write from multiple buffers | ✅ Yes | Updates offset |
| `preadv()` | Read at a given offset | ✅ Yes | Does **not** update offset |
| `pwritev()` | Write at a given offset | ✅ Yes | Does **not** update offset |

---

## **7. Advantages & Use Cases**
| **Advantage** | **Use Case** |
|--------------|------------|
| Reduces system call overhead | Large I/O operations |
| Avoids extra memory copies | Network packet processing |
| Atomic I/O operations | Logging systems |
| Allows structured I/O | Reading structured data from files |

---

## **8. 📂 Hardcoded File Example & Finding Buffer Sizes**  

Let's create a **binary file** (`datafile.bin`) with **structured records** and manually show how to determine buffer sizes.

---

### **📜 Step 1: Creating a Hardcoded Binary File (Hex View)**
Here’s an example of a **binary file containing structured records**. Each record has **596 bytes** with fields of **fixed sizes**.

#### **Hex Dump of `datafile.bin` (Each Record = 596 Bytes)**
```
00000000: 4D 45 54 41 44 41 54 41 00 00 00 00 00 00 00 00  Metadata........
00000010: 39 30 00 00                                      ID: 12345 (Hex: 0x3039)
00000014: 41 6C 69 63 65 00 00 00 00 00 ... (64 Bytes)     Name: "Alice"
00000054: XX XX XX XX XX ... (512 Bytes)                   Data: (Raw Binary Data)
```

---

### **🛠 Step 2: Finding Buffer Sizes for `readv()`**
Each field in the record has a **fixed size**:
| Field   | Offset | Size  | Buffer in C |
|---------|--------|------|--------------|
| Header  | 0      | 16B  | `char header[16];` |
| ID      | 16     | 4B   | `int32_t id;` |
| Name    | 20     | 64B  | `char name[64];` |
| Data    | 84     | 512B | `char data[512];` |

---

### **📥 Step 3: Using `readv()` to Read a Record into Separate Buffers**
```c
#include <sys/uio.h>  // For readv()
#include <fcntl.h>    // For open()
#include <unistd.h>   // For read(), close()
#include <stdio.h>    // For printf()

int main() {
    int fd = open("datafile.bin", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Buffers for different fields
    char header[16];
    int32_t id;
    char name[64];
    char data[512];

    // iovec structures to map buffers
    struct iovec iov[4];
    iov[0].iov_base = header;  iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &id;     iov[1].iov_len = sizeof(id);
    iov[2].iov_base = name;    iov[2].iov_len = sizeof(name);
    iov[3].iov_base = data;    iov[3].iov_len = sizeof(data);

    // Read entire structured record in one syscall
    ssize_t bytesRead = readv(fd, iov, 4);
    if (bytesRead == -1) {
        perror("Error reading file");
    } else {
        printf("Read %ld bytes successfully!\n", bytesRead);
        printf("ID: %d, Name: %s\n", id, name);
    }

    close(fd);
    return 0;
}
```
✅ **How `readv()` works here?**  
- `header[]` gets first 16 bytes (`Metadata`).
- `id` gets the next 4 bytes (`0x3039` → 12345).
- `name[]` gets the next 64 bytes (`Alice\0...`).
- `data[]` gets the remaining 512 bytes.

---

### **📤 Step 4: Using `writev()` to Modify & Write Back a Record**
```c
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = open("datafile.bin", O_WRONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Buffers for writing
    char header[16] = "UPDATED_HDR";
    int32_t id = 67890;
    char name[64] = "Bob";
    char data[512] = {0};  // Empty raw data

    struct iovec iov[4];
    iov[0].iov_base = header;  iov[0].iov_len = sizeof(header);
    iov[1].iov_base = &id;     iov[1].iov_len = sizeof(id);
    iov[2].iov_base = name;    iov[2].iov_len = sizeof(name);
    iov[3].iov_base = data;    iov[3].iov_len = sizeof(data);

    ssize_t bytesWritten = writev(fd, iov, 4);
    if (bytesWritten == -1) {
        perror("Error writing file");
    } else {
        printf("Wrote %ld bytes successfully!\n", bytesWritten);
    }

    close(fd);
    return 0;
}
```
✅ **How `writev()` works here?**  
- **Writes all fields at once**, preserving the structured format.
- **Avoids extra copies**, as each buffer is written directly.

---

### **🔍 Summary**
1. **Find buffer sizes** from the file format (**structured record layout**).
2. Use `readv()` to **scatter-read** data into multiple buffers efficiently.
3. Modify fields **in memory** without parsing.
4. Use `writev()` to **gather-write** all modified fields back in one syscall.

---

### **🎯 Why This Works for Large Files?**
✔ **No need to allocate a giant buffer**.  
✔ **Efficient memory usage** (only loads necessary fields).  
✔ **Avoids extra processing** (no manual parsing needed).  
✔ **Atomic updates** (modifies and writes the record in one step).  

## **9. Conclusion**
- `readv()` and `writev()` are useful for **efficient, atomic** I/O.
- They **reduce system call overhead** by transferring multiple buffers at once.
- `preadv()` and `pwritev()` allow **random access I/O** without affecting the file offset.
- Useful for **networking, logging, and structured data I/O**.
