#pragma once

/**
 *
 * --------------------------------------------------------------------
 *
 * File System Utility Module, create new files, delete files, write to files,
 * read from files, rename files, create directories, delete directories,
 * backup directories, and other file system related operations...
 *
 * --------------------------------------------------------------------
 *
 */
#ifndef _GLIBCXX_STRING
#include <string>
#endif

#ifndef _GLIBCXX_UTILITY
#include <utility>
#endif

#ifndef _NEW_
#include <new>
#endif

#ifndef _GLIBCXX_ALGORITHM
#include <algorithm>
#endif

#ifndef _GLIBCXX_IOSTREAM
#include <iostream>
#endif

#ifndef __cpp_lib_filesystem
#include <filesystem>
#endif

#include <complex>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <thread>
#include <type_traits>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#if defined(__linux__) || defined(__APPLE__)

#ifndef __MODULE_FS_CONTROLLER__
#define __MODULE_FS_CONTROLLER__

namespace FSControllerModule
{

static const uint64_t GenerateRandomId(void) noexcept {
    std::random_device _d;
    std::mt19937 _eng(_d());
    std::uniform_int_distribution<uint64_t> _g(9999999999999, 9999999999999999);
    return _g(_eng);
};

// defining this macro for debugging purposes, remove for production mode.
#define IS_DEBUGGING_CODE 1 // remove this definition for production


/*                      DEBUGGING                        *\
\*-------------------------------------------------------*/
#ifdef IS_DEBUGGING_CODE
#if !defined(__with_optimize_perform__)
#define __with_optimize_perform__ optimize("0")
#endif
#else
#if !defined(__with_optimize_perform__)
#define __with_optimize_perform__ optimize("3")
#endif
#endif

/**
 * GNU or CLANG compiler detection
 */
#if defined(__GNUC__) || defined(__clang__)

#else

#endif


/*                      Global Macro                     *\
\*-------------------------------------------------------*/
#define FS_MAX_FILE_NAME_LENGTH (uint16_t)200
#define FS_MAX_COLLECTION_STACK_SIZE (uint32_t)100000

/*                      Type Alias                       *\
\*-------------------------------------------------------*/

#if !defined(__UNIT_TYPES__)
// defining these alias for type consistency
namespace
{
using String_t = std::basic_string<char>;
using StringView_t = std::basic_string_view<char>;
}; // namespace

#else
using namespace Types;
#endif

/*                      Enumeration                      *\
\*-------------------------------------------------------*/


/*                      Structure                        *\
\*-------------------------------------------------------*/
struct alignas(void *) _sFm_
{
    String_t bytes{};
    String_t fname{};
    size_t size{};

    inline void Clean() noexcept
    {
        bytes.clear();
    };

    ~_sFm_() = default;
};

using stFileInfo = _sFm_;

template <typename _Ft, typename = std::enable_if<!std::is_array_v<_Ft> && !std::is_pointer_v<_Ft>>> struct alignas(void *) stFileAggregation
{
    std::unordered_map<_Ft, struct _sFm_> stack{};
    size_t size{};

    inline const struct _sFm_ get(const _Ft _i) noexcept
    {
        if (size > 0) [[likely]]
        {
            if (stack.find(_i) != stack.end())
            {
                return stack[_i];
            }
        }
        return {};
    };

    inline void propagate(const struct _sFm_ &_fsi) noexcept
    {
        if (++size > 0 && size < FS_MAX_COLLECTION_STACK_SIZE) [[likely]]
        {
            if (stack.find(_fsi.fname) == stack.end())
            {
                stack.insert({_fsi.fname, std::move(_fsi)});
            }
            return;
        }
        --size;
    };

    ~stFileAggregation(void) noexcept = default;
};


/*                          Class                        *\
\*-------------------------------------------------------*/
template <typename _fnT = String_t> class FSController
{

  private:
    bool _fc_new_ = false;
    struct stFileAggregation<_fnT> _fColl_;
    uint64_t _Fid_ = GenerateRandomId();

  public:
    explicit FSController() noexcept
    {
        if (!this->_fc_new_)
        {
            this->_fc_new_ = true;
        }
        std::cout << "ID: " << this->_Fid_ << "\n";
    };

    FSController(const FSController& _o) noexcept {
        if(*this != _o){
            this->_fColl_ = _o._fColl_;
            this->_fc_new_ = _o._fc_new_;
            this->_Fid_ = _o._Fid_;
        }
    };
    FSController &operator=(const FSController& _o) noexcept {
        if(*this != _o){
            this->_fColl_ = _o._fColl_;
            this->_fc_new_ = _o._fc_new_;
            this->_Fid_ = _o._Fid_;
        }
        return *this;
    };

    FSController(FSController&& _o) noexcept {
        if(*this != _o){
            this->_fColl_ = _o._fColl_;
            this->_fc_new_ = _o._fc_new_;
            this->_Fid_ = _o._Fid_;
        }
    };
    FSController &operator=(FSController&& _o) noexcept {
        if(*this != _o){
            this->_fColl_ = std::move(_o._fColl_);
            this->_fc_new_ = _o._fc_new_;
            this->_Fid_ = _o._Fid_;
            _o.~FSController();
        }
        return *this;
    };

    constexpr bool operator==(const FSController& _o) noexcept {
        return this->_Fid_ == _o._Fid_;
    };
    constexpr bool operator!=(const FSController& _o) noexcept {
        return !(this->_Fid_ == _o._Fid_);
    };


    inline static const bool FileExists(const StringView_t file_name) noexcept
    {
        if (file_name.empty() || file_name.size() > FS_MAX_FILE_NAME_LENGTH || file_name.find(" ") != std::string::npos) [[unlikely]]
            return false;

        return std::filesystem::exists(file_name.data());
    };

    inline static const bool FileExists(const StringSet_t file_list) noexcept
    {
        if (file_list.empty() && file_list.size() > 500000) [[unlikely]]
            return false;

        for (const String_t &fn : file_list)
            if (!FileExists(fn))
                return false;

        return true;
    };

    inline static const bool IsTextFile(const StringView_t target) noexcept
    {
        return std::filesystem::is_regular_file(target.data());
    };

    inline static const bool IsExecutable(const StringView_t target) noexcept
    {
        if (IsTextFile(target.data()))
        {
            std::filesystem::perms f_perms = std::filesystem::status(target.data()).permissions();
            if (((f_perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) || ((f_perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none) || ((f_perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none))
                return true;
        }
        return false;
    };

    inline static const bool IsSymlink(const StringView_t target) noexcept
    {
        return std::filesystem::is_symlink(target.data());
    };

    inline static const bool IsDirectory(const StringView_t target) noexcept
    {
        return std::filesystem::is_directory(target.data());
    };

    /**
     * @throw std::runtime_error
     */
    inline static const struct _sFm_ FileRead(const StringView_t _t, const bool _create_new = false)
    {
        struct _sFm_ fStat
        {
            .bytes{}, .fname{_t.data()}, .size
            {
                0
            }
        };
        Int_t fRead = open(_t.data(), O_RDONLY);
        if (fRead == -1)
            throw std::runtime_error("Error opening file");

        struct stat file_stat;

        if (fstat(fRead, &file_stat) == -1)
            throw std::runtime_error("Error getting file size");

        const off_t map_offset(0);

        fStat.size = file_stat.st_size;

        char *mappedData(static_cast<char*>(mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fRead, map_offset)));

        if (mappedData == MAP_FAILED)
            throw std::runtime_error("Error mapping file");

        fStat.bytes.assign(static_cast<char *>(mappedData), file_stat.st_size);

        if (munmap(mappedData, file_stat.st_size) == -1)
            throw std::runtime_error("Cannot unmap address!");

        close(fRead);
        return fStat;
    };

    /**
     * @throw std::runtime_error
     */
    inline static void FileWrite(const StringView_t _t, const StringView_t _b, const bool _create_new = false)
    {
        Int_t fWrite(open(_t.data(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR));
        if (fWrite == -1)
            throw std::runtime_error("cannot open file to write!");

        const off_t file_size(_b.length());

        if (ftruncate(fWrite, file_size) == -1)
            throw std::runtime_error("Cannot truncate file!");

        char *mapped_data(static_cast<char*>(mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fWrite, 0)));
        if (mapped_data == MAP_FAILED)
            throw std::runtime_error("error mapping file!");

        size_t data_size(_b.length());
        std::memcpy(mapped_data, _b.data(), data_size);

        if (munmap(mapped_data, file_size) == -1)
            throw std::runtime_error("Cannot unmap address!");

        close(fWrite);
    };

    inline void StackAggregate(const struct _sFm_ _fsi) noexcept
    {
        if (_fsi.size > 0) [[likely]]
            this->_fColl_.propagate(std::move(_fsi));
    };

    constexpr size_t &GetAggregationSize(void) noexcept
    {
        return this->_fColl_.size;
    };

    inline const struct _sFm_ StackLoad(const _fnT &_i) noexcept
    {
        return this->_fColl_.get(_i);
    };

    ~FSController() noexcept
    {
        if (this->_fc_new_) [[likely]]
            this->_fc_new_ = false;
    };
};
#endif
}; // namespace FSControllerModule

#endif
