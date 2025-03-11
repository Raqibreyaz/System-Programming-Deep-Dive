# **🚀 20% Terminal Concepts (Critical Concepts for 80% Impact)**
This list **ensures** that you have a **complete foundation** in system programming, backend development, and terminal mastery.

---

## 🔹 **1. Process Management (The Core of System Programming)**
✅ **Why?** Every system/backend dev deals with processes, multitasking, and job control.  

### **🟢 Must-Know Commands & Concepts**
- `ps aux` → List all running processes  
- `top` / `htop` → Monitor resource usage  
- `kill -9 <PID>` → Force kill a process  
- `kill -SIGTERM <PID>` → Graceful termination  
- `jobs` → List background jobs  
- `fg %1` → Bring job to foreground  
- `bg %1` → Resume job in background  
- `nohup command &` → Run process immune to logout  
- `disown -h %1` → Remove job from shell control  

### **🟢 Signals (Critical for Process Control)**
- `trap 'echo Caught SIGINT' SIGINT` → Handle `Ctrl+C` in scripts  
- `kill -SIGHUP <PID>` → Simulate user logout  
- `kill -SIGKILL <PID>` → Forcefully stop  

### **🟢 Zombie & Orphan Processes**
- `ps aux | grep defunct` → Find zombies  
- **Use `wait()` in C to prevent zombies**  

---

## 🔹 **2. I/O Redirection & Pipes (Mastering Data Flow)**
✅ **Why?** Control where input/output goes—essential for scripting and backend.  

### **🟢 Redirection**
- `command > file.txt` → Redirect stdout  
- `command 2> error.txt` → Redirect stderr  
- `command >> file.txt` → Append output  
- `command 2>&1` → Merge stdout & stderr  

### **🟢 Pipes (Essential for Chaining Commands)**
- `command | another_command` → Send output as input  
- `ls | grep ".txt"` → Find `.txt` files  
- `ps aux | grep nginx | awk '{print $2}' | xargs kill -9` → Find & kill processes  

### **🟢 `tee` (Duplicate Output)**
- `ls | tee output.txt` → Save output to file **and** print  

---

## 🔹 **3. File System & Permissions (Must-Know for Secure Backend Development)**
✅ **Why?** Files and permissions control **everything** in Linux-based systems.  

### **🟢 File Management**
- `ls -lah` → List files with size & permissions  
- `touch file.txt` → Create empty file  
- `rm -rf folder/` → Force remove a folder  
- `find /path -name "*.log"` → Find files  
- `grep -r "error" /var/logs/` → Search inside files  

### **🟢 Permissions & Ownership**
- `chmod u+x file.sh` → Make executable  
- `chown user:group file.txt` → Change file owner  
- `stat file.txt` → Get file metadata  

---

## 🔹 **4. Networking & Ports (Backend = Networking)**
✅ **Why?** If you run a backend service, you **must** know how ports & connections work.  

### **🟢 Essential Networking Commands**
- `netstat -tulnp` / `ss -tulnp` → Show open ports  
- `curl -I http://example.com` → Check HTTP headers  
- `wget http://example.com/file.zip` → Download files  
- `nc -zv 192.168.1.1 80` → Test if port is open  
- `ping google.com` → Check network connectivity  
- `traceroute google.com` → Find network path  
- `tcpdump -i eth0` → Capture network packets  

---

## 🔹 **5. Disk Usage & System Monitoring (Prevent Server Crashes)**
✅ **Why?** Running out of disk space or memory = **backend disaster**.  

### **🟢 Disk & Memory Management**
- `df -h` → Check disk usage  
- `du -sh folder/` → Get folder size  
- `free -h` → Check memory usage  
- `uptime` → See how long system has been running  

### **🟢 Process & Resource Monitoring**
- `top` / `htop` → CPU/memory monitoring  
- `vmstat 1` → Check system performance  

---

## 🔹 **6. Debugging & System Calls (Deep System Understanding)**
✅ **Why?** Debugging processes & system interactions is **critical**.  

### **🟢 Debugging Processes**
- `strace ./program` → Trace system calls  
- `lsof -p <PID>` → List open files for a process  
- `gdb ./a.out` → Debug with GDB  

### **🟢 Inspecting Logs**
- `tail -f /var/log/syslog` → Live log monitoring  
- `journalctl -u nginx.service` → View logs of a service  

---

## 🔹 **7. Shell Scripting (Automation = Power)**
✅ **Why?** As a backend dev, you **will** automate tasks using scripts.  

### **🟢 Shell Basics**
- `#!/bin/bash` → Shebang for bash scripts  
- `VAR="hello"` → Set variable  
- `echo $VAR` → Use variable  

### **🟢 Conditional Execution**
- `[ -f file.txt ] && echo "File exists"` → If file exists  
- `if [ -f file.txt ]; then echo "File exists"; fi`  

### **🟢 Loops**
- `for file in *.txt; do echo "$file"; done`  

---

## 🔹 **8. System Daemons & Services (Backend Servers)**
✅ **Why?** If you're working with **backend services**, you need to know this.  

### **🟢 Managing Services**
- `systemctl status nginx` → Check status  
- `systemctl restart nginx` → Restart service  

### **🟢 Running Services on Boot**
- `systemctl enable my_service`  

---

## 🔹 **9. Cron Jobs (Automate Everything)**
✅ **Why?** Used for **scheduled** backend tasks (backups, maintenance).  

### **🟢 Managing Cron Jobs**
- `crontab -e` → Edit cron jobs  
- Example: `0 3 * * * /path/to/script.sh` → Run script **daily at 3 AM**  

---

## 🔹 **10. User & Group Management (Security & Access Control)**
✅ **Why?** Secure backend access **requires** managing users & permissions.  

### **🟢 User & Group Commands**
- `whoami` → Current user  
- `sudo useradd -m newuser` → Create user  
- `sudo passwd newuser` → Set password  
- `groups` → Show user’s groups  
- `usermod -aG sudo newuser` → Give sudo access  

---

# **🚀 The **Real** 20% You Must Master**
Here’s the **revised** Pareto breakdown:  

| 🔢 **Concept**                         | ⏳ **Importance** |  
|---------------------------------|--------------|  
| **Process Management & Signals** | 🚨🚨🚨🚨🚨 |  
| **I/O Redirection & Pipes**       | 🚨🚨🚨🚨🚨 |  
| **File System & Permissions**      | 🚨🚨🚨🚨 |  
| **Networking & Ports**              | 🚨🚨🚨🚨 |  
| **System Monitoring**               | 🚨🚨🚨 |  
| **Debugging & System Calls**        | 🚨🚨🚨 |  
| **Shell Scripting**                 | 🚨🚨🚨 |  
| **Services & Daemons**              | 🚨🚨 |  
| **Cron Jobs**                       | 🚨🚨 |  
| **User & Group Management**         | 🚨🚨 |  

---

# **🛠️ Next Steps: How to Master These**
1️⃣ **Apply Each Concept in a Real-World Project** (Automate a task, monitor logs, handle signals)  
2️⃣ **Deep Dive into Any Missing Areas** (Let me know what to prioritize next)  
3️⃣ **Practice Daily in the Terminal** (Force yourself to use `tmux`, pipes, signals, and scripts)  