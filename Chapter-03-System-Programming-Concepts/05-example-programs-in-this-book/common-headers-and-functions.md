## Common Header File

This header file includes various other header files used by many of the example programs,
defines a Boolean data type, and defines macros for calculating the minimum and
maximum of two numeric values. Using this header file allows us to make the
example programs a bit shorter.

```c
#ifndef TLPI_HDR_H

/* Prevent accidental double inclusion */
#define TLPI_HDR_H

/* Type definitions used by many programs */
#include <sys/types.h>

/* Standard I/O functions */
#include <stdio.h>

/* Prototypes of commonly used library functions,
plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <stdlib.h>

/* Prototypes for many system calls */
#include <unistd.h>

/* Declares errno and defines error constants */
#include <errno.h>

/* Commonly used string-handling functions */
#include <string.h>

/* Declares our functions for handling numeric
arguments (getInt(), getLong()) */
#include "get_num.h"

/* Declares our error-handling functions */
#include "error_functions.h"

typedef enum { FALSE, TRUE } Boolean;
#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))
#endif
```
Let's break this down step by step.

---

## Error Diagnostic Functions

## **1. Purpose of Error-Diagnostic Functions**
When writing system programs in C, handling errors properly is crucial. Instead of repeating error-handling code everywhere, it's better to use functions that:
- Display error messages clearly
- Indicate the source of the error
- Optionally terminate the program if needed
- Provide meaningful error information (like `errno` descriptions)

The **error-diagnostic functions** in this section simplify error handling.

---

## **2. Header File: `error_functions.h`**
This file declares all the error functions that will be used.

### **Key Declarations**
```c
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

void errMsg(const char *format, ...);
void errExit(const char *format, ...) NORETURN;
void err_exit(const char *format, ...) NORETURN;
void errExitEN(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usageErr(const char *format, ...) NORETURN;
void cmdLineErr(const char *format, ...) NORETURN;

#endif
```
### **Breakdown of Each Function:**
- **`errMsg`** → Prints an error message but does NOT terminate the program.
- **`errExit`** → Prints an error message and terminates using `exit()`.
- **`err_exit`** → Similar to `errExit`, but terminates using `_exit()`.
- **`errExitEN`** → Similar to `errExit`, but allows an explicit error number (`errnum`).
- **`fatal`** → Prints an error message and terminates.
- **`usageErr`** → Used for incorrect command-line usage; prints a "Usage" message and exits.
- **`cmdLineErr`** → Similar to `usageErr`, but indicates a command-line argument error.

### **Why Use `NORETURN`?**
```c
#define NORETURN __attribute__ ((__noreturn__))
```
- This tells **GCC** that the function does **not return** (it terminates the program).
- This prevents compiler warnings like "control reaches end of non-void function".

---

## **3. Implementation in `error_functions.c`**
Now, let's understand how these functions are implemented.

### **3.1 `terminate(Boolean useExit3)`**
This function is used to terminate the program properly.
```c
static void terminate(Boolean useExit3)
{
    char *s;
    s = getenv("EF_DUMPCORE");  // Check if EF_DUMPCORE environment variable is set
    if (s != NULL && *s != '\0')
        abort();  // Produce a core dump
    else if (useExit3)
        exit(EXIT_FAILURE);  // Normal exit
    else
        _exit(EXIT_FAILURE);  // Terminate without flushing buffers
}
```
- If `EF_DUMPCORE` is set → Calls `abort()` (creates a core dump for debugging).
- If `useExit3 == TRUE` → Calls `exit()`, which cleans up before exiting.
- If `useExit3 == FALSE` → Calls `_exit()`, which terminates immediately.

---

### **3.2 `outputError()` – The Core of Error Printing**
```c
static void outputError(Boolean useErr, int err, Boolean flushStdout,
                        const char *format, va_list ap)
{
    #define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                 (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?",
                 strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    if (flushStdout)
        fflush(stdout);  // Flush stdout in case it's not written yet

    fputs(buf, stderr);  // Print error message to stderr
    fflush(stderr);  // Ensure the message is displayed
}
```
#### **What It Does:**
- Formats an error message like `printf()`.
- If `useErr == TRUE`, it includes the error description (`errno` value).
- Flushes output if necessary.
- Prints the error message to `stderr`.

---

### **3.3 `errMsg()` – Print Error Without Exiting**
```c
void errMsg(const char *format, ...)
{
    va_list argList;
    int savedErrno = errno;  // Save current errno (to restore later)

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    errno = savedErrno;  // Restore errno
}
```
- Uses `errno` to print system error messages.
- Calls `outputError()` to handle formatting.
- Does **not** terminate the program.

---

### **3.4 `errExit()` – Print Error and Exit**
```c
void errExit(const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}
```
- Same as `errMsg()`, but calls `terminate(TRUE)` to **exit the program**.

---

### **3.5 `err_exit()` – Print Error and Exit Without Flushing Buffers**
```c
void err_exit(const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(TRUE, errno, FALSE, format, argList);
    va_end(argList);

    terminate(FALSE);
}
```
#### **Difference Between `errExit()` and `err_exit()`**
| Function | Calls | Flushing `stdout`? | Use Case |
|----------|------|-----------------|----------|
| `errExit()` | `exit()` | Yes | General errors |
| `err_exit()` | `_exit()` | No | Used in child processes to avoid flushing parent’s buffers |

---

### **3.6 `errExitEN()` – Print Error from Explicit `errnum`**
```c
void errExitEN(int errnum, const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(TRUE, errnum, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}
```
- Instead of using `errno`, it takes an explicit error number (`errnum`).
- Useful for **POSIX threads (`pthread`) errors** because they return an error code instead of setting `errno`.

---

### **3.7 `fatal()` – Print Custom Error and Exit**
```c
void fatal(const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(FALSE, 0, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}
```
- Similar to `errExit()`, but doesn't use `errno`.
- Used for **non-system-related errors**.

---

### **3.8 `usageErr()` – Print Usage Error and Exit**
```c
void usageErr(const char *format, ...)
{
    va_list argList;
    fflush(stdout);
    fprintf(stderr, "Usage: ");

    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}
```
- Prints **"Usage: ..."** before the error message.
- Used when a program is executed with incorrect arguments.

---

### **3.9 `cmdLineErr()` – Print Command-Line Error and Exit**
```c
void cmdLineErr(const char *format, ...)
{
    va_list argList;
    fflush(stdout);
    fprintf(stderr, "Command-line usage error: ");

    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);
    exit(EXIT_FAILURE);
}
```
- Similar to `usageErr()`, but specifies **"Command-line usage error: ..."**.
- Used when a command-line argument is incorrect.

---

### **4. Summary**
| Function | Prints Error? | Uses `errno`? | Terminates? | Notes |
|----------|-------------|--------------|-------------|-------|
| `errMsg()` | ✅ | ✅ | ❌ | Just prints error |
| `errExit()` | ✅ | ✅ | ✅ | Calls `exit()` |
| `err_exit()` | ✅ | ✅ | ✅ | Calls `_exit()` (no flushing) |
| `errExitEN()` | ✅ | ✅ (explicit) | ✅ | For POSIX thread errors |
| `fatal()` | ✅ | ❌ | ✅ | Non-system errors |
| `usageErr()` | ✅ | ❌ | ✅ | Command-line usage |
| `cmdLineErr()` | ✅ | ❌ | ✅ | Command-line errors |
---

## Function for Parsing Numeric Command Line Arguments

### **1. Purpose of `getInt()` and `getLong()`**
- These functions convert a string argument (`arg`) into an `int` or `long`, respectively.
- They perform error checking to ensure valid numeric input, unlike `atoi()` and `atol()`, which don't detect errors.
- If an error occurs (invalid input, out-of-range value), the program exits with an error message.

---

### **2. Header File: `get_num.h`**
This header file (Listing 3-5) defines constants and function prototypes.

```c
#ifndef GET_NUM_H
#define GET_NUM_H

#define GN_NONNEG   01  /* Value must be >= 0 */
#define GN_GT_0     02  /* Value must be > 0 */
#define GN_ANY_BASE 0100 /* Can use any base - like `strtol()` */
#define GN_BASE_8   0200 /* Value is in octal */
#define GN_BASE_16  0400 /* Value is in hexadecimal */

long getLong(const char *arg, int flags, const char *name);
int getInt(const char *arg, int flags, const char *name);

#endif
```

**Explanation:**
- `GN_NONNEG`: Ensures the number is non-negative (`>= 0`).
- `GN_GT_0`: Ensures the number is greater than 0 (`> 0`).
- `GN_ANY_BASE`: Allows conversion from any numeric base.
- `GN_BASE_8`: Parses the number as octal (`base 8`).
- `GN_BASE_16`: Parses the number as hexadecimal (`base 16`).

---

### **3. Error Handling Function: `gnFail()`**
If an error occurs (invalid input, out of range, etc.), `gnFail()` prints an error message and terminates the program.

```c
static void gnFail(const char *fname, const char *msg, const char *arg, const char *name) {
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "offending text: %s\n", arg);
    exit(EXIT_FAILURE);
}
```

**Explanation:**
- Prints an error message indicating which function (`fname`) caused the issue.
- Includes the argument name (`name`) if provided.
- Displays the erroneous input (`arg`).
- Calls `exit(EXIT_FAILURE)` to stop execution.

---

### **4. Main Conversion Function: `getNum()`**
Handles the core logic of converting a string into a numeric value.

```c
static long getNum(const char *fname, const char *arg, int flags, const char *name) {
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')  
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE) ? 0 :
           (flags & GN_BASE_8) ? 8 :
           (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res = strtol(arg, &endptr, base);

    if (errno != 0)  
        gnFail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')  
        gnFail(fname, "nonnumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)  
        gnFail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)  
        gnFail(fname, "value must be > 0", arg, name);

    return res;
}
```

**Step-by-Step Breakdown:**
1. **Check for Null or Empty String:**  
   - If `arg` is `NULL` or an empty string, `gnFail()` is called.

2. **Determine Numeric Base:**  
   - Default is base 10.
   - If `GN_ANY_BASE` is set, `strtol()` automatically detects the base.
   - If `GN_BASE_8` is set, base is `8` (octal).
   - If `GN_BASE_16` is set, base is `16` (hexadecimal).

3. **Convert String to Number:**  
   - `strtol(arg, &endptr, base)` converts `arg` to a `long` integer.
   - `endptr` points to the first non-numeric character.

4. **Check for Conversion Errors:**
   - If `errno != 0`, conversion failed (`gnFail()` is called).
   - If `*endptr != '\0'`, there are extra non-numeric characters.

5. **Enforce Range Restrictions:**
   - If `GN_NONNEG` is set, `res` must be non-negative.
   - If `GN_GT_0` is set, `res` must be greater than zero.

---

### **5. Wrapper Function: `getLong()`**
Calls `getNum()` with `"getLong"` as the function name.

```c
long getLong(const char *arg, int flags, const char *name) {
    return getNum("getLong", arg, flags, name);
}
```

---

### **6. Wrapper Function: `getInt()`**
- Calls `getNum()`, but ensures the result fits in an `int`.
- If `res` is out of the `int` range, it triggers an error.

```c
int getInt(const char *arg, int flags, const char *name) {
    long res;
    res = getNum("getInt", arg, flags, name);
    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "integer out of range", arg, name);
    return (int) res;
}
```

**Step-by-Step Breakdown:**
1. Calls `getNum()` to get a `long` value.
2. Checks if `res` is within the valid `int` range (`INT_MIN` to `INT_MAX`).
3. If `res` is out of range, `gnFail()` is called.
4. Otherwise, it safely casts `res` to `int`.

---

### **7. Example Usage**
Let's assume we have a program that takes a numeric command-line argument:

```c
#include <stdio.h>
#include "get_num.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int num = getInt(argv[1], GN_NONNEG, "number");
    printf("You entered: %d\n", num);

    return 0;
}
```

#### **Example Runs**
```bash
$ ./a.out 42
You entered: 42

$ ./a.out -10
getInt error (in number): negative value not allowed
offending text: -10

$ ./a.out abc
getInt error (in number): nonnumeric characters
offending text: abc
```

---

### **8. Summary**
| Function | Purpose |
|----------|---------|
| `getNum()` | Core function that converts a string to a `long` with error handling |
| `getLong()` | Calls `getNum()` for `long` values |
| `getInt()` | Calls `getNum()`, ensures value fits in `int` |

| Feature | How it Works |
|---------|-------------|
| **Base Selection** | Defaults to decimal, can use octal (`GN_BASE_8`), hex (`GN_BASE_16`), or auto-detect (`GN_ANY_BASE`) |
| **Error Handling** | Detects invalid input, non-numeric characters, and out-of-range values |
| **Range Checking** | Can enforce non-negative (`GN_NONNEG`) or positive (`GN_GT_0`) constraints |

These functions provide **safer, more robust** command-line argument parsing for numeric values compared to standard library functions like `atoi()`.