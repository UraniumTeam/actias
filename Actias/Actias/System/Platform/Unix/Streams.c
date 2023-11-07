#include <Actias/System/Streams.h>

ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(ActiasStandardDescriptor descriptor, ActiasHandle* pHandle)
{ 
    *pHandle = (void*)&descriptor;
    if (*pHandle == null)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasWrite(ActiasHandle fileHandle, ACTIAS_CONST void* pBuffer, USize byteCount, USize* pBytesWritten)
{
    write((int)((USize)fileHandle), *pBuffer, byteCount);
    if (errno != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }
    return ACTIAS_SUCCESS;
}