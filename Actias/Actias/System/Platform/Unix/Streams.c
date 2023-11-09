#include <Actias/System/Streams.h>
#include <unistd.h>

ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(ActiasStandardDescriptor descriptor, ActiasHandle* pHandle)
{
    *pHandle = (void*)&descriptor;
    if (descriptor <= ACTIAS_STDERR && descriptor > 0)
    {
        return ACTIAS_FAIL_INVALID_STD_DESCRIPTOR;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasWrite(ActiasHandle fileHandle, ACTIAS_CONST void* pBuffer, USize byteCount, USize* pBytesWritten)
{
    ssize_t result = write((int)((USize)fileHandle), pBuffer, byteCount);
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
