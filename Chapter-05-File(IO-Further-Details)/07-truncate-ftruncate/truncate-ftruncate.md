### **Final Note on `truncate()` and `ftruncate()`**  

1. **Purpose:**  
   - Both `truncate()` and `ftruncate()` modify the size of a file.  
   - If the new size is **smaller**, excess data is **lost**.  
   - If the new size is **larger**, the file is **extended**.  

2. **Differences:**  
   - `truncate(path, length)`: Works on a **file path** (no need to open the file).  
   - `ftruncate(fd, length)`: Works on an **open file descriptor** (file must be opened for writing).  

3. **Holes vs. Null Bytes (`\0`) When Extending:**  
   - If the **file system supports sparse files** (e.g., `ext4`, `XFS`), **holes** (unallocated space) are created.  
   - If the **file system does not support sparse files** (e.g., `FAT32`), it fills the space with **null bytes (`\0`)**.  
   - The behavior also depends on the **OS and kernel implementation**.  

4. **Checking If a File Has Holes:**  
   - Use `ls -lh <file>` to see the **logical size**.  
   - Use `du -h <file>` to see the **actual disk usage**.  
   - If `du` reports a much smaller size than `ls`, the file contains **holes**.  

🚀 **Key Takeaway:** `truncate()` and `ftruncate()` efficiently manage file sizes, but whether extended space is filled with holes or null bytes depends on the file system.

pread
pwrite
readv
writev
preadv
pwritev
truncate
ftruncate