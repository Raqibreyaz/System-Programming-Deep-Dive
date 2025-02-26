## **📌 Feature Test Macros (Final, Fully Compiled Version)**
This is the **most detailed, structured, and simplest possible explanation** of **Feature Test Macros** with all sections covered.

---

## **🔹 What Are Feature Test Macros?**
Feature Test Macros **control which functions and definitions are available** in system header files when compiling a C program.

💡 **Why do they exist?**  
Different UNIX systems (POSIX, BSD, GNU, etc.) provide **different sets of functions**. Feature test macros help you:  
1. **Write portable code** that works across multiple UNIX systems.  
2. **Enable specific features** from POSIX, BSD, System V, or GNU.  
3. **Avoid conflicts** between different system implementations.  

---

## **🔹 How They Work Internally**
1. **System header files (`.h` files) contain functions** but don’t always expose all of them.  
2. **Feature test macros act as "switches"** to turn features ON or OFF.  
3. **Header files use `#ifdef` checks** to decide whether to include a function.  

Example from `<string.h>`:  
```c
#ifdef _POSIX_C_SOURCE
char *strdup(const char *s); // strdup() is exposed only if _POSIX_C_SOURCE is set
#endif
```
- If `_POSIX_C_SOURCE` is defined, `strdup()` is **available**.  
- If `_POSIX_C_SOURCE` is **not defined**, `strdup()` **is hidden**, and you get a **compilation error**.

---

## **🔹 How the Compiler Decides What to Include or Exclude**
When you compile a program:  
1. **The compiler reads the feature test macros** (either from the code or compiler flags).  
2. **It passes them to system header files** (`unistd.h`, `stdio.h`, etc.).  
3. **Header files check which macros are set** and expose only the allowed functions.  

---

## **🔹 How to Use Feature Test Macros**
There are **two ways** to define a feature test macro:  

### **1️⃣ Inside Your C Code (Before Including Headers)**
Define the macro **at the beginning** of your program:  
```c
#define _POSIX_C_SOURCE 200809  // Enable POSIX 2008 features
#include <stdio.h>
#include <unistd.h>
```

### **2️⃣ When Compiling (Using `-D` Option)**
Pass the macro as a **compiler flag**:  
```sh
cc -D_POSIX_C_SOURCE=200809 myprogram.c
```

💡 **Which method should you use?**  
- **Inside the C code** → When writing a program that must always work on specific systems.  
- **With `-D` flag** → When compiling the same code for **different systems with different options**.  

---

## **🔹 Feature Test Macros & Their Use Cases**
Here’s a breakdown of the **most commonly used** macros:

| **Macro**             | **Purpose** |
|----------------------|------------|
| `_POSIX_C_SOURCE`    | Enables **POSIX** features (1990 - 2008) |
| `_XOPEN_SOURCE`      | Enables **X/Open and UNIX** (XSI extensions) |
| `_BSD_SOURCE`        | Enables **BSD-specific** features (Deprecated) |
| `_SVID_SOURCE`       | Enables **System V (SVID) features** |
| `_GNU_SOURCE`        | Enables **everything** (GNU + POSIX + BSD + System V) |
| `_DEFAULT_SOURCE`    | Equivalent to `_BSD_SOURCE + _SVID_SOURCE` |

---

## **🔹 Explanation of Each Macro (With Examples)**

### **📌 `_POSIX_C_SOURCE` (Enable POSIX Functions)**
Defines which **POSIX standard** should be followed.

| **Value**      | **Enabled Features** |
|--------------|----------------|
| `1`         | POSIX.1-1990 |
| `199309`    | POSIX.1b (Realtime) |
| `199506`    | POSIX.1c (Threads) |
| `200112`    | POSIX.1-2001 |
| `200809`    | POSIX.1-2008 |

💡 **Example (Enable `strdup()` and `sigaction()`)**  
```c
#define _POSIX_C_SOURCE 200809
#include <string.h>
#include <signal.h>
```

---

### **📌 `_XOPEN_SOURCE` (Enable X/Open & UNIX Extensions)**
- Defined by **X/Open Group** (SUS – Single UNIX Specification).
- Includes **POSIX + extra UNIX features**.

| **Value** | **Enabled Features** |
|---------|----------------|
| `500`   | SUSv2 (UNIX 98) |
| `600`   | SUSv3 (UNIX 03) + C99 |
| `700`   | SUSv4 (UNIX 08) |

💡 **Example (Enable UNIX 03 Features)**  
```c
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <unistd.h>
```

---

### **📌 `_BSD_SOURCE` (Enable BSD Functions)**
- Provides **extra functions** found in BSD-based UNIX (e.g., Linux, macOS).
- **Deprecated** in favor of `_DEFAULT_SOURCE`.

💡 **Example (Enable `gethostname()`)**  
```c
#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>

int main() {
    char name[256];
    gethostname(name, sizeof(name));
    printf("Hostname: %s\n", name);
}
```

---

### **📌 `_GNU_SOURCE` (Enable EVERYTHING)**
- Enables **all features** from:
  - **POSIX**
  - **X/Open**
  - **BSD**
  - **GNU extensions**
- **Use only if you want maximum compatibility.**

💡 **Example (Enable `strcasestr()`, a GNU-only function)**  
```c
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>

int main() {
    char *s = "Hello World";
    char *sub = strcasestr(s, "world");  // GNU-specific function
    printf("%s\n", sub);
}
```

---

## **🔹 Feature Test Macros in Function Prototypes**
System headers use **conditional compilation** to decide whether to **declare** functions based on macros.

Example from `<string.h>`:
```c
#ifdef _POSIX_C_SOURCE
char *strdup(const char *s);
#endif
```
If `_POSIX_C_SOURCE` is **defined**, `strdup()` is available. Otherwise, it's **hidden**.

---

## **🔹 What Happens If You Don’t Define Any Macro?**
By default, when using **GNU C Compiler (GCC)**:
- `_POSIX_C_SOURCE=200809`
- `_BSD_SOURCE`
- `_SVID_SOURCE`

If you compile with **strict mode** (`-std=c99`), only **standard C functions** are available.

---

## **✅ Summary Table (Final Cheat Sheet)**
| **Macro**         | **Use It When You Need...** |
|-----------------|----------------------------|
| `_POSIX_C_SOURCE` | **POSIX functions** like `strdup()`, `sigaction()` |
| `_XOPEN_SOURCE`  | **X/Open UNIX functions** (SUSv2, SUSv3, SUSv4) |
| `_BSD_SOURCE`    | **BSD functions** like `gethostname()` (Deprecated, use `_DEFAULT_SOURCE`) |
| `_SVID_SOURCE`   | **System V functions** (Rarely needed) |
| `_GNU_SOURCE`    | **Everything (POSIX + X/Open + GNU)** |
| `_DEFAULT_SOURCE` | **A mix of `_BSD_SOURCE` + `_SVID_SOURCE`** |

---