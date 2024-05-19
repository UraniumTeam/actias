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

ActiasHandle ACTIAS_ABI ActiasOpen(const char* filename, ActiasFlags protection)
{
    USize result = open(filename, protection);
    if (result == -1)
    {
        return NULL;
    }

    return (ActiasHandle)result;
}

ActiasResult ACTIAS_ABI ActiasRead(ActiasHandle fileHandle, void* pBuffer, USize byteCount, USize* pBytesRead)
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

ActiasResult ACTIAS_ABI ActiasWrite(ActiasHandle fileHandle, const void* pBuffer, USize byteCount, USize* pBytesWritten)
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
