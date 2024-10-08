# FSController


![Logo](https://github.com/Somorpher/FSController/blob/main/5ed30b80df61f3e3da9bab9bcf780025_66b6bef1ac50d.png?raw=true)

[![GNU License](https://img.shields.io/badge/License-MIT-red.svg)](https://github.com/Somorpher/FSController/blob/main/LICENSE)

The FSController is a comprehensive C++ file system utility module that provides a wide range of operations for managing files and directories, including reading and writing files using memory mapping, creating and deleting files and directories, checking file existence and type, recursively aggregating directory entries, scanning directories for specific file types, creating backups of directories, and managing file profiler structures, all while being designed to be thread-safe and optimized for performance and size, with a focus on flexibility and ease of use, and providing a robust and efficient way to interact with the file system

## implementation
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
// last 3 arguments are:
// create_backup_dir   => if true and backup dir not found, create bk dir.
// dest_override       => if true, override the destination files
// copy_empty_files    => if true, will also copy empty files
 
if (!FSC.CreateDirectoryBackup("path/to/source/dir", "path/to/backup/dir", true, true, true)){
  std::cout << "Cannot create directory backup!\n";
}
```

### Generate Directory Profiler
> scan a directory, and create profiles for each block, will scan the entire directory and create a structure associated with FK
```cpp
auto directoryProfiler = FSC.DirectoryProfiler("/path/to/dir/to/profile"); // <FKT, stFileDescriptor>
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
std::cout << "Profiler Register Size Before: " << FSC.GetRegisterSize() << "\n"; // 0

// register is private, retrieve with function call...
stProfilerStackRegister pRegister = FSC.GetStackPointer();

// create new file description 
stFileDescriptor FD = FSC.FileRead("file/to/read");

// register new description
FSC.RegisterNewProfile(FD);

// print register size again
std::cout << "Profiler Register Size After: " << FSC.GetRegisterSize() << "\n"; // 1

// delete a profile(Safe way)
FSC.DeleteProfile(FD.file_name); // filename has FK type and acts as the FK aka foreign-key

// delete a profile(e-chain bypass)
pRegister.eraseProfile(FD.file_name); // same shit...

```


### More In-Depth implementation

```cpp
#include "path/to/FSController.hpp"

using namespace FSControllerModule;

typedef String_t FC_FK; // use this shit for foreign key association...

static const std::set<String_t> CreateTestFiles(std::set<String_t> &use_set)
{
	std::set<String_t> output_set;
	if (use_set.size() > 0)
	{
		for (const String_t &s_entry : use_set)
		{
			std::size_t u_counter(0);
			while (++u_counter <= 3)
			{
				String_t _f(s_entry);
				_f += "_file";
				_f += std::to_string(u_counter);
				_f += ".txt";
				output_set.insert(_f);
			}
		}
	}
	return output_set;
};

// looks ugly but who cares...
String_t randomValue(int x) {
	std::vector<String_t> random_values{"vlkds", "skdlsd dsldksd", "sldl", "slkdsdks dsdksdnksds", "sdlkksdls", "sldklsdsdsds", "sdklklds sdlskdlskdsd", "sldkskd dsldksldksd dslkdsldklsds", 
	"sdlksd s;dld sdlskdlskdlsdsds", "sdlks dsdsdlskds dsdksdnk3nw dsfd", "sdsd sdksnds dsdksdks dks dks dksd", "sdlkds Dsdnsk ds dskdsds dsdks d", "sdkld dsldslkds dsdsd", "kdslskldksd ksndksjdlksjdlsjdljoewjdsd sdks dks dks k dks dsds", 
	"sldkslds dsds ds ds dks ds dsk dksdksdlisdos dsds", "sdlksd sdsdsds", "sl", "sdklskds dsldlsdklskdsd sdlskdsdsd", "dkslkdsd dsldsjscsad sadsa  sadsaijdlksajdsadjsadsad sad sadsadsa dsa dsadksadksajdsa dsajd sajdsadksaa"};
	return random_values[x];
};

int main(int argc, char *argv[])
{
	try
	{
		String_t get_home_dir(getenv("HOME"));
		if (!get_home_dir.empty())
		{
			if (get_home_dir.find_last_of("/") != std::string::npos && get_home_dir.find_last_of("/") != get_home_dir.length() - 1)
			{
				get_home_dir += "/";
			}
			String_t TEST_DIRECTORY(get_home_dir + "fstest");

			std::cout << "Using test directory = " << TEST_DIRECTORY << "\n";

			// full file controller implementation example...

			FSController<String_t> fsc;

			std::cout << "FsController ID: " << fsc.GetFsControllerUID() << "\n";
			std::cout << "FsController Initial Register size: " << fsc.GetRegisterSize() << "\n";

			// create a fstest directory for testing...
			std::cout << "Checking <" << TEST_DIRECTORY << "> exists...\n";
			if (!fsc.IsDirectory(TEST_DIRECTORY))
			{
				std::cout << "<" << TEST_DIRECTORY << "> does not exist, creating <" << TEST_DIRECTORY << "> directory now...\n";
				fsc.CreateDirectory(TEST_DIRECTORY);
			}
			if (fsc.IsDirectory(TEST_DIRECTORY))
			{
				std::cout << "Directory <" << TEST_DIRECTORY << "> created!\n";
			}
			else
			{
				std::cout << "Directory <" << TEST_DIRECTORY << "> not created!\n";
				return 1;
			}

			std::set<String_t> destination_path_test_file{
				String_t(TEST_DIRECTORY + "/test_d1/td1_dir1/"),
				String_t(TEST_DIRECTORY + "/test_d1/td1_dir2/"),
				String_t(TEST_DIRECTORY + "/test_d2/td2_dir1/"),
				String_t(TEST_DIRECTORY + "/test_d2/td2_dir2/"),
				String_t(TEST_DIRECTORY + "/test_d2/td2_dir3/"),
				String_t(TEST_DIRECTORY + "/test_d3/td3_dir1/"),
				String_t(TEST_DIRECTORY + "/test_d3/td3_dir2/"),
			};

			std::set<String_t> test_files = CreateTestFiles(destination_path_test_file);

			std::size_t dir_gen_counter(0), file_gen_counter(0);

			std::cout << "total test directories/files created: " << destination_path_test_file.size() << "/" << test_files.size() << "\n";

			if (test_files.size() > destination_path_test_file.size())
			{
				// Create directories first, then files...

				std::cout << "Creating now test directories ...\n";

				for (const String_t &dir_path_entry : destination_path_test_file)
				{

					std::cout << "checking if directory '" << dir_path_entry << "' exists... ";
					if (fsc.IsDirectory(dir_path_entry))
					{
						++dir_gen_counter;
						std::cout << " found! Skipping...\n";
						continue;
					}

					std::cout << " not found! Create now ... ";
					if (fsc.CreateDirectory(dir_path_entry))
					{
						++dir_gen_counter;
						std::cout << " done!\n";
					}
					else
					{
						std::cout << " failed!\n";
						break;
					}
				}
				if (dir_gen_counter == destination_path_test_file.size())
				{
					std::cout << "All test directories created!\n";

					for (const String_t &t_file : test_files)
					{
						std::cout << "Checking if file <" << t_file << "> exists... ";
						if (fsc.IsTextFile(t_file))
						{
							++file_gen_counter;
							std::cout << "skipping!\n";
							continue;
						}
						std::cout << "create now ... ";
						if (fsc.CreateFile(t_file))
						{
							++file_gen_counter;
							std::cout << "done!\n";
						}
						else
						{
							std::cout << "failed!\n";
							break;
						}
					}
					if(file_gen_counter == test_files.size()){
						std::cout << "All test files created!\n";

						// Write something into each file, content must vary...
						int x = 0;
						for(const auto& file: test_files){
							String_t a = randomValue(++x < 19 ? x : 2);
							if(fsc.FileExists((StringView_t)file)){
								String_t write_content("content into file path ");
								write_content+=file;
								write_content+=" = ";
								write_content+=a;
								fsc.FileWrite((StringView_t)file, (StringView_t)write_content, false);
							}
						}

						// now read content from all new test files...
						auto dir_profiler = fsc.DirectoryProfiler(TEST_DIRECTORY);
						std::cout << "Create test directory profiler...";
						if(dir_profiler.size() > 0){
							std::cout << " success!\n";
							std::cout << "Will list each profiler with name -> size -> content.\n";
							for(const auto& profiler: dir_profiler){
								std::cout << "[%] Profiler ID(aka FK): " << profiler.first << "\n";
								std::cout << "[%] Profiler Size:       " << profiler.second.file_size << "\n";
								std::cout << "[%] Profiler Content:    \n" << profiler.second.file_content << "\n\n";
							}

							// work with internal profiler register
							std::cout << "Internal Stack Register Size: " << fsc.GetRegisterSize() << "\n";

							std::cout << "allocating new profile descriptors into register...\n";
							fsc.RegisterNewProfile(dir_profiler, false);
							std::cout << "Register size after profile allocation: " << fsc.GetRegisterSize()<<"\n";
						    std::cout << "dir_profiler after allocation with ? move: " << dir_profiler.size() << "\n";
							
							// look for specific file types within directory
							auto scan_result = fsc.DirectoryContainsFileType<stDirectoryLookup>(TEST_DIRECTORY, ".txt", true);

							std::cout << "File Type Scan Result: \n";
							std::cout << "File Type Found: " << std::boolalpha << scan_result.has_found << "\n";
							std::cout << "FileName: " << scan_result.file << "\n";
							std::cout << "FilePath: " << scan_result.path << "\n";

							std::cout << "Total Directory Size: " << fsc.GetDirectorySize(TEST_DIRECTORY) << '\n';

							std::cout << "listing last time test directory files...\n";

							for(const auto&[fk, profiler]: dir_profiler){
								std::cout << "[+] " << fk << " = " << std::boolalpha << fsc.FileExists(fk) << '\n';
							}

							std::cout << "Creating backup of " << TEST_DIRECTORY << " before wiping!\n";

							if(fsc.CreateDirectoryBackup(TEST_DIRECTORY, "/home/user/FSTEST_BACKUP", true, true, true)){
								std::cout << "Directory Backup created successfully!\n";
							}else{
								std::cout << "Cannot create backup directory!\n";
							}
 
							std::cout << "pausing 3 seconds...\n";
							int s(0);
							while(s++ <= 3){
								std::cout << "wipe all in ... " << s << "\n";
								std::this_thread::sleep_for(std::chrono::seconds(1));
							}
							std::cout << "Wiping test directory content...\n";
							
							fsc.DirectoryWipe(TEST_DIRECTORY, true);

							std::cout << "listing test directory files after wipe...\n";

							for(const auto&[fk, profiler]: dir_profiler){
								std::cout << "[+] " << fk << " = " << std::boolalpha << fsc.FileExists(fk) << '\n';
							}

							std::cout << "Total Directory Size After Wipe: " << fsc.GetDirectorySize(TEST_DIRECTORY) << '\n';
						}else{
							std::cout << " failure!\n";
							return 1;
						}
					}else{
						std::cout << "Some test files were not created, result counter: " << file_gen_counter << "\n";
						return 1;
					}
				}
				else
				{
					std::cout << "Some test directories were not created!\n";
					return false;
				}
			}
			else
			{
				std::cout << "Error creating test file set container...\n";
				return 1;
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}

	return EXIT_SUCCESS;
}

```
