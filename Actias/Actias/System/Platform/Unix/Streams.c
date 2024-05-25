#include <Actias/System/Platform/Unix/linux_syscall_support.h>
#include <Actias/System/Streams.h>

ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(Int32 descriptor, ActiasHandle* pHandle)
{
    if (descriptor < ACTIAS_STD_DESCRIPTOR_MIN || descriptor > ACTIAS_STD_DESCRIPTOR_MAX)
    {
        return ACTIAS_FAIL_INVALID_STD_DESCRIPTOR;
    }

    *pHandle = (ActiasHandle)((USize)descriptor);
    return ACTIAS_SUCCESS;
}

inline USize ACTIAS_ABI ActiasConvertFileOpenFlags(ActiasFlags flags)
{
    switch (flags)
    {
    case ACTIAS_FILE_OPEN_NONE:
        return 0;
    case ACTIAS_FILE_OPEN_READ_ONLY:
        return O_RDONLY;
    case ACTIAS_FILE_OPEN_WRITE_ONLY:
        return O_WRONLY;
    case ACTIAS_FILE_OPEN_APPEND:
        return O_APPEND;
    case ACTIAS_FILE_OPEN_CREATE:
        return O_CREAT;
    case ACTIAS_FILE_OPEN_CREATE_NEW:
        return O_CREAT; //!
    case ACTIAS_FILE_OPEN_TRUNCATE:
        return O_TRUNC;
    case ACTIAS_FILE_OPEN_READ_WRITE:
        return O_RDWR;
    default:
        return 0;
    }
}

ActiasResult ACTIAS_ABI ActiasOpenFile(const char* filename, ActiasFlags protection, ActiasHandle* pHandle)
{
    USize result = sys_open(filename, ActiasConvertFileOpenFlags(protection), 0);
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

ActiasResult ACTIAS_ABI ActiasSeekFile(ActiasHandle fileHandle, USize offset, ActiasFileSeekModeBits seekMode)
{
    USize result = lseek((int)((USize)fileHandle), offset, seekMode);
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasTellFile(ActiasHandle fileHandle, USize* position)
{
    USize result = lseek((int)((USize)fileHandle), 0, SEEK_CUR);
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *position = result;

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasFlushFile(ActiasHandle fileHandle)
{
    USize result = fsync((int)((USize)fileHandle));
    if (result == -1)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}
