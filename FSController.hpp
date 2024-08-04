/**
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++-------------
 *
 *
 * This C++ code implements a comprehensive file system utility module that provides a wide
 * range of operations for managing files and directories. At the heart of the code is the
 * FSController class, a template-based class that allows for flexibility in choosing the
 * type of foreign key used to identify file profiler structures.
 * The FSController class provides a wide range of functions for interacting with the file system,
 * including reading and writing files using memory mapping. The FileRead function uses the mmap
 * system call to map a file into memory, allowing for efficient and fast access to file contents.
 * The FileWrite function, on the other hand, uses the mmap system call to map a file into memory,
 * and then uses the std::memcpy function to copy data from a buffer into the mapped memory region.
 * In addition to reading and writing files, the FSController class also provides functions for
 * creating and deleting files, checking if a file exists, is a text file, executable, or a symlink,
 * and creating and deleting directories. These functions are implemented using the std::filesystem
 * library, which provides a modern and efficient way to interact with the file system.
 * The FSController class also provides functions for recursively aggregating directory entries,
 * scanning directories for specific file types, and creating backups of directories. These functions
 * are  implemented using the std::filesystem::directory_iterator class, which provides a way to
 * iterate over the entries in a directory.
 * Another key feature of the FSController class is its ability to create, register, and manage
 * file profiler structures. A file profiler structure is a data structure that contains information
 * about a file, such as its name, size, and contents. The FSController class provides functions for
 * creating and registering these structures, as well as for performing garbage collection to ensure
 * that unused profiler structures are properly cleaned up.
 * The FSController class is designed to be thread-safe, with a mutex used to protect access to shared
 * resources. This ensures that the class can be safely used in multi-threaded environments without
 * fear of data corruption or other concurrency-related issues.
 * One notable feature of the FSController class is its use of attribute macros to specify compiler
 * optimizations and other attributes for its member functions. For example, the __0x_attr_gri macro
 * is used to specify that a function should be optimized for performance, while the __0x_attr_psrsgp
 * macro is used to specify that a function should be optimized for size.
 * The FSController class also provides a number of convenience functions for working with files and
 * directories. For example, the CreateFile function creates a new file with the specified name and
 * contents, while the DeleteFile function deletes a file with the specified name. The CreateDirectory
 * function creates a new directory with the specified name, while the DeleteDirectory function deletes
 * a directory with the specified name.
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++-------------
 *
 */
