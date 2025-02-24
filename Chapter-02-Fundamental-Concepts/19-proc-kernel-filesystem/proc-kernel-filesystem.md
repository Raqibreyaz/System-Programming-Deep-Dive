# **📌 `/proc` File System**  

## **🔹 1. What is `/proc`?**  
✅ **`/proc` is a virtual file system** that provides a way to access **kernel data** as if they were regular files.  
✅ It is **not a real file system** on disk—it is generated **on the fly** by the Linux kernel.  

📌 **Purpose of `/proc`** → Allows users and programs to **view and modify system attributes** in a simple way.  

---

## **🔹 2. Why is `/proc` Useful?**  
Instead of using **complex system calls** to get kernel info, you can simply:  
- **Read files in `/proc`** → Get system & process information.  
- **Write to certain `/proc` files** → Modify system settings.  

✅ **Example:** Want to check CPU info? Just run:  
```bash
cat /proc/cpuinfo
```

---

## **🔹 3. Structure of `/proc` Directory**  
The `/proc` directory contains **various subdirectories and files**:  

### **🔹 (A) System-Wide Information**  
| Path | Description |
|------|------------|
| `/proc/cpuinfo` | Info about CPU(s) (model, cores, speed, etc.). |
| `/proc/meminfo` | Info about RAM (total/free memory). |
| `/proc/uptime` | System uptime in seconds. |
| `/proc/loadavg` | System load averages (like `uptime` command). |
| `/proc/mounts` | List of mounted file systems. |
| `/proc/swaps` | Info about swap memory usage. |

✅ **Example: Get Total RAM Available**  
```bash
cat /proc/meminfo | grep MemTotal
```

---

### **🔹 (B) Process-Specific Information (`/proc/PID`)**  
Each running process has a **directory inside `/proc`** named after its **PID (Process ID)**:  
📌 Example: Process with **PID 1234** → Info stored in `/proc/1234/`  

| Path | Description |
|------|------------|
| `/proc/PID/cmdline` | Command that started the process. |
| `/proc/PID/status` | Process status (e.g., running, sleeping). |
| `/proc/PID/environ` | Process environment variables. |
| `/proc/PID/fd/` | Open file descriptors of the process. |
| `/proc/PID/maps` | Memory map of the process. |

✅ **Example: Check command used to start a process (PID 1234)**  
```bash
cat /proc/1234/cmdline
```

---

## **🔹 4. Can We Modify `/proc`?**  
Yes! Some files in `/proc` can be **written to** in order to change system settings.  

📌 **Example: Enable/Disable CPU Core (as root user)**  
```bash
echo 0 > /proc/sys/devices/system/cpu/cpu1/online   # Disable CPU core 1
echo 1 > /proc/sys/devices/system/cpu/cpu1/online   # Enable CPU core 1
```

---

## **🔹 5. `/proc` is Linux-Specific**  
📌 **Important Note:**  
- The `/proc` file system is **not standardized** by POSIX.  
- It **varies** across different Unix-like systems.  
- **Linux-specific feature**, though some other OSes have similar implementations.  

---

## **🔹 6. TL;DR (Super Simplified Summary)**  
✅ **`/proc` is a virtual file system** that provides an easy way to access kernel data.  
✅ **Everything inside `/proc` looks like a file**, but it’s dynamically created by the kernel.  
✅ You can **read files** to get system/process info and **write to some files** to modify system settings.  
✅ Each **process has its own directory** (`/proc/PID`) containing information about it.  
✅ `/proc` is **Linux-specific** and not part of Unix standards.  

---