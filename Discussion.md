# Multithreaded Log Filtering Solution  

## Solutions Considered  

### *1. Single-threaded Processing*  
*Approach:*  
- Read the log file line by line.  
- Check if each line starts with the target date.  
- Write matching lines to the output file.  

*Pros:*  
- Simple implementation.  
- Low memory usage.  

*Cons:*  
- Slow for large log files.  
- Does not utilize multi-core CPUs efficiently.  

### *2. Multithreaded Line-by-Line Processing*  
*Approach:*  
- Spawn multiple threads, each processing a chunk of lines.  
- Use a mutex to synchronize writing to the output file.  

*Pros:*  
- Parallel processing speeds up filtering.  

*Cons:*  
- Synchronization overhead due to frequent mutex locking.  
- Difficult to distribute work evenly across threads.  

### *3. Multithreaded Chunk-Based Processing (Final Solution)*  
*Approach:*  
- Read large chunks (64MB) from the log file.  
- Spawn a separate thread for each chunk.  
- Process each chunk independently and write to the output file using a mutex.  

*Pros:*  
- Efficient CPU usage with parallel processing.  
- Reduces the number of mutex locks compared to line-by-line processing.  
- Faster than a single-threaded approach.  

*Cons:*  
- Slightly higher memory usage due to chunk storage.  

---

## Final Solution Summary  

We chose the *multithreaded chunk-based processing* approach because:  
- It *balances speed and complexity* by efficiently distributing work among threads.  
- It *minimizes synchronization overhead* by reducing mutex locks to once per chunk instead of per line.  
- It *scales well* with large log files by utilizing multiple CPU cores.  

This approach provided the best trade-off between performance and ease of implementation.  

---

## Steps to Run  

### *Prerequisites*  
- Ensure you have a *C++17 or newer* compiler installed (e.g., GCC, Clang, or MSVC).  
- Make sure your system has sufficient RAM to handle large file chunks efficiently.  

### *Compilation*  

Compile the program using a C++ compiler:  

```sh
g++ -std=c++17 -pthread -o log_filter log_filter.cpp
