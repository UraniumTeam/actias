#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Streams.h>

inline DWORD ACTIAS_ABI ActiasConvertStandardDescriptor(ActiasStandardDescriptor descriptor)
{
    switch (descriptor)
    {
    case ACTIAS_STDIN:
        return STD_INPUT_HANDLE;
    case ACTIAS_STDOUT:
        return STD_OUTPUT_HANDLE;
    case ACTIAS_STDERR:
        return STD_ERROR_HANDLE;
    default:
        return 0;
    }
}

ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(ActiasStandardDescriptor descriptor, ActiasHandle* pHandle)
{
    DWORD winStdHandle = ActiasConvertStandardDescriptor(descriptor);

    if (winStdHandle == 0)
    {
        return ACTIAS_FAIL_INVALID_STD_DESCRIPTOR;
    }

    *pHandle = GetStdHandle(winStdHandle);

    if (*pHandle == INVALID_HANDLE_VALUE)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasWrite(ActiasHandle fileHandle, const void* pBuffer, USize byteCount, USize* pBytesWritten)
{
    DWORD bytesWritten;
    BOOL result = WriteFile(fileHandle, pBuffer, (DWORD)byteCount, &bytesWritten, NULL);

    if (result == FALSE)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    if (pBytesWritten)
    {
        *pBytesWritten = (USize)bytesWritten;
    }

    return ACTIAS_SUCCESS;
}
