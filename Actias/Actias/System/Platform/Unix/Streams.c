#include <Actias/System/Platform/Unix/linux_syscall_support.h>
#include <Actias/System/Streams.h>

inline static USize ACTIAS_ABI ActiasConvertFileOpenFlags(ActiasFlags flags)
{
    switch (flags)
    {
    case ACTIAS_FILE_OPEN_MODE_NONE:
        return 0;
    case ACTIAS_FILE_OPEN_MODE_READ_ONLY:
        return O_RDONLY;
    case ACTIAS_FILE_OPEN_MODE_WRITE_ONLY:
        return O_WRONLY;
    case ACTIAS_FILE_OPEN_MODE_APPEND:
        return O_APPEND;
    case ACTIAS_FILE_OPEN_MODE_CREATE:
        return O_CREAT;
    case ACTIAS_FILE_OPEN_MODE_CREATE_NEW:
        return O_CREAT; //!
    case ACTIAS_FILE_OPEN_MODE_TRUNCATE:
        return O_TRUNC;
    case ACTIAS_FILE_OPEN_MODE_READ_WRITE:
        return O_RDWR;
    default:
        return 0;
    }
}

ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(Int32 descriptor, ActiasHandle* pHandle)
{
    if (descriptor < ACTIAS_STD_DESCRIPTOR_MIN || descriptor > ACTIAS_STD_DESCRIPTOR_MAX)
    {
        return ACTIAS_FAIL_INVALID_STD_DESCRIPTOR;
    }

    *pHandle = (ActiasHandle)((USize)descriptor);
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasOpenFile(const char* filePath, ActiasFileOpenMode openMode, ActiasHandle* pHandle)
{
    USize result = sys_open(filePath, ActiasConvertFileOpenFlags(openMode), 0);
    if (result == -1)
    {
        *pHandle = NULL;
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pHandle = (ActiasHandle)result;

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasCloseFile(ActiasHandle fileHandle)
{
    USize result = sys_close((int)((USize)fileHandle));
    if (result != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasReadFile(ActiasHandle fileHandle, void* pBuffer, USize byteCount, USize* pBytesRead)
{
    ssize_t result = sys_read((int)((USize)fileHandle), pBuffer, byteCount);
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    if (pBytesRead)
    {
        *pBytesRead = (USize)result;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasWriteFile(ActiasHandle fileHandle, const void* pBuffer, USize byteCount, USize* pBytesWritten)
{
    ssize_t result = sys_write((int)((USize)fileHandle), pBuffer, byteCount);
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    if (pBytesWritten)
    {
        *pBytesWritten = (USize)result;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasSeekFile(ActiasHandle fileHandle, USize offset, ActiasFileSeekMode seekMode)
{
    ssize_t result = sys_lseek((int)((USize)fileHandle), offset, seekMode);
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasTellFile(ActiasHandle fileHandle, USize* pPosition)
{
    ssize_t result = sys_lseek((int)((USize)fileHandle), 0, SEEK_CUR);
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pPosition = (USize)result;
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasGetFileStats(ActiasHandle fileHandle, ActiasFileStats* pResult)
{
    struct kernel_stat kernelStat;
    if (sys_fstat((int)((USize)fileHandle), &kernelStat) == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    pResult->ByteSize = (USize)kernelStat.st_size;

    // Time of last status change since we don't have a better option here.
    pResult->CreationTimeUTC         = kernelStat.st_ctime_;
    pResult->LastModificationTimeUTC = kernelStat.st_mtime_;
    pResult->LastAccessTimeUTC       = kernelStat.st_atime_;
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasRemoveFile(const char* filePath)
{
    // TODO
    return ACTIAS_FAIL_NOT_SUPPORTED;
}
