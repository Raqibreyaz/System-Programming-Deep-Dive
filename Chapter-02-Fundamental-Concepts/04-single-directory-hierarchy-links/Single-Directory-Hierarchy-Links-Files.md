# **Single Driectory Hierarchy, Directories, Links and Files**

---

## **1. Single Directory Hierarchy**

Unlike Windows, which has separate directory structures for each disk (C:, D:, etc.), Linux has a **single hierarchical structure**.

### **Example:**

```
/
├── bin/
│   ├── bash
│   ├── ls
│   ├── cp
├── home/
│   ├── user/
│   │   ├── Documents/
│   │   ├── Downloads/
├── etc/
│   ├── passwd
│   ├── group
```

- The **root (********`/`********)** directory is at the top.
- Everything (files, directories, devices) exists **under this single hierarchy**.
- No concept of different drives (like C: or D:).

---

## **2. File Types in Linux**

Every file in Linux belongs to a specific **type**:

| File Type           | Description                                     | Example                                         |
| ------------------- | ----------------------------------------------- | ----------------------------------------------- |
| **Regular file**    | Normal text, code, binary files                 | `/home/user/document.txt`                       |
| **Directory**       | Folder containing other files                   | `/home/user/`                                   |
| **Symbolic link**   | A shortcut (reference to another file)          | `/usr/bin/python -> /usr/bin/python3.10`        |
| **Hard link**       | Another name for a file, sharing the same inode | `ln file1 file2` creates a hard link            |
| **Special files**   | Represent hardware devices                      | `/dev/sda1` (hard drive), `/dev/tty` (terminal) |
| **Pipes & Sockets** | For inter-process communication                 | `mkfifo mypipe`                                 |

---

## **3. Hard Links vs. Symbolic Links**

### **What are Links?**

A **link** is an alternative way to access a file, similar to having multiple names for the same document.

### **Key Differences:**

| Feature                               | Hard Link             | Symbolic Link (Soft Link)      |
| ------------------------------------- | --------------------- | ------------------------------ |
| Points To                             | The same data on disk | The original file location     |
| Works After Original File is Deleted? | ✅ Yes                 | ❌ No (Becomes a "broken link") |
| Works Across Different Filesystems?   | ❌ No                  | ✅ Yes                          |
| Works Across Different Partitions?    | ❌ No                  | ✅ Yes                          |

### **Example Usage:**

```bash
ln file.txt hardlink.txt      # Create a hard link  
ln -s file.txt softlink.txt   # Create a symbolic link  
```

- `hardlink.txt` and `file.txt` share the **same inode and content**.
- `softlink.txt` is a **shortcut pointing to \*\*\*\*****`file.txt`**.

---

## **4. Filenames & Pathnames**

### **Filenames**

- Can be up to **255 characters long**.
- Cannot contain `/` (slash) or `\0` (null).
- Should avoid special characters (`*`, `?`, `&`, `;`) to prevent shell confusion.

### **Pathnames**

A **pathname** specifies a file’s location in the directory structure:

- **Absolute path** → Starts from `/` (root directory).
  - Example: `/home/user/file.txt`
- **Relative path** → Starts from the current directory.
  - Example: `../Downloads/file.txt`

### **Example:**

```sh
# Move into a directory using absolute path
cd /home/user/Documents

# Move using a relative path
cd ../Downloads
```

---

## **5. File Ownership & Permissions**

Every file has:

1. **Owner** (User ID - `UID`)
2. **Group** (Group ID - `GID`)
3. **Permissions** for:
   - **Owner**
   - **Group**
   - **Others**

### **Permission Bits (********`rwx`******** Format)**

| Symbol  | Meaning | Example               |
| ------- | ------- | --------------------- |
| `r` (4) | Read    | View file contents    |
| `w` (2) | Write   | Modify file contents  |
| `x` (1) | Execute | Run file as a program |

Example of file permissions (`ls -l` output):

```sh
-rwxr-xr--  1 user group 1234 Feb 21 12:00 script.sh
```

- `rwx` → Owner can **read, write, execute**
- `r-x` → Group can **read, execute**
- `r--` → Others can **only read**

### **Changing Permissions**

```sh
chmod 755 myfile      # Owner: rwx, Group: r-x, Others: r-x
chmod u+w myfile      # Add write permission for owner
chmod g-x myfile      # Remove execute permission for group
```

### **Changing Ownership**

```sh
chown user:group myfile  # Change file owner and group
```

---

## **6. Practical Example: Setting Up Secure Permissions**

Let’s say we have a script `backup.sh`, and we want:

- Only the **owner** to edit and execute it.
- The **group** to execute it but not modify it.
- **Others** to have no access.

### **Step 1: Set Permissions**

```bash
chmod 750 backup.sh
```

- Owner (`7`) → Read, write, execute
- Group (`5`) → Read, execute
- Others (`0`) → No access

### **Step 2: Set Ownership**

```bash
chown admin:staff backup.sh
```

- The **owner** is now `admin`.
- The **group** is `staff`.

Now, only **admin** can modify and execute, while others can’t touch it.

---

## **7. Summary**

| Concept                           | Key Takeaways                                                          |
| --------------------------------- | ---------------------------------------------------------------------- |
| **Single Directory Hierarchy**    | Everything starts from `/` (root)                                      |
| **File Types**                    | Regular files, directories, links, devices, pipes, and sockets         |
| **Hard Links vs. Symbolic Links** | Hard links are alternate names; symbolic links are shortcuts           |
| **Filenames & Pathnames**         | Use **absolute paths** for clarity, **relative paths** for flexibility |
| **Permissions**                   | `rwx` for Owner, Group, Others (`chmod`, `chown`)                      |

---