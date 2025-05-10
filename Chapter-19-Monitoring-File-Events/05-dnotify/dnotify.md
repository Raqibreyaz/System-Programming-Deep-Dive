## dnotify: The Older Linux File Event Notification System

### What is dnotify?

- **dnotify** is an older Linux kernel mechanism (introduced in 2.4) for monitoring file events in directories.
- It has been **superseded by inotify** (introduced in kernel 2.6.13), which is more flexible and powerful.

---

### How dnotify Works

- Applications use the `fcntl()` system call with the `F_NOTIFY` command to request notifications about changes in a directory.
- Notifications are delivered via **signals** (typically `SIGIO`), not via file descriptors or event queues.

---

### Key Limitations of dnotify (vs. inotify)

| Limitation                        | dnotify                                                                 | inotify (modern)                |
|------------------------------------|------------------------------------------------------------------------|---------------------------------|
| **Notification Mechanism**         | Uses signals (e.g., `SIGIO`)                                           | Uses file descriptors           |
| **Monitoring Unit**                | Only directories (not individual files)                                | Directories **and** files       |
| **File Descriptor Usage**          | Requires an open file descriptor for each watched directory            | No need to keep directories open|
| **Unmounting Issues**              | Watched directories can't be unmounted (file system is "busy")         | No such problem                 |
| **Resource Usage**                 | Consumes one file descriptor per watched directory                     | No extra descriptors needed     |
| **Event Precision**                | Only signals that "something changed" in the directory;                | Tells you **which file** changed, and what kind of event (create, delete, etc.) |
|                                    | does **not** tell you which file or what kind of change                |                                 |
| **Reliability**                    | Sometimes unreliable; may miss events                                  | More reliable                   |
| **Library Use**                    | Hard to use in libraries (signals are global, may conflict)            | Easy to use in libraries        |

---

### Example: How dnotify is Used

```c
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
    printf("Directory changed!\n");
}

int main() {
    int fd = open("/tmp", O_RDONLY);
    if (fd == -1) { perror("open"); return 1; }

    signal(SIGIO, handler);

    // Request notification on file modifications in the directory
    if (fcntl(fd, F_NOTIFY, DN_MODIFY | DN_CREATE | DN_DELETE | DN_MULTISHOT) == -1) {
        perror("fcntl");
        return 1;
    }

    // Wait for signals
    while (1) pause();
}
```

**Problems with this approach:**
- You only know "something" changed in `/tmp`, not what or which file.
- You must keep `/tmp` open, so you can't unmount the filesystem.
- If you want to watch many directories, you use up many file descriptors.
- If another part of your program (or a library) changes the signal handler, you may miss notifications.

---

### Why inotify is Better

- **No signals:** Uses a file descriptor you can `read()` for events.
- **More precise:** Tells you exactly which file changed, and what happened.
- **Can watch files or directories.**
- **No need to keep directories open.**
- **Easier to use in libraries and large programs.**

---

### Further Reading

- `man 2 fcntl` (search for `F_NOTIFY`)
- Kernel documentation: `Documentation/dnotify.txt` (may not be present on all systems)

---

### Summary Table

| Feature                | dnotify         | inotify         |
|------------------------|-----------------|-----------------|
| Notification method    | Signal          | File descriptor |
| Watchable objects      | Directories     | Files/Dirs      |
| Event detail           | Low             | High            |
| Resource usage         | High (FDs)      | Low             |
| Unmounting issues      | Yes             | No              |
| Library-friendly       | No              | Yes             |

---

**Bottom line:**  
dnotify is now obsolete and should not be used for new projects. Use inotify for all modern Linux file event monitoring needs.

---
