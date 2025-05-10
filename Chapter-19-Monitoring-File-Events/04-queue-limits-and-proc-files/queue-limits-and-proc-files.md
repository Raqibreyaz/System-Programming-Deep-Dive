# 📝 **inotify: Queue Limits and /proc Files**

## **Why are there limits?**
- inotify uses kernel memory to queue events.
- To prevent a single user or process from using too much memory (and affecting the whole system), the kernel enforces limits.

---

## **Where are the limits set?**
- The limits are controlled by files in:  
  `/proc/sys/fs/inotify/`

---

## **The Three Main Limits**

### 1. **max_queued_events**
- **What:**  
  Maximum number of events that can be queued for a single inotify instance (i.e., per `inotify_init()`).
- **Default:**  
  16,384
- **What happens if exceeded?**  
  - The kernel generates an `IN_Q_OVERFLOW` event.
  - Any further events are discarded until there’s space.
  - The `wd` field in the overflow event will be `-1`.

### 2. **max_user_instances**
- **What:**  
  Maximum number of inotify instances (file descriptors) a single user (UID) can create.
- **Default:**  
  128

### 3. **max_user_watches**
- **What:**  
  Maximum number of watch items (i.e., files/directories being watched) a single user (UID) can have across all their inotify instances.
- **Default:**  
  8192

---

## **How to check or change these limits?**

- **Check current value:**
  ```bash
  cat /proc/sys/fs/inotify/max_queued_events
  cat /proc/sys/fs/inotify/max_user_instances
  cat /proc/sys/fs/inotify/max_user_watches
  ```

- **Temporarily change value (until reboot):**
  ```bash
  sudo sysctl -w fs.inotify.max_user_watches=16384
  ```

- **Permanently change value:**  
  Add a line to `/etc/sysctl.conf`, e.g.:
  ```
  fs.inotify.max_user_watches=16384
  ```
  Then run:
  ```bash
  sudo sysctl -p
  ```

---

## **Summary Table**

| File                                 | What it limits                        | Default value |
|---------------------------------------|---------------------------------------|--------------|
| `/proc/sys/fs/inotify/max_queued_events`   | Max events queued per instance         | 16384        |
| `/proc/sys/fs/inotify/max_user_instances`  | Max inotify instances per user         | 128          |
| `/proc/sys/fs/inotify/max_user_watches`    | Max watches per user                   | 8192         |

---

## **Practical Tip**
- If your program misses events and you see `IN_Q_OVERFLOW`, consider increasing `max_queued_events`.
- If you get errors from `inotify_add_watch` or `inotify_init`, you may have hit the per-user limits.

---
