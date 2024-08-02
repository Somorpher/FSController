#pragma once

/**
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++-------------
 *
 * File System Utility Module, create new files, delete files, write to files,
 * read from files, rename files, create directories, delete directories,
 * backup directories, and other file system related operations...
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++-------------
 *
 */

/*                     Headers/Libs                      *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <algorithm>
#include <complex>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <new>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <thread>
#include <type_traits>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/**
 * Namespace Pollution Guard, naming-collision prevention
 *
 */
#ifndef __MODULE_FS_CONTROLLER__
#define __MODULE_FS_CONTROLLER__

/*                     Namespace                         *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
namespace FSControllerModule {

#define IS_DEBUGGING_CODE 1 // remove this definition for production

/**
 * Os/Architecture Detection
 *
 */
#if defined(__linux__) || defined(__APPLE__)
/*                      Global Macro                     *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define __SUPPORTED_COMPILER_1__ __GNUC__
#define __SUPPORTED_COMPILER_2__ __clang__
#define FS_MAX_FILE_NAME_LENGTH (std::uint16_t)200         /* the max length for individual absolute address */
#define FS_MAX_COLLECTION_STACK_SIZE (std::uint32_t)100000 /* max value for instance aggregation size */

#define __tm_file_aggregation template <typename _Ft, typename = std::enable_if<!std::is_array_v<_Ft> && !std::is_pointer_v<_Ft>>>

/**
 * Random Number Generator Static Function(Namespace Encapsulated)
 */
static const std::uint64_t GenerateRandomId(void) noexcept {
  std::random_device _d;
  std::mt19937 _eng(_d());
  std::uniform_int_distribution<uint64_t> _g(9999999999999, 9999999999999999);
  return _g(_eng);
};

/**
 * Debugging configuration mode, remove this macro for production use!
 */

#ifdef IS_DEBUGGING_CODE
#if !defined(__with_optimize_perform__)
#define __with_optimize_perform__ optimize("0") // 0 optimization for compiler
#endif
#else
#if !defined(__with_optimize_perform__)
#define __with_optimize_perform__ optimize("3") // 3 max optimization
#endif
#endif

/**
 * Compiler Detection, only implement with supported compiler installed
 */
#if defined(__SUPPORTED_COMPILER_1__) || defined(__SUPPORTED_COMPILER_2__)

/*                      Type Alias                       *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
using String_t = std::basic_string<char>;
using StringView_t = std::basic_string_view<char>;

/*                      Enumeration                      *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*                      Structure                        *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
struct alignas(void *) stFileProfiler {
  String_t file_content{}; /* contains file content */
  String_t file_name{};    /* full path(absolute path) to file*/
  size_t file_size{};      /* file size in bytes */

  /* Clean struct file associated information */
  inline void Clean() noexcept {
    file_content.clear();
    file_name.clear();
  };

  ~stFileProfiler() = default;
};

/**
 *
 * file profile register structure, register file profiler structure block within "stack", and profile stack register size within "reg_stack_size" member variables
 */
__tm_file_aggregation struct stProfilerStackRegister {
  std::unordered_map<_Ft, struct stFileProfiler> stack_register{}; /* stack register, allocates file name(abs. address) as key and new file descriptor instance. */
  size_t reg_stack_size{};                                         /* address register stack size */

  /**
   *
   * Helper Utility Function, locate and get fs description at index _i.
   * @param _Ft the index to search for
   * @returns struct stFileProfiler, the descriptor at
   */
  inline const struct stFileProfiler getProfile(const _Ft _profile_id) noexcept {
    if (reg_stack_size > 0) [[likely]] {
      if (stack_register.find(_profile_id) != stack_register.end()) {
        return stack_register[_profile_id];
      }
    }
    return {};
  };

  inline void joinProfile(const struct stFileProfiler &_new_profile) noexcept {
    if (++reg_stack_size < FS_MAX_COLLECTION_STACK_SIZE - 1) [[likely]] {
      if (stack_register.find(_new_profile.file_name) == stack_register.end()) {
        stack_register.insert({_new_profile.file_name, std::move(_new_profile)});
      }
      return;
    }
    --reg_stack_size;
  };

  ~stProfilerStackRegister(void) noexcept = default;
};

/*                          Class                        *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/**
 * @class FSController
 * template parameter type is used to identify file profiler, and it is the file name itself.
 * this type should be a string-like or other form of profiler FK(foreign-key) identifier compatible.
 * if using numeric fk types, fk must come from a hex or some other kind of transformation that
 * can be used as identifier, for instance:
 *
 * -------------------------------------------------
 *                                                 |
 * int fk = fromStringToHex("FILE_NAME_STRING");   |
 *                                                 |
 *                    or                           |
 *                                                 |
 * int fk = fromStringToInt("FILE_NAME_STRING");   |
 *                                                 |
 * -------------------------------------------------
 *
 * in such a way that it can be possible to reverse the key value into its original one.
 *
 */
template <typename _ForeignKeyType_ = String_t> class FSController {

private:
  struct stProfilerStackRegister<_ForeignKeyType_> _profile_stack_reg; /* file profile stack register */

  std::uint64_t _fs_instance_uid = GenerateRandomId(); /* fs instance unique id, for copy/move semantics, avoid copy same object */

  bool _fs_new_instance = false; /* boolean flag indicating if instance is new or used, double-free error prevention */

public:
  /* FS Controller default Constructor */
  explicit FSController() noexcept {
    if (!this->_fs_new_instance)
      this->_fs_new_instance = true;
  };

  /* FS Controller Copy Constructor */
  FSController(const FSController &_o) noexcept : _profile_stack_reg(_o._profile_stack_reg), _fs_instance_uid(_o._fs_instance_uid), _fs_new_instance(_o._fs_instance_uid) {};

  /* FS Controller Copy Operator Overload */
  FSController &operator=(const FSController &_o) noexcept { return *this->__initFsController<const FSController &>(_o); };

  /* FS Controller Move Constructor */
  FSController(FSController &&_o) noexcept
      : _profile_stack_reg(std::move(_o._profile_stack_reg)), _fs_instance_uid(std::move(_o._fs_instance_uid)), _fs_new_instance(std::move(_o._fs_instance_uid)) {};

  /* FS Controller Move Operator Overload */
  FSController &operator=(FSController &&_o) noexcept { return *this->__initFsController<FSController &&>(std::move(_o)); };

  /* FS Controller Comparision Operator Overload */
  constexpr bool operator==(const FSController &_o) noexcept { return this->_fs_instance_uid == _o._fs_instance_uid; };
  constexpr bool operator!=(const FSController &_o) noexcept { return !(this->_fs_instance_uid == _o._fs_instance_uid); };

  /**
   *
   * check if file_name exists or not.
   * @param StringView_t absolute path to target file
   * @returns bool true if file_name is found, false otherwise
   *
   */
  inline static const bool FileExists(const StringView_t file_name) noexcept {
    if (file_name.empty() || file_name.size() > FS_MAX_FILE_NAME_LENGTH || file_name.find(" ") != std::string::npos) [[unlikely]]
      return false;

    return std::filesystem::exists(file_name.data());
  };

  /**
   *
   * check if every file within file_list exists or not, will return failure if even 1 is missing.
   * @param std::unordered_set<String_t> list with target files
   * @returns bool true if every target within file_list is found, false otherwise
   *
   */
  inline static const bool FileExists(const std::unordered_set<String_t> file_list) noexcept {
    if (file_list.empty() && file_list.size() > 500000) [[unlikely]]
      return false;

    for (const String_t &file_name : file_list)
      if (!FileExists(file_name))
        return false;

    return true;
  };

  /**
   *
   * check if file_name is a text file
   * @param StringView_t absolute path to target file
   * @returns bool true if file_name is a text file, false otherwise
   *
   */
  inline static const bool IsTextFile(const StringView_t file_name) noexcept { return std::filesystem::is_regular_file(file_name.data()); };

  /**
   *
   * check if file_name is a executable
   * @param StringView_t absolute path to target file
   * @returns bool true if file_name is executable, false otherwise
   *
   */
  inline static const bool IsExecutable(const StringView_t file_name) noexcept {
    if (IsTextFile(file_name.data())) {
      std::filesystem::perms f_perms = std::filesystem::status(file_name.data()).permissions();
      if (((f_perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) || ((f_perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none) ||
          ((f_perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none))
        return true;
    }
    return false;
  };

  /**
   *
   * check if file_name is a symlink
   * @param StringView_t absolute path to target symlink
   * @returns bool true if file_name is symlink, false otherwise
   *
   */
  inline static const bool IsSymlink(const StringView_t file_name) noexcept { return std::filesystem::is_symlink(file_name.data()); };

  /**
   *
   * check if file_name is a directory
   * @param StringView_t absolute path to target file
   * @returns bool true if file_name is directory, false otherwise
   *
   */
  inline static const bool IsDirectory(const StringView_t file_name) noexcept { return std::filesystem::is_directory(file_name.data()); };

  /**
   *
   * Read _file_name content and return full profiler structure associated with _file_name.
   * @param StringView_t the absolute file path to read
   * @param bool a const boolean flag dictating if _file_name should be created on missing or not
   * @returns stFileProfiler a const read-only access to _file_name associated profile structure
   *
   */
  inline static const struct stFileProfiler FileRead(const StringView_t _file_name, const bool _create_new = false) {
    struct stFileProfiler new_profiler {
      .file_content{}, .file_name{_file_name.data()}, .file_size { 0 }
    };
    int fileDescriptor = open(_file_name.data(), O_RDONLY);
    if (fileDescriptor == -1) [[unlikely]]
      throw std::runtime_error("Error opening file");

    struct stat file_stat_description;

    if (fstat(fileDescriptor, &file_stat_description) == -1) [[unlikely]]
      throw std::runtime_error("Error getting file size");

    const off_t map_offset(0);

    new_profiler.file_size = file_stat_description.st_size;

    char *mapped_data_pointer(static_cast<char *>(mmap(NULL, file_stat_description.st_size, PROT_READ, MAP_PRIVATE, fileDescriptor, map_offset)));

    if (mapped_data_pointer == MAP_FAILED) [[unlikely]]
      throw std::runtime_error("Error mapping file");

    new_profiler.file_content.assign(static_cast<char *>(mapped_data_pointer), file_stat_description.st_size);

    if (munmap(mapped_data_pointer, file_stat_description.st_size) == -1) [[unlikely]]
      throw std::runtime_error("Cannot unmap address!");

    close(fileDescriptor);
    return new_profiler;
  };

  /**
   *
   * put _buffer into file _fle_name, create new _file_name if _create_new is true and _file_name is not found.
   * @param StringView_t read only argument value representing absolute path to _file_name
   * @param StringView_t read only argument value holding data to be written into _file_name
   * @param bool const boolean flag dictating if _file_name should be create if not found
   * @returns void
   *
   */
  inline static void FileWrite(const StringView_t _file_name, const StringView_t _buffer, const bool _create_new = false) {
    int fileDescriptor = open(_file_name.data(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fileDescriptor == -1) [[unlikely]]
      throw std::runtime_error("cannot open file to write!");

    const off_t file_size(_buffer.length());

    if (ftruncate(fileDescriptor, file_size) == -1) [[unlikely]]
      throw std::runtime_error("Cannot truncate file!");

    char *mapped_data(static_cast<char *>(mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0)));
    if (mapped_data == MAP_FAILED) [[unlikely]]
      throw std::runtime_error("error mapping file!");

    std::memcpy(mapped_data, _buffer.data(), _buffer.length());

    if (munmap(mapped_data, file_size) == -1) [[unlikely]]
      throw std::runtime_error("Cannot unmap address!");

    close(fileDescriptor);
  };

  /**
   *
   * Register new Profile, propagate _new_profile into local register.
   * @param struct stFileProfiler new profile to allocate
   * @returns void
   *
   */
  inline void RegisterNewProfiler(const struct stFileProfiler _new_profile) noexcept {
    if (_new_profile.file_size > 0) [[likely]]
      this->_profile_stack_reg.joinProfile(std::move(_new_profile));
  };

  /**
   *
   * Get profiler register stack size
   * @returns size_t the size of register
   *
   */
  constexpr size_t &GetStackRegisterSize(void) noexcept { return this->_profile_stack_reg.reg_stack_size; };

  /**
   *
   * Get Instance UID
   * @returns std::uint64_t the instance associated uid
   *
   */
  constexpr std::uint64_t &GetFsControllerUID(void) noexcept { return this->_fs_instance_uid; };

  /**
   *
   * Get profiler structure data block at _profile_id index.
   * @param _ForeignKeyType_& reference to register profiler associated FK assigned at allocation
   * @returns stFileProfiler const struct description(profiler) identified by _profile_id or empty descriptor if not found
   *
   */
  inline const struct stFileProfiler GetStackProfile(const _ForeignKeyType_ &_profile_id) noexcept { return this->_profile_stack_reg.get(_profile_id); };

  ~FSController() noexcept {
    if (this->_fs_new_instance) [[likely]]
      this->_fs_new_instance = false;
  };

private:
  /**
   *
   * initialize object, either copy or move from _o depending on _tN value, template requires _tN class type.
   * @param _tN the object to move/copy
   * @returns FSController the pointer to this object
   *
   */
  template <typename _tN, typename = std::enable_if<std::is_class_v<_tN>>> inline FSController *__initFsController(_tN _o) noexcept {
    std::cout << "is reference: " << std::boolalpha << std::is_reference_v<_tN> << "\n";
    if (*this != _o) {
      this->_profile_stack_reg = std::is_rvalue_reference_v<_tN> ? std::move(_o._profile_stack_reg) : _o._profile_stack_reg;
      this->_fs_new_instance = std::is_rvalue_reference_v<_tN> ? std::move(_o._fs_new_instance) : _o._fs_new_instance;
      this->_fs_instance_uid = std::is_rvalue_reference_v<_tN> ? std::move(_o._fs_instance_uid) : _o._fs_instance_uid;
    }
    return this;
  };
};
#endif

#endif

}; // namespace FSControllerModule

typedef FSControllerModule::FSController<> _FSC_;

#endif
