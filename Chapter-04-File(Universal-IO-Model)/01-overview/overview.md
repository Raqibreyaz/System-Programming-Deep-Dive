# Linux File I/O: Overview

## **1. File Descriptors (`fd`) – Understanding Their True Role**

### **What Exactly is a File Descriptor?**

- A **file descriptor (FD)** is an **integer** assigned by the OS when a process opens a file or other resource.
- It acts as a **reference** for operations like **reading, writing, or closing** the file.
- File descriptors are stored in the **per-process file descriptor table** managed by the kernel.

### **Types of File Descriptors**

File descriptors aren’t just for files! They are also used for:
| File Descriptor | Used For |
|----------------|---------|
| **Regular Files** | Reading/Writing to files (`open()`, `read()`, `write()`) |
| **Pipes** | Inter-process communication (`pipe()`) |
| **Sockets** | Network communication (`socket()`, `connect()`, `send()`, `recv()`) |
| **Devices** | Talking to hardware (`/dev/null`, `/dev/tty`) |

### **File Descriptor Table**

- Every process has a **File Descriptor Table**.
- **Default FDs for Every Process:**
  - **`0` → Standard Input (STDIN)**
  - **`1` → Standard Output (STDOUT)**
  - **`2` → Standard Error (STDERR)**
  - **`3+` → Files or other resources opened by the process**

### **Example: Understanding File Descriptors**

```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd1 = open("file1.txt", O_CREAT | O_WRONLY, 0644);
    int fd2 = open("file2.txt", O_CREAT | O_WRONLY, 0644);

    printf("FD for file1.txt: %d\n", fd1);
    printf("FD for file2.txt: %d\n", fd2);

    close(fd1);
    close(fd2);
    return 0;
}
```

🔹 **Expected Output:**

```
FD for file1.txt: 3
FD for file2.txt: 4
```

---

Yes! There are **some missing details and refinements** that I should have included for a **fully complete** breakdown. Here’s what I **missed** and will now **cover in depth**:

### **1. Missing Details**

- **Detailed explanation of `fd`, `numRead`, and `numWritten`**
- **More information on `status` returned by system calls**
- **Edge cases and error handling for `read()` and `write()`**
- **More examples for better clarity**
- **Advanced `open()` flags (`O_EXCL`, `O_SYNC`, `O_NONBLOCK`)**

Now, let's **fill in the missing pieces** in the smallest possible chunks.

---

---

## **2. Understanding `numRead` and `numWritten`**

### **What is `numRead`?**

- `numRead` is the **return value** of `read()`.
- It tells **how many bytes were actually read**.
- **If `numRead == 0`**, it means **End of File (EOF)** was reached.

### **What is `numWritten`?**

- `numWritten` is the **return value** of `write()`.
- It tells **how many bytes were successfully written**.
- If `numWritten < count`, it means **only part of the data was written**, so we may need to write again.

### **Example: Reading a File with `numRead`**

```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd;
    char buffer[50];
    ssize_t numRead;

    fd = open("test.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    numRead = read(fd, buffer, sizeof(buffer) - 1);
    if (numRead == -1) {
        perror("Error reading file");
        return 1;
    }

    buffer[numRead] = '\0'; // Null terminate the string
    printf("Read %zd bytes: %s\n", numRead, buffer);
    close(fd);
    return 0;
}
```

---

## **3. Understanding `status` Returned by System Calls**

### **How to Handle Return Values Properly?**

| System Call | Return Value | Meaning                                         |
| ----------- | ------------ | ----------------------------------------------- |
| `open()`    | `>= 0`       | File descriptor (success)                       |
| `open()`    | `-1`         | Error (file not found, permission denied, etc.) |
| `read()`    | `> 0`        | Bytes read successfully                         |
| `read()`    | `0`          | End of File (EOF) reached                       |
| `read()`    | `-1`         | Error occurred                                  |
| `write()`   | `> 0`        | Bytes successfully written                      |
| `write()`   | `-1`         | Error (e.g., disk full)                         |
| `close()`   | `0`          | Success                                         |
| `close()`   | `-1`         | Error                                           |

### **Example: Handling Return Values Correctly**

```c
int fd = open("test.txt", O_RDONLY);
if (fd == -1) {
    perror("Failed to open file");
    return 1;
}

ssize_t numRead = read(fd, buffer, 100);
if (numRead == -1) {
    perror("Read error");
    close(fd);
    return 1;
}

printf("Successfully read %zd bytes\n", numRead);
close(fd);
```

---

## **4. Advanced `open()` Flags (You Might Need These!)**

### **Additional Flags in `open()`**

| Flag         | Meaning                                                           |
| ------------ | ----------------------------------------------------------------- |
| `O_EXCL`     | Prevents opening if the file already exists (used with `O_CREAT`) |
| `O_SYNC`     | Writes are performed synchronously (slow but safe)                |
| `O_NONBLOCK` | Opens a file in **non-blocking mode**                             |

### **Example: Creating a File Safely**

```c
int fd = open("important.txt", O_CREAT | O_EXCL | O_WRONLY, 0644);
if (fd == -1) {
    perror("File already exists or cannot be created");
}
```

---

## **5. Edge Cases and Error Handling**

### **Common Errors and How to Handle Them**

| Error    | Cause                       | Fix                              |
| -------- | --------------------------- | -------------------------------- |
| `EACCES` | No permission to read/write | Check file permissions           |
| `ENOENT` | File not found              | Use `O_CREAT` if creating a file |
| `EBADF`  | Invalid file descriptor     | Ensure `fd` is valid             |
| `EINTR`  | Interrupted system call     | Retry the call                   |
| `ENOSPC` | No space left on device     | Free up disk space               |

---

## **6. Writing Data Safely (Handling Partial Writes)**

### **Problem: `write()` May Not Write Everything**

- `write()` might write **less** than requested.
- We need to **loop** until all bytes are written.

### **Example: Ensuring All Data is Written**

```c
ssize_t full_write(int fd, const char *buffer, size_t count) {
    ssize_t totalWritten = 0;
    while (totalWritten < count) {
        ssize_t numWritten = write(fd, buffer + totalWritten, count - totalWritten);
        if (numWritten == -1) {
            perror("Error writing to file");
            return -1;
        }
        totalWritten += numWritten;
    }
    return totalWritten;
}
```

## **7. File Offsets and lseek()**

When reading/writing, the file position (offset) moves forward.
To manually change the position, use lseek().

```c
off_t lseek(int fd, off_t offset, int whence);

```

- offset → How many bytes to move. 
- whence → Position Reference:
    - SEEK_SET → From the beginning
    - SEEK_CUR → From the current position
    - SEEK_END → From the end of file 

### Example: Move to the 10th Byte

```c
lseek(fd,10,SEEK_SET);
```
this moves the file position 10 bytes from the start

---

## **8. Understanding `copy.c`**

Now, let's **break down every line** of `copy.c` and **explain it in depth**.

### **Code: `copy.c` (Copying a File Using I/O System Calls)**

```c
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif
```

### **What This Does?**

1. **Includes necessary headers**:
   - `<sys/stat.h>` → File permissions (`S_IRUSR`, `S_IWUSR`, etc.).
   - `<fcntl.h>` → File control options (`O_CREAT`, `O_RDONLY`).
   - `"tlpi_hdr.h"` → Utility functions like `errExit()` (from TLPI book).
2. **Defines `BUF_SIZE = 1024`**, meaning we'll **copy 1024 bytes at a time**.

---

### **Opening the Input and Output Files**

```c
int inputFd, outputFd, openFlags;
mode_t filePerms;
ssize_t numRead;
char buf[BUF_SIZE];

if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s old-file new-file\n", argv[0]);
```

### **Explanation:**

- **Command-line arguments (`argc`, `argv[]`)**:
  - `argv[1]` → Source file (to be copied).
  - `argv[2]` → Destination file (to create/copy into).
- If the user **does not provide 2 filenames**, it prints usage:
  ```
  ./copy source.txt destination.txt
  ```

---

### **Opening the Input File (Source File)**

```c
inputFd = open(argv[1], O_RDONLY);
if (inputFd == -1)
    errExit("opening file %s", argv[1]);
```

### **Explanation:**

- **Opens the source file (`argv[1]`) in read-only mode (`O_RDONLY`).**
- If `open()` fails (e.g., file doesn’t exist), it **exits with an error**.

---

### **Opening the Output File (Destination File)**

```c
openFlags = O_CREAT | O_WRONLY | O_TRUNC;
filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

outputFd = open(argv[2], openFlags, filePerms);
if (outputFd == -1)
    errExit("opening file %s", argv[2]);
```

### **Explanation:**

- **`openFlags` (How we open the file):**
  - `O_CREAT` → Create file if it doesn’t exist.
  - `O_WRONLY` → Open file for writing only.
  - `O_TRUNC` → Truncate file (overwrite if it exists).
- **`filePerms` (Permissions for the new file):**
  - `S_IRUSR | S_IWUSR` → Owner: Read (`r`), Write (`w`).
  - `S_IRGRP | S_IWGRP` → Group: Read (`r`), Write (`w`).
  - `S_IROTH | S_IWOTH` → Others: Read (`r`), Write (`w`).
- **Equivalent to `chmod 666 newfile.txt`** (`rw-rw-rw-`).

---

### **Reading from the Input File and Writing to the Output File**

```c
while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    if (write(outputFd, buf, numRead) != numRead)
        fatal("couldn't write whole buffer");

if (numRead == -1)
    errExit("read");
```

### **Explanation:**

1. **`while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)`**

   - Reads **up to** `BUF_SIZE` (1024 bytes) into `buf[]`.
   - `numRead` stores **actual bytes read**.
   - Stops when `read()` returns `0` (EOF).

2. **`if (write(outputFd, buf, numRead) != numRead)`**
   - Writes exactly `numRead` bytes to the output file.
   - If `write()` writes **less than expected**, it **exits with an error**.

---

### **Closing the Files**

```c
if (close(inputFd) == -1)
    errExit("close input");
if (close(outputFd) == -1)
    errExit("close output");

exit(EXIT_SUCCESS);
```

### **Explanation:**

- **Closes both files.**
- `close()` ensures the file descriptor is **released**.
- If closing fails, `errExit()` is called.

---

## **9. Edge Cases and Errors**

### **What If…**

| Situation                                  | What Happens?                                                 |
| ------------------------------------------ | ------------------------------------------------------------- |
| File doesn't exist                         | `open()` fails (`ENOENT`).                                    |
| No permission to read file                 | `open()` fails (`EACCES`).                                    |
| Output file exists                         | **It gets overwritten** (because of `O_TRUNC`).               |
| `write()` writes fewer bytes than expected | **Error is raised** (`fatal("couldn't write whole buffer")`). |

---

## **10. Summary of Key Takeaways**

✅ **File descriptors (fd) represent open files, sockets, pipes, etc.**  
✅ **We use `open()`, `read()`, `write()`, and `close()` to copy a file.**  
✅ **File permissions (`S_IRUSR`, `S_IWUSR`, etc.) control access.**  
✅ **Error handling is crucial—always check return values!**  
✅ **Partial writes must be handled carefully (`write()` may not write everything at once).**

---
