# FSController

The FSController is a comprehensive C++ file system utility module that provides a wide range of operations for managing files and directories, including reading and writing files using memory mapping, creating and deleting files and directories, checking file existence and type, recursively aggregating directory entries, scanning directories for specific file types, creating backups of directories, and managing file profiler structures, all while being designed to be thread-safe and optimized for performance and size, with a focus on flexibility and ease of use, and providing a robust and efficient way to interact with the file system

very easy to implement...

### Simple Instance
```cpp
#include "path/to/FSController.hpp"
#include <iostream>
#include <string>

using namespace FSControllerModule; // to use without prefixing

typedef std::string FKT; // recommended to have a unique type alias for the FK,
                         // this will be used to identify/associate files within map
int main(){
FSController<FKT> FSC; // using default constructor
// filesystem operations...
return 0;
}
```

## Structure

* stFileDescriptor
* stProfilerStackRegister
* stDirectoryCollectionStat
* stDirectoryLookup

> stFileDescriptor
```cpp
struct alignas(void *) stFileDescriptor
{
	String_t file_content{};
	String_t file_name{};    
	size_t file_size{};     
};
```

> stProfilerStackRegister
```cpp
struct alignas(void *) stProfilerStackRegister
{
	std::unordered_map<_FKType, struct stFileDescriptor> stack_register{};
	size_t reg_stack_size{};                                              
	bool extended_wiper{false}; 
	bool gc_executed{false};
};
```


> stDirectoryCollectionStat
```cpp
typedef struct alignas(void *)
{
	std::vector<String_t> registry{};
	String_t largest_file_path{};
	std::size_t registry_size{0};
	std::size_t max_size{0};
	std::size_t min_size{0};
} stDirectoryCollectionStat;
```

> stDirectoryLookup
```cpp
typedef struct alignas(void *)
{
	String_t path{};
	String_t file{};
	bool has_found{false};
} stDirectoryLookup;
```

### Read File
> read content from a file
```cpp
struct stFileDescriptor file_description = FSC.FileRead("file_to_read"); // see "structure" section...
```

### Write to File
> write content to a file
```cpp
FSC.FileWrite("file_to_write");
```

### Create File
> Create New File if file does not exist
```cpp
if (!FSC.CreateFile("new_file", "optional initial content")){
   std::cout << "Cannot Create file: " << std::strerror(errno) << "\n";
}
```

### Create Directory(recursivelly)
> Create a directory if does not exist
```cpp
if (!FSC.CreateDirectory("path/to/new/dir")){
   std::cout << "Cannot Create directory: " << std::strerror(errno) << "\n";
}
```

### Delete File
> Delete File if file does exist
```cpp
FSC.DeleteFile("path/to/file/to/delete");
```

### Check Files
> Verify if file exists
```cpp
FSC.FileExists("path/to/file/to/check");
```

### Check Type
> Verify target type
```cpp
const bool is_text_file = FSC.IsTextFile("path/to/file/to/check");
const bool is_exe_file  = FSC.IsExecutable("path/to/file/to/check");
const bool is_sym_file  = FSC.IsSymlinkFile("path/to/file/to/check");
const bool is_dir       = FSC.IsDirectory("path/to/dir");
```

### Check Files
> Verify if file exists
```cpp
FSC.FileExists("path/to/file/to/check");
```


### File/FType Lookup
> Search file or file type within directory
```cpp
  // this will throw an error if no access allowed!

  stDirectoryLookup dir_lookup = FSC.DirectoryContainsFileType("/path/to/search/dir", "something_to_search");
	std::cout << "Needle Found: " << std::boolalpha << dir_lookup.has_found << "\n";
	std::cout << "Needle: " << dir_lookup.file << "\n";
	std::cout << "Full Path to needle: " << dir_lookup.path << "\n";
```


### Create Backup of Directory
> write content to a file
```cpp
// last 2 arguments are:
// dest_override       => if true, override the destination files
// copy_empty_files    => if true, will also copy empty files
 
if (!FSC.CreateDirectoryBackup("path/to/source/dir", "path/to/backup/dir", true, true)){
  std::cout << "Cannot create directory backup!\n";
}
```

### Generate Directory Profiler
> scan a directory, and create profiles for each block, will scan the entire directory and create a structure associated with FK
```cpp
auto directoryProfiler = FSC.DirectoryProfiler("/path/to/dir/to/profile"); // map<FKT, stFileDescriptor>
conts String_t get_index("some_file_name_or_fk");

if(directoryProfiler.find(get_index)){
  struct stFileDescriptor profileDescriptor = directoryProfiler[get_index]; // or .at()
  std::cout << "profiler fk: " << profileDescriptor.file_name << "\n";
  std::cout << "file size: " << profileDescriptor.file_size << "\n";
  std::cout << "File Content: " << profileDescriptor.file_content << "\n";
}
```


### Collect Directory entries with profiling
> scan through a directory and register biggest file size found, smallest file size found, biggest file path found, a boolean value indicating operation status and a register containing the entries.
```cpp
  /**
    * string_view        - the directory to collect from
    * bool               - recursive or not
    * size_t             - threshold value, if register < threshold => register = clear
    */
stDirectoryCollectionStat profiledCollection = FC1.CollectDirectoryEntriesWithProfiling("path/to/directory", true, 1);

std::cout << "Collected " << profiledCollection.registry_size << " entries...\n";
std::cout << "Smallest entry: " << profiledCollection.min_size << "\n";
std::cout << "Largest entry: " << profiledCollection.max_size << "\n";
std::cout << "Path to Largest: " << profiledCollection.largest_file_path << "\n";
if (profiledCollection.registry_size > 0)
{
  for (auto entry = profiledCollection.registry.begin(); entry != profiledCollection.registry.end(); entry++)
  {
    std::cout << "Entry: " << entry->c_str() << "\n";
  }
}
```

## Using internal Register profiler

> FileRead() will return a file description...
```cpp
// print register size 
std::cout << "Profiler Register Size Before: " << FSC.GetStackRegisterSize() << "\n"; // 0

// register is private, retrieve with function call...
stProfilerStackRegister pRegister = FSC.GetStackPointer();

// create new file description 
stFileDescriptor FD = FSC.FileRead("file/to/read");

// register new description
FSC.RegisterNewProfiler(FD);

// print register size again
std::cout << "Profiler Register Size After: " << FSC.GetStackRegisterSize() << "\n"; // 1

// delete a profile(Safe way)
FSC.EraseRegisterProfile(FD.file_name); // filename has FK type and acts as the FK aka foreign-key

// delete a profile(call-wrapping bypass)
pRegister.eraseProfile(FD.file_name); // same shit...

```
