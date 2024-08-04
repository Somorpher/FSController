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
#include <atomic>
#include <complex>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
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
namespace FSControllerModule
{

#define IS_DEBUGGING_CODE 1 // remove this definition for production

/**
 * Os/Architecture Detection
 *
 */
#if defined(__linux__) || defined(__APPLE__)

/*                      Global Macro                     *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef __GNUC__
#define __SUPPORTED_COMPILER_1__ __GNUC__
#endif

#ifdef __clang__
#define __SUPPORTED_COMPILER_2__ __clang__
#endif

#ifdef IS_DEBUGGING_CODE
#define ATTR_OPTIMIZE_LEVEL "0"
#else
#define ATTR_OPTIMIZE_LEVEL "3"
#endif

#define FS_MAX_FILE_NAME_LENGTH (std::uint16_t)200         /* the max length for individual absolute address */
#define FS_MAX_COLLECTION_STACK_SIZE (std::uint32_t)100000 /* max value for instance aggregation size */

/* FKType is the foreign key type name to use for entity associations */
#define __tm_file_aggregation template <typename _FKType, typename = std::enable_if<!std::is_array_v<_FKType> && !std::is_pointer_v<_FKType>>>

/**
 * Compiler Detection, only implement attributes with supported compiler installed
 */
#if defined(__SUPPORTED_COMPILER_1__) || defined(__SUPPORTED_COMPILER_2__)

#define __0x_attr_gri __attribute__((hot, no_icf, warn_unused_result, nothrow, pure, zero_call_used_regs("all"), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_psrsgp __attribute__((no_icf, warn_unused_result, nothrow, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_psrscp __attribute__((no_icf, nothrow, always_inline, access(read_only, 1), zero_call_used_regs("all"), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_cc __attribute__((no_icf, nothrow, always_inline, cold, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_mc __attribute__((no_icf, nothrow, always_inline, cold, access(read_write, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_co __attribute__((no_icf, nothrow, always_inline, const, cold, warn_unused_result, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_fe __attribute__((no_icf, nothrow, always_inline, flatten, pure, hot, warn_unused_result, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_itf __attribute__((no_icf, nothrow, always_inline, flatten, pure, warn_unused_result, no_stack_protector, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_iex __attribute__((no_icf, nothrow, always_inline, flatten, pure, warn_unused_result, no_stack_protector, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_isl __attribute__((no_icf, nothrow, always_inline, flatten, pure, warn_unused_result, no_stack_protector, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_id __attribute__((no_icf, nothrow, always_inline, flatten, pure, warn_unused_result, no_stack_protector, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_fr __attribute__((no_icf, warn_unused_result, hot, stack_protect, zero_call_used_regs("used"), access(read_only, 1), access(read_only, 2), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_fw __attribute__((no_icf, stack_protect, hot, zero_call_used_regs("used"), access(read_only, 1), access(read_only, 2), access(read_only, 3), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_rnp __attribute__((no_icf, nothrow, stack_protect, hot, zero_call_used_regs("used"), access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_gss __attribute__((no_icf, nothrow, noipa, no_stack_protector, pure, flatten, warn_unused_result, no_sanitize_address, no_sanitize_coverage, no_sanitize_undefined, optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_gci __attribute__((no_icf, nothrow, noipa, no_stack_protector, pure, flatten, warn_unused_result, no_sanitize_address, no_sanitize_coverage, no_sanitize_undefined, optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_gsp __attribute__((no_icf, nothrow, stack_protect, warn_unused_result, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_ifc __attribute__((no_icf, nothrow, stack_protect, warn_unused_result, cold, flatten, access(read_write, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_cpm __attribute__((no_icf, nothrow, stack_protect, warn_unused_result, hot, flatten, pure, access(read_only, 1), access(read_only, 2), access(read_only, 3), zero_call_used_regs("used"), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_ofd __attribute__((no_icf, stack_protect, warn_unused_result, hot, flatten, pure, access(read_only, 1), access(read_only, 2), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_cfs __attribute__((no_icf, warn_unused_result, flatten, pure, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_vms __attribute__((no_icf, flatten, stack_protect, zero_call_used_regs("used"), access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_amb __attribute__((no_icf, nothrow, flatten, stack_protect, tainted_args, zero_call_used_regs("used"), access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_dmc __attribute__((no_icf, flatten, stack_protect, zero_call_used_regs("all"), access(read_write, 1), access(read_write, 2), access(read_only, 3), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_drz __attribute__((no_icf, flatten, stack_protect, always_inline, access(read_only, 1), access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_cps __attribute__((no_icf, flatten, nothrow, pure, warn_unused_result, no_stack_protector, noipa, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL)))
#define __0x_attr_FSC_cmb __attribute__((no_icf, flatten, nothrow, stack_protect, zero_call_used_regs("used"), access(write_only, 1), access(read_only, 2), optimize(ATTR_OPTIMIZE_LEVEL)))

#else

#define __0x_attr_gri [[nodiscard, nothrow]]
#define __0x_attr_psrsgp [[nodiscard, nothrow]]
#define __0x_attr_psrscp [[nothrow]]
#define __0x_attr_FSC_cc [[nothrow, nodiscard]]
#define __0x_attr_FSC_mc [[nothrow, nodiscard]]
#define __0x_attr_FSC_co [[nothrow, nodiscard]]
#define __0x_attr_FSC_fe [[nothrow, nodiscard]]
#define __0x_attr_FSC_itf [[nothrow, nodiscard]]
#define __0x_attr_FSC_iex [[nothrow, nodiscard]]
#define __0x_attr_FSC_isl [[nothrow, nodiscard]]
#define __0x_attr_FSC_id [[nothrow, nodiscard]]
#define __0x_attr_FSC_fr [[nodiscard]]
#define __0x_attr_FSC_fw [[nodiscard]]
#define __0x_attr_FSC_rnp [[nothrow]]
#define __0x_attr_FSC_gss [[nothrow, nodiscard]]
#define __0x_attr_FSC_gci [[nothrow, nodiscard]]
#define __0x_attr_FSC_gsp [[nothrow, nodiscard]]
#define __0x_attr_FSC_ifc [[nothrow, nodiscard]]
#define __0x_attr_FSC_cpm [[nothrow, nodiscard]]
#define __0x_attr_FSC_ofd [[nodiscard]]
#define __0x_attr_FSC_ofs [[nodiscard]]
#define __0x_attr_FSC_vms [[nothrow]]
#define __0x_attr_FSC_amb [[nothrow]]
#define __0x_attr_FSC_dmc [[nothrow]] // ?
#define __0x_attr_FSC_drz [[nothrow]] // ?
#define __0x_attr_FSC_cps [[nothrow, nodiscard]]
#define __0x_attr_FSC_cmb [[nothrow]]

#endif

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
 * Random Number Generator Static Function(Namespace Encapsulated)
 */
__0x_attr_gri static const std::uint64_t GenerateRandomId(void) noexcept
{
    std::random_device _d;
    std::mt19937 _eng(_d());
    std::uniform_int_distribution<uint64_t> _g(9999999999999, 9999999999999999);
    return _g(_eng);
};

/*                      Type Alias                       *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
using String_t = std::basic_string<char>;
using StringView_t = std::basic_string_view<char>;
typedef char *fMap_t;
typedef String_t _FKType;
typedef _FKType FKt;

/*                      Enumeration                      *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
enum class eFileDescriptorMode : uint8_t
{
    READ = 0,
    WRITE,
    READ_WRITE
};

/*                      Structure                        *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#pragma pack(1)
struct alignas(void *) stFileDescriptor
{
    String_t file_content{}; /* contains file content */
    String_t file_name{};    /* full path(absolute path) to file*/
    size_t file_size{};      /* file size in bytes */

    /* Clean struct file associated information */
    inline void Clean() noexcept
    {
        if (!file_content.empty())
            file_content.clear();

        if (!file_name.empty())
            file_name.clear();
    };

    ~stFileDescriptor() noexcept
    {
        Clean();
    };
};

/**
 *
 * file profile register structure, register file profiler structure block within "stack",
 * and profile stack register size within "reg_stack_size" member variables
 */
struct alignas(void *) stProfilerStackRegister
{
    std::unordered_map<_FKType, struct stFileDescriptor> stack_register{}; /* stack register, allocates file name(abs. address) as key and new file descriptor instance. */
    size_t reg_stack_size{};                                               /* address register stack size */

    bool extended_wiper{false}; /* what to do on GC execution, clean up program associated data or real file. */

    bool gc_executed{false};

    /**
     *
     * Helper Utility Function, locate and get fs description at index _i.
     * @param _FKType the index to search for
     * @returns struct stFileDescriptor, the descriptor at
     */
    __0x_attr_psrsgp inline const struct stFileDescriptor getProfile(const _FKType _profile_id) noexcept
    {
        if (reg_stack_size > 0) [[likely]]
        {
            if (stack_register.find(_profile_id) != stack_register.end())
            {
                return stack_register[_profile_id];
            }
        }
        return {};
    };

    __0x_attr_psrscp inline void createProfile(const struct stFileDescriptor &_new_profile) noexcept
    {
        if (++reg_stack_size < FS_MAX_COLLECTION_STACK_SIZE - 1) [[likely]]
        {
            if (stack_register.find(_new_profile.file_name) == stack_register.end())
            {
                stack_register.insert({_new_profile.file_name, std::move(_new_profile)});
            }
            return;
        }
        reg_stack_size--;
    };

    inline void eraseProfile(const _FKType _fk, const bool globally = false)
    {
        if (gc_executed)
            return;
        if (reg_stack_size > 0 && reg_stack_size-- > 0) [[likely]]
        {
            auto rsi = stack_register.find(_fk);
            if (rsi != stack_register.end()) [[likely]]
            {
                stack_register.erase(rsi);
                if (extended_wiper && globally && std::filesystem::exists(_fk)) [[unlikely]]
                {
                    if (unlink(_fk.c_str()) == -1) [[likely]]
                    {
                        throw std::runtime_error(std::move(String_t("Error Unlink File ") + _fk + ": " + std::strerror(errno)));
                    }
                }
            }
        }
    };

    inline void GarbageCollect(void)
    {
        if (!gc_executed)
        {
            gc_executed = true;
            if (reg_stack_size > 0) [[likely]]
            {
                for (auto &[fKey, profile_description] : stack_register)
                {
                    if (extended_wiper)
                    {
                        eraseProfile(profile_description.file_name, true);
                    }
                    else
                    {
                        eraseProfile(profile_description.file_name, false);
                    }
                }
            }
        }
    };

    inline void SetExtendedAccess(void) noexcept
    {
        if (!extended_wiper)
            extended_wiper = true;
    };

    inline void SetRestrictedAccess(void) noexcept
    {
        if (extended_wiper)
            extended_wiper = false;
    };

    stProfilerStackRegister() = default;

    stProfilerStackRegister(const stProfilerStackRegister &o) = default;
    stProfilerStackRegister(stProfilerStackRegister &&o) = default;

    stProfilerStackRegister &operator=(const stProfilerStackRegister &o) = default;
    stProfilerStackRegister &operator=(stProfilerStackRegister &&o) = default;

    ~stProfilerStackRegister(void) noexcept
    {
        GarbageCollect();
    };
};

#pragma pack()

/*                          Class                        *\
\*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/**
 * @class FSController
 * template parameter type is used to identify file profiler, and it is the file name
 * itself. this type should be a string-like or other form of profiler FK(foreign-key)
 * identifier compatible. if using numeric fk types, fk must come from a hex or some other
 * kind of transformation that.
 */
template <typename _ForeignKeyType_ = String_t> class FSController
{
    using directoryScanResult_t = std::unordered_map<_ForeignKeyType_, struct stFileDescriptor>;

  private:
    struct stProfilerStackRegister _profile_stack_reg; /* file profile stack register */

    std::uint64_t _fs_instance_uid =
        GenerateRandomId(); /* fs instance unique id, for copy/move semantics, avoid copy
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         same object */

    bool _fs_new_instance =
        false; /* boolean flag indicating if instance is new or used,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        double-free error prevention */

    std::mutex _mtx_guard;

  public:
    /* FS Controller default Constructor */
    explicit FSController() noexcept
    {
        if (!this->_fs_new_instance)
            this->_fs_new_instance = true;
    };

    /* FS Controller Copy Constructor */
    __0x_attr_FSC_cc FSController(const FSController &_o) noexcept : _profile_stack_reg(_o._profile_stack_reg), _fs_instance_uid(_o._fs_instance_uid), _fs_new_instance(_o._fs_instance_uid) {};

    /* FS Controller Copy Operator Overload */
    __0x_attr_FSC_cc FSController &operator=(const FSController &_o) noexcept
    {
        return *this->__initFsController<const FSController &>(_o);
    };

    /* FS Controller Move Constructor */
    __0x_attr_FSC_mc FSController(FSController &&_o) noexcept : _profile_stack_reg(std::move(_o._profile_stack_reg)), _fs_instance_uid(std::move(_o._fs_instance_uid)), _fs_new_instance(std::move(_o._fs_instance_uid)) {};

    /* FS Controller Move Operator Overload */
    __0x_attr_FSC_mc FSController &operator=(FSController &&_o) noexcept
    {
        return *this->__initFsController<FSController &&>(std::move(_o));
    };

    /* FS Controller Comparision Operator Overload */
    __0x_attr_FSC_co constexpr bool operator==(const FSController &_o) noexcept
    {
        return this->_fs_instance_uid == _o._fs_instance_uid;
    };
    __0x_attr_FSC_co constexpr bool operator!=(const FSController &_o) noexcept
    {
        return !(this->_fs_instance_uid == _o._fs_instance_uid);
    };

    /* FS Controller custom profile index deleter Operator Overload */
    inline void operator[](const _ForeignKeyType_ &_fk) noexcept
    { // deleter
        std::cout << "Operator deleter:\n";
        return;
    };

    // inline const struct stFileDescriptor operator[](const _ForeignKeyType_ &_fk)
    // noexcept { // getter
    //   return {};
    // };

    inline const bool operator[](const struct stFileDescriptor &o) noexcept
    { // pusher
        std::cout << "Operator Pusher:\n";
        return true;
    };

    /**
     *
     * check if file_name exists or not.
     * @param StringView_t absolute path to target file
     * @returns bool true if file_name is found, false otherwise
     *
     */
    __0x_attr_FSC_fe inline static const bool FileExists(const StringView_t &file_name) noexcept
    {
        if (file_name.empty() || file_name.size() > FS_MAX_FILE_NAME_LENGTH || file_name.find(" ") != std::string::npos) [[unlikely]]
            return false;
        return std::filesystem::exists(file_name.data());
    };

    /**
     *
     * check if every file within file_list exists or not, will return failure if even 1
     * is missing.
     * @param std::unordered_set<String_t> list with target files
     * @returns bool true if every target within file_list is found, false otherwise
     *
     */
    __0x_attr_FSC_fe inline static const bool FileExists(const std::unordered_set<String_t> &file_list) noexcept
    {
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
    __0x_attr_FSC_itf inline static const bool IsTextFile(const StringView_t &file_name) noexcept
    {
        return std::filesystem::is_regular_file(file_name.data());
    };

    /**
     *
     * check if file_name is a executable
     * @param StringView_t absolute path to target file
     * @returns bool true if file_name is executable, false otherwise
     *
     */
    __0x_attr_FSC_iex inline static const bool IsExecutable(const StringView_t &file_name) noexcept
    {
        if (IsTextFile(file_name.data()))
        {
            std::filesystem::perms f_perms = std::filesystem::status(file_name.data()).permissions();
            if (((f_perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) || ((f_perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none) || ((f_perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none))
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
    __0x_attr_FSC_isl inline static const bool IsSymlink(const StringView_t &file_name) noexcept
    {
        return std::filesystem::is_symlink(file_name.data());
    };

    /**
     *
     * check if file_name is a directory
     * @param StringView_t absolute path to target file
     * @returns bool true if file_name is directory, false otherwise
     *
     */
    __0x_attr_FSC_id inline static const bool IsDirectory(const StringView_t &file_name) noexcept
    {
        return std::filesystem::is_directory(file_name.data());
    };

    /**
     *
     * Read _file_name content and return full profiler structure associated with
     * _file_name.
     * @param StringView_t the absolute file path to read
     * @param bool a const boolean flag dictating if _file_name should be created on
     * missing or not
     * @returns stFileDescriptor a const read-only access to _file_name associated profile
     * structure
     *
     */
    __0x_attr_FSC_fr const struct stFileDescriptor FileRead(const StringView_t &_file_name, const bool _create_new = false)
    {

        this->__fileStreamStatusHandle(_file_name, _create_new);

        struct stFileDescriptor new_profiler(this->__createEmptyProfilerStructure(_file_name));

        int fileDescriptor(this->__openFileDescriptor(_file_name, eFileDescriptorMode::READ));

        struct stat file_stat_description(this->__createFileStat(fileDescriptor));

        if (file_stat_description.st_size > 0)
        {
            new_profiler.file_size = file_stat_description.st_size;

            fMap_t mapped_data_pointer(this->__createPointerMap(fileDescriptor, file_stat_description.st_size, true, 0));

            this->__verifyMemMapState(std::as_const(mapped_data_pointer));

            this->__allocMappedBytes(new_profiler.file_content, &mapped_data_pointer, file_stat_description.st_size);

            this->__descriptorMapClose(fileDescriptor, &mapped_data_pointer, file_stat_description.st_size);
        }
        return new_profiler;
    };

    /**
     *
     * put _buffer into file _fle_name, create new _file_name if _create_new is true and
     * _file_name is not found.
     * @param StringView_t read only argument value representing absolute path to
     * _file_name
     * @param StringView_t read only argument value holding data to be written into
     * _file_name
     * @param bool const boolean flag dictating if _file_name should be create if not
     * found
     * @returns void
     *
     */
    __0x_attr_FSC_fw inline void FileWrite(const StringView_t &_file_name, const StringView_t &_buffer, const bool _create_new = false)
    {

        this->__fileStreamStatusHandle(_file_name, _create_new, _buffer);

        int fileDescriptor(this->__openFileDescriptor(std::move(_file_name), eFileDescriptorMode::WRITE));

        const off_t file_size(_buffer.length());

        this->__descriptorResize(fileDescriptor, file_size);

        fMap_t mapped_data(this->__createPointerMap(fileDescriptor, file_size, false, 0));

        this->__verifyMemMapState(mapped_data);

        this->__copyMappedMemoryBytes(mapped_data, _buffer);

        this->__descriptorMapClose(fileDescriptor, &mapped_data, file_size);
    };

    /**
     *
     * Register new Profile, propagate _new_profile into local register.
     * @param struct stFileDescriptor new profile to allocate
     * @returns void
     *
     */
    __0x_attr_FSC_rnp inline void RegisterNewProfiler(const struct stFileDescriptor &_new_profile) noexcept
    {
        if (_new_profile.file_size > 0) [[likely]]
        {
            std::lock_guard<std::mutex> _lock(this->_mtx_guard);
            this->_profile_stack_reg.createProfile(std::move(_new_profile));
        }
    };

    /**
     *
     * Get profiler register stack size
     * @returns size_t the size of register
     *
     */
    __0x_attr_FSC_gss constexpr size_t &GetStackRegisterSize(void) noexcept
    {
        std::lock_guard<std::mutex> _lock(this->_mtx_guard);
        return this->_profile_stack_reg.reg_stack_size;
    };

    /**
     *
     * Get Instance UID
     * @returns std::uint64_t the instance associated uid
     *
     */
    __0x_attr_FSC_gci constexpr std::uint64_t &GetFsControllerUID(void) noexcept
    {
        return this->_fs_instance_uid;
    };

    /**
     *
     * Get profiler structure data block at _profile_id index.
     * @param _ForeignKeyType_& reference to register profiler associated FK assigned at
     * allocation
     * @returns stFileDescriptor const struct description(profiler) identified by
     * _profile_id or empty descriptor if not found
     *
     */
    __0x_attr_FSC_gsp inline const struct stFileDescriptor GetRegisterProfiler(const _ForeignKeyType_ &_profile_id) noexcept
    {
        std::lock_guard<std::mutex> _lock(this->_mtx_guard);
        return this->_profile_stack_reg.getProfile(_profile_id);
    };

    /**
     *
     * Get read-only internal stack register
     * @returns stProfilerStackRegister returns the internal register
     *
     */
    __attribute__((no_icf, cold, nothrow, always_inline, warn_unused_result, optimize(ATTR_OPTIMIZE_LEVEL))) inline const struct stProfilerStackRegister GetStackPointer(void) noexcept
    {
        std::lock_guard<std::mutex> _lock(this->_mtx_guard);
        return this->_profile_stack_reg;
    };

    /**
     *
     * unlink either locally or globally _profile_id(file address), if not found, nothing
     * will happen.
     * @param _ForeignKeyType_& reference to register profiler associated FK
     * @param bool if true, will unlink globally, meaning the actual file will be unlinked
     * @returns void
     *
     */
    __attribute__((no_icf, nothrow, always_inline, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL))) inline void UnlinkRegisterProfile(const _ForeignKeyType_ &_profile_id, const bool g_scope) noexcept
    {
        std::lock_guard<std::mutex> _lock(this->_mtx_guard);
        this->_profile_stack_reg.eraseProfile(_profile_id, g_scope);
    };

    /**
     *
     * Create _file_name.
     * @param StringView_t the absolute path to _file_name
     * @param StringView_t content to write into the file, defaults to nothing
     * @returns bool if created, true
     *
     */
    __attribute__((no_icf, always_inline, access(read_only, 1), access(read_only, 2), optimize(ATTR_OPTIMIZE_LEVEL))) inline const bool CreateFileNew(const StringView_t &_file_name, const StringView_t &_content = "")
    {
        std::ofstream fileCreate(_file_name.data());
        if (!fileCreate.is_open())
            throw std::runtime_error(std::move(String_t("File Create: ") + std::strerror(errno)));

        if (!_content.empty())
        {
            fileCreate.write(_content.data(), _content.length());
        }
        fileCreate.close();

        return FileExists(_file_name);
    };

    /**
     *
     * Extend profiler register scope, this will set the global variable within stack register to true,
     * in this case, when gc executes or erase(i) is called, the actual profiler with the real file descriptor will be erased.
     * @param bool if true, set extended mode, otherwise limited mode
     * @returns void
     *
     */
    __attribute__((no_icf, cold, always_inline, optimize(ATTR_OPTIMIZE_LEVEL))) inline void SetProfileRegisterExtendedMode(const bool mode) noexcept
    {
        if (mode)
            this->_profile_stack_reg.SetExtendedAccess();
        else
            this->_profile_stack_reg.SetRestrictedAccess();
    };

    /**
     *
     * Load directory recursively, aggregate every entry which is a regular file type and return the aggregation.
     * @param StringView_t& path to aggregate from
     * @returns directoryScanResult_t the aggregation
     *
     */
    __attribute__((no_icf, cold, warn_unused_result, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL))) const directoryScanResult_t RecursiveDirectoryLoad(const StringView_t &path)
    {
        directoryScanResult_t scan_result;

        if (path.empty())
            return scan_result;
        if (path.length() < FS_MAX_FILE_NAME_LENGTH)
        {
            if (!std::filesystem::path(path).is_absolute())
                throw std::runtime_error("Use an absolute path please!");
            this->__recursiveAggregation(path, scan_result);
        }
        return scan_result;
    };

    inline ~FSController() noexcept
    {
        if (this->_fs_new_instance) [[likely]]
            this->_fs_new_instance = false;
    };

  private:
    /**
     *
     * initialize object, either copy or move from _o depending on _tN value, template
     * requires _tN class type.
     * @param _tN the object to move/copy
     * @returns FSController the pointer to this object
     *
     */
    template <typename _tN, typename = std::enable_if<std::is_class_v<_tN>>> __0x_attr_FSC_ifc inline FSController *__initFsController(_tN _o) noexcept
    {
        if (*this != _o)
        {
            this->_profile_stack_reg = std::is_rvalue_reference_v<_tN> ? std::move(_o._profile_stack_reg) : _o._profile_stack_reg;
            this->_fs_new_instance = std::is_rvalue_reference_v<_tN> ? std::move(_o._fs_new_instance) : _o._fs_new_instance;
            this->_fs_instance_uid = std::is_rvalue_reference_v<_tN> ? std::move(_o._fs_instance_uid) : _o._fs_instance_uid;
        }
        return this;
    };

    /**
     *
     * Create a memory map for a file descriptor.
     *
     * @param int& fileDescriptor The file descriptor to map.
     * @param off_t& descriptor_size The size of the file descriptor.
     * @param bool read_mode Whether to open the map in read-only mode (default) or
     * read-write mode.
     * @param size_t offset_size The offset to the starting position of the map (default
     * is 0).
     *
     * @returns fMap_t The pointer to the mapped memory region.
     */
    __0x_attr_FSC_cpm inline fMap_t __createPointerMap(const int &fileDescriptor, const off_t &descriptor_size, const bool read_mode = true, const size_t &offset_size = 0) noexcept
    {
        const int proto_map_mode(read_mode ? PROT_READ : PROT_READ | PROT_WRITE);
        const int map_access_scope(read_mode ? MAP_PRIVATE : MAP_SHARED);
        const off_t map_offset = read_mode ? 0 : offset_size;
        return static_cast<fMap_t>(mmap(NULL, descriptor_size, proto_map_mode, map_access_scope, fileDescriptor, (read_mode ? map_offset : offset_size)));
    };

    /**
     *
     * Open a file descriptor for the given file name and mode.
     *
     * @param StringView_t _file_name The name of the file to open.
     * @param eFileDescriptorMode _mode The mode in which to open the file (read-only or
     * read-write).
     *
     * @returns int The file descriptor.
     *
     * @throws std::runtime_error If the file descriptor cannot be opened.
     */
    __0x_attr_FSC_ofd inline int __openFileDescriptor(const StringView_t &_file_name, const eFileDescriptorMode &_mode)
    {
        int descriptor_open(_mode == eFileDescriptorMode::READ ? open(_file_name.data(), O_RDONLY) : open(_file_name.data(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR));
        if (descriptor_open == -1) [[unlikely]]
            throw std::runtime_error(std::move(String_t("Cannot open file descriptor for file ") + _file_name.data()));
        return descriptor_open;
    };

    /**
     *
     * Create a file status structure for the given file descriptor.
     *
     * @param int& file_descriptor The file descriptor.
     *
     * @returns struct stat The file status structure.
     *
     * @throws std::runtime_error If the file status cannot be obtained.
     */
    __0x_attr_FSC_cfs inline struct stat __createFileStat(int &file_descriptor)
    {
        struct stat file_stat_description;
        if (fstat(file_descriptor, &file_stat_description) == -1) [[unlikely]]
            throw std::runtime_error(String_t("Error getting file stat") + std::strerror(errno));
        return file_stat_description;
    };

    /**
     *
     * Verify the state of a memory map.
     *
     * @param fMap_t& map_ptr The pointer to the mapped memory region.
     *
     * @throws std::runtime_error If the memory map is invalid.
     */
    __0x_attr_FSC_vms inline void __verifyMemMapState(const fMap_t &map_ptr)
    {
        if (map_ptr == MAP_FAILED) [[unlikely]]
            throw std::runtime_error(String_t("Error mapping file: ") + std::strerror(errno));
    };

    /**
     *
     * Allocate memory for a mapped region.
     *
     * @param String_t& destination_alloc The destination string.
     * @param fMap_t *__restrict__ mapped_data_pointer The pointer to the mapped memory
     * region.
     * @param off_t& map_size The size of the mapped region.
     *
     * @returns void
     */
    __0x_attr_FSC_amb inline void __allocMappedBytes(String_t &destination_alloc, const fMap_t *__restrict__ mapped_data_pointer, const off_t &map_size) noexcept
    {
        destination_alloc.assign(static_cast<fMap_t>(*mapped_data_pointer), map_size);
    };

    /**
     *
     * Close a file descriptor and unmap the associated memory region.
     *
     * @param int& fileDescriptor The file descriptor to close.
     * @param fMap_t *__restrict__ mapped_data_pointer The pointer to the mapped memory
     * region.
     * @param off_t& map_size The size of the mapped region.
     *
     * @throws std::runtime_error If the file descriptor cannot be closed or the memory
     * region cannot be unmapped.
     */
    __0x_attr_FSC_dmc inline void __descriptorMapClose(int &fileDescriptor, const fMap_t *__restrict__ mapped_data_pointer, const off_t &map_size)
    {
        if (munmap(*mapped_data_pointer, map_size) == -1) [[unlikely]]
            throw std::runtime_error(String_t("Cannot unmap address! ") + std::strerror(errno));
        close(fileDescriptor);
    };

    /**
     *
     * Resize a file descriptor.
     *
     * @param int& descriptor The file descriptor to resize.
     * @param off_t& offset_size The new size of the file descriptor.
     *
     * @throws std::runtime_error If the file descriptor cannot be resized.
     */
    __0x_attr_FSC_drz inline void __descriptorResize(int &descriptor, const off_t &offset_size)
    {
        if (ftruncate(descriptor, offset_size) == -1) [[unlikely]]
            throw std::runtime_error(String_t("Cannot resize descriptor!") + std::strerror(errno));
    };

    /**
     *
     * Create an empty profiler structure.
     *
     * @param StringView_t _file_name The name of the file (optional).
     *
     * @returns struct stFileDescriptor The empty profiler structure.
     */
    __0x_attr_FSC_cps constexpr struct stFileDescriptor __createEmptyProfilerStructure(const StringView_t _file_name = "") noexcept
    {
        return stFileDescriptor{.file_content{""}, .file_name{static_cast<String_t>(_file_name)}, .file_size{0}};
    };

    /**
     *
     * Copy data from a mapped memory region to a destination string.
     *
     * @param fMap_t& from_map The pointer to the mapped memory region.
     * @param StringView_t destination The destination string.
     *
     * @returns void
     */
    __0x_attr_FSC_cmb inline void __copyMappedMemoryBytes(const fMap_t &destination, const StringView_t &source) noexcept
    {
        std::memcpy(destination, source.data(), source.length());
    };

    __attribute__((no_icf, always_inline, access(read_only, 1), optimize(ATTR_OPTIMIZE_LEVEL))) inline void __fileStreamStatusHandle(const StringView_t &_file_name, const bool _create_new, const StringView_t &_buffer = "")
    {
        if (!FileExists(_file_name))
        {
            if (_create_new)
            {
                if (!this->CreateFileNew(_file_name, _buffer))
                {
                    throw std::runtime_error(std::move(String_t("Cannot Create File ") + _file_name.data()));
                }
            }
            else
            {
                throw std::runtime_error(std::move(String_t("File do not exist: ") + _file_name.data()));
            }
        }
    };

    __attribute__((hot, nothrow, access(read_only, 1), stack_protect, zero_call_used_regs("used"), optimize(ATTR_OPTIMIZE_LEVEL))) inline void __recursiveAggregation(const StringView_t &_p, directoryScanResult_t &recursive_scan) noexcept
    {
        if (IsDirectory(_p))
        {
            std::filesystem::directory_iterator rdi(_p);
            for (auto &dir_entry : rdi)
            {
                if (dir_entry.is_directory())
                {
                    __recursiveAggregation(static_cast<StringView_t>(dir_entry.path().string()), recursive_scan);
                }
                else
                {
                    struct stFileDescriptor new_description = this->FileRead(dir_entry.path().string(), false);
                    if (new_description.file_size > 0)
                    {
                        recursive_scan.insert_or_assign(static_cast<_ForeignKeyType_>(new_description.file_name), std::move(new_description));
                    }
                }
            }
        }
    };
};

#endif

}; // namespace FSControllerModule

typedef FSControllerModule::FSController<> _FSC_;

#endif
