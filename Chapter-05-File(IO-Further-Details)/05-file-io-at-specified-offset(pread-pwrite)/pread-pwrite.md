# File I/O at a Specified Offset: `pread()` and `pwrite()`

## Introduction
The `pread()` and `pwrite()` system calls function similarly to `read()` and `write()`, with one key difference: they allow file I/O to occur at a specific offset without modifying the file’s current offset. This makes them particularly useful for concurrent programming and performance optimization.

---

## Function Signatures
```c
#include <unistd.h>

ssize_t pread(int fd, void *buf, size_t count, off_t offset);
// Returns number of bytes read, 0 on EOF, or -1 on error

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);
// Returns number of bytes written, or -1 on error
```

- `fd`: File descriptor of the file to read from or write to.
- `buf`: Buffer to store read data (`pread()`) or buffer containing data to be written (`pwrite()`).
- `count`: Number of bytes to read/write.
- `offset`: Position in the file where the operation should occur.

Unlike `read()` and `write()`, these system calls do not change the file’s offset.

---

## How `pread()` Works
Calling `pread()` is equivalent to performing the following steps atomically:
```c
off_t orig;
orig = lseek(fd, 0, SEEK_CUR);  // Save current file offset
lseek(fd, offset, SEEK_SET);    // Move to desired offset
ssize_t s = read(fd, buf, count); // Read data
lseek(fd, orig, SEEK_SET);      // Restore original offset
```
The key advantage is that the original file offset remains unchanged, making it safer in multithreaded environments.

---

## Why Use `pread()` and `pwrite()`?
### 1. **Avoiding Race Conditions in Multithreading**
- In a multithreaded process, all threads share the same file descriptor table, meaning they share the same file offset.
- If multiple threads use `lseek()` and `read()` separately, another thread might change the offset before `read()` executes, causing unintended results.
- `pread()` and `pwrite()` eliminate this race condition by ensuring atomicity.

**Example:**
```c
pthread_create(&t1, NULL, read_at_offset, (void *)fd);
pthread_create(&t2, NULL, read_at_offset, (void *)fd);
```
If `pread()` is used, each thread reads from the intended offset, avoiding conflicts.

---

### 2. **Performance Optimization**
- Performing `lseek()` followed by `read()` (or `write()`) requires two system calls.
- `pread()` and `pwrite()` combine these operations into a single system call, reducing overhead.
- While system call overhead is generally small, in high-performance applications, avoiding extra calls improves efficiency.

---

## Key Limitations
- `pread()` and `pwrite()` only work on **seekable files** (i.e., regular files). They **cannot** be used on pipes, sockets, or terminals.
- Error handling should always be implemented to check for partial reads/writes or failures.

---

## Summary
| Feature         | `pread()` / `pwrite()` | `read()` / `write()` + `lseek()` |
|----------------|---------------------|--------------------------|
| Modifies file offset? | ❌ No | ✅ Yes |
| Thread-safe? | ✅ Yes | ❌ No |
| System call overhead | ✅ Lower | ❌ Higher |
| Works with non-seekable files? | ❌ No | ✅ Yes |

By using `pread()` and `pwrite()`, applications can achieve **better concurrency, atomic operations, and improved performance** in file handling.

