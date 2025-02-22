# **Users & Groups in Linux**  

---

## **1. Users in Linux**  
Each user in a Linux system has:  
✔ **A unique username** (login name)  
✔ **A User ID (UID)** → Numeric identifier  
✔ **A Group ID (GID)** → Primary group  
✔ **A home directory** (default location after login)  
✔ **A login shell** (default command interpreter)  

**Where is user information stored?**  
📂 **`/etc/passwd`** → Stores user details (except passwords).  

---

## **2. Understanding `/etc/passwd` File**  
Each user has an entry in `/etc/passwd` that looks like this:  

```bash
username:x:UID:GID:Full Name:Home Directory:Shell
```

🔹 **Example Entry:**  
```bash
john:x:1001:1001:John Doe:/home/john:/bin/bash
```

| Field | Description |
|--------|----------------|
| `john` | Username |
| `x` | Password (stored in `/etc/shadow`) |
| `1001` | User ID (UID) |
| `1001` | Group ID (GID) |
| `John Doe` | Full name (optional) |
| `/home/john` | Home directory |
| `/bin/bash` | Default shell |

**Check your current user & UID:**  
```bash
whoami
id
```

---

## **3. Password Storage (`/etc/shadow`)**  
🔒 **Passwords are NOT stored in `/etc/passwd`** for security reasons.  
Instead, they are in:  
📂 **`/etc/shadow`** (accessible only by root).  

**Example entry in `/etc/shadow`:**  
```bash
john:$6$sdf8.../jGFDY7gJDO:19000:0:99999:7:::
```
🔹 This contains **hashed passwords**, not plaintext ones!  

---

## **4. Groups in Linux**  
Linux groups help manage permissions for multiple users.  

**Where is group information stored?**  
📂 **`/etc/group`**  

Each line in `/etc/group` follows this format:  
```bash
groupname:GID:User list
```

🔹 **Example Entry:**  
```bash
developers:x:1002:john,alice
```

| Field | Description |
|--------|----------------|
| `developers` | Group name |
| `x` | Password (not usually used) |
| `1002` | Group ID (GID) |
| `john, alice` | Users in this group |

**Check your groups:**  
```bash
groups
```

---

## **5. Superuser (Root User)**  
👑 The **superuser (root)** has **UID = 0** and full system privileges.  
- Can access any file  
- Can kill any process  
- Can modify system settings  

Check if you're root:  
```bash
whoami
```
If it prints `root`, you have full control over the system! ⚡  

---

## **6. Essential User & Group Commands**
| Action | Command |
|------------|--------------------------------|
| List all users | `cat /etc/passwd` |
| List all groups | `cat /etc/group` |
| Show current user | `whoami` |
| Show user details | `id` |
| Add a new user | `sudo useradd -m username` |
| Set password for a user | `sudo passwd username` |
| Create a group | `sudo groupadd groupname` |
| Add user to a group | `sudo usermod -aG groupname username` |
| Change a user's shell | `sudo usermod -s /bin/zsh username` |

---

### **🔹 Quick Recap**  
✅ **Users** have a **UID**, home directory, and shell.  
✅ **User details** are in `/etc/passwd`, but **passwords** are in `/etc/shadow`.  
✅ **Groups** help manage permissions (`/etc/group`).  
✅ The **root user (UID 0)** has full system control.  