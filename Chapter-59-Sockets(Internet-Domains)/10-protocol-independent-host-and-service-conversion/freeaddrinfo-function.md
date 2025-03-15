### **🔹 Why is `freeaddrinfo()` Needed?**
- `getaddrinfo()` **dynamically allocates memory** for a **linked list** of `struct addrinfo`.
- This means that all the structures it returns **live on the heap**, not the stack.
- If we don't call `freeaddrinfo()`, we create a **memory leak** because the allocated memory will never be freed.

---

### **🔹 What Happens Internally?**
When `getaddrinfo()` is called, it allocates:
1. **A linked list** of `struct addrinfo`
2. **Each `ai_addr` field** (which is a pointer to `sockaddr`)
3. **Other dynamically allocated fields**, like `ai_canonname` (if used)

**Example:**
```c
struct addrinfo *res;
getaddrinfo("google.com", "http", &hints, &res);

/* The 'res' pointer now points to a linked list of addrinfo structs */
```

Internally, it looks something like this:

```
res -> [ addrinfo struct 1 ] -> [ addrinfo struct 2 ] -> [ NULL ]
```

Each `addrinfo` struct contains:
- A pointer to its `ai_addr` field (another dynamically allocated structure)
- A pointer to the **next** `addrinfo` struct (if multiple results exist)

If we **don’t** call `freeaddrinfo(res)`, all this memory is **lost**.

---

### **🔹 How `freeaddrinfo()` Works**
- `freeaddrinfo(res)` **frees all allocated memory** in the linked list.
- It **frees each `addrinfo` node** and its corresponding `ai_addr` memory.
- **After calling `freeaddrinfo()`**, the `res` pointer is no longer valid.

**Example:**
```c
freeaddrinfo(res); // Frees all linked list memory at once
res = NULL;        // Optional safety measure
```

Now, the memory layout looks like:
```
res -> NULL
```
Everything is freed properly.

---

### **🔹 What If We Need to Keep an Address?**
If you need to **keep a copy of an address** (e.g., storing it for later use), you must **manually copy it** before calling `freeaddrinfo()`.

#### **Example: Duplicating an Address Before Freeing**
```c
struct sockaddr_in saved_addr;
memcpy(&saved_addr, res->ai_addr, sizeof(struct sockaddr_in)); // Copy first result

freeaddrinfo(res); // Safe to free now, as we saved the address elsewhere
```

### **🔹 When Would You Need to Copy?**
- If you want to **store** the IP address for later **after freeing** the `addrinfo` list.
- If you want to use the address **outside** the function that calls `getaddrinfo()`.

---

### **🔹 Summary**
| **Concept**        | **Explanation** |
|--------------------|----------------|
| `getaddrinfo()`   | Allocates a linked list of `addrinfo` on the **heap** |
| `freeaddrinfo()`  | Frees **all allocated memory** at once |
| Why free memory?  | To **avoid memory leaks** |
| Copying needed?   | If you want to **preserve an address**, manually copy it first |

---

### **🔹 Key Takeaways**
✅ **Always call `freeaddrinfo(res);` after you're done with the list**  
✅ **If you need to keep an address, copy it before freeing the list**  
✅ **Never use `res` after freeing it—it's no longer valid**  
