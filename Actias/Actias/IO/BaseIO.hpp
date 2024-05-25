#pragma once
#include <Actias/Strings/StringSlice.hpp>
#include <Actias/System/Streams.h>
#include <Actias/Utils/Result.hpp>

namespace Actias::IO
{
    //! \brief Result code of an I/O operation.
    enum class ResultCode : ActiasResult
    {
        Success = ACTIAS_SUCCESS, //!< Operation succeeded.

        PermissionDenied  = ACTIAS_FAIL_PERMISSION_DENIED,    //!< Permission denied.
        NoFileOrDirectory = ACTIAS_FAIL_NO_FILE_OR_DIRECTORY, //!< No such file or directory.
        FileExists        = ACTIAS_FAIL_FILE_EXISTS,          //!< File already exists.
        FileTooLarge      = ACTIAS_FAIL_FILE_TOO_LARGE,       //!< File is too large.
        FilenameTooLong   = ACTIAS_FAIL_FILENAME_TOO_LONG,    //!< Filename is too long.
        NotDirectory      = ACTIAS_FAIL_NOT_DIRECTORY,        //!< Not a directory.
        IsDirectory       = ACTIAS_FAIL_IS_DIRECTORY,         //!< Is a directory.
        DirectoryNotEmpty = ACTIAS_FAIL_DIRECTORY_NOT_EMPTY,  //!< Directory is not empty.
        TooManyOpenFiles  = ACTIAS_FAIL_TOO_MANY_OPEN_FILES,  //!< Too many files are open.
        InvalidSeek       = ACTIAS_FAIL_INVALID_SEEK,         //!< Invalid seek operation.
        IOError           = ACTIAS_FAIL_IO_ERROR,             //!< IO error.
        DeadLock          = ACTIAS_FAIL_DEAD_LOCK,            //!< Resource deadlock would occur.
        UnknownError      = ACTIAS_FAIL_UNKNOWN,              //!< Unknown error.

        ReadNotAllowed  = ACTIAS_FAIL_READ_NOT_ALLOWED,  //!< Read operation is not allowed.
        WriteNotAllowed = ACTIAS_FAIL_WRITE_NOT_ALLOWED, //!< Write operation is not allowed.
        NotSupported    = ACTIAS_FAIL_NOT_SUPPORTED,     //!< Operation is not supported.
        NotOpen         = ACTIAS_FAIL_NOT_OPEN           //!< File or stream is not open.
    };

    //! \brief Get result code description.
    StringSlice GetResultDesc(ResultCode code);

    //! \brief File or stream open mode.
    enum class OpenMode
    {
        None      = ACTIAS_FILE_OPEN_MODE_NONE,       //!< None mode.
        ReadOnly  = ACTIAS_FILE_OPEN_MODE_READ_ONLY,  //!< Read only open mode.
        WriteOnly = ACTIAS_FILE_OPEN_MODE_WRITE_ONLY, //!< Write only open mode.
        Append    = ACTIAS_FILE_OPEN_MODE_APPEND,     //!< Append open mode.
        Create    = ACTIAS_FILE_OPEN_MODE_CREATE,     //!< Create (new or rewrite existing) open mode.
        CreateNew = ACTIAS_FILE_OPEN_MODE_CREATE_NEW, //!< Create (new only) open mode.
        Truncate  = ACTIAS_FILE_OPEN_MODE_TRUNCATE,   //!< Truncate open mode.
        ReadWrite = ACTIAS_FILE_OPEN_MODE_READ_WRITE, //!< Read and write open mode.
    };

    //! \brief Check if writing is allowed with the specified open mode.
    //!
    //! \param mode - The open mode to check.
    inline bool IsWriteAllowed(OpenMode mode)
    {
        switch (mode)
        {
        case OpenMode::WriteOnly:
        case OpenMode::Append:
        case OpenMode::Create:
        case OpenMode::CreateNew:
        case OpenMode::Truncate:
        case OpenMode::ReadWrite:
            return true;
        default:
            return false;
        }
    }

    //! \brief Check if reading is allowed with the specified open mode.
    //!
    //! \param mode - The open mode to check.
    inline bool IsReadAllowed(OpenMode mode)
    {
        switch (mode)
        {
        case OpenMode::ReadOnly:
        case OpenMode::ReadWrite:
            return true;
        default:
            return false;
        }
    }

    //! \brief Seek mode.
    enum class SeekMode
    {
        Begin   = ACTIAS_FILE_SEEK_MODE_BEGIN,   //!< Begin seek mode.
        Current = ACTIAS_FILE_SEEK_MODE_CURRENT, //!< Current seek mode
        End     = ACTIAS_FILE_SEEK_MODE_END,     //!< End seek mode.
    };
} // namespace Actias::IO
