#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Streams.h>

inline DWORD ActiasConvertStandardDescriptor(ActiasStandardDescriptor descriptor)
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

ActiasResult ActiasGetStdFileHandle(ActiasStandardDescriptor descriptor, ActiasHandle* pHandle)
{
    DWORD winStdHandle = ActiasConvertStandardDescriptor(descriptor);
    *pHandle           = GetStdHandle(winStdHandle);

    if (*pHandle == INVALID_HANDLE_VALUE)
    {
        // TODO: maybe we should return our own result codes, from our own validation logic;
        //   or we can convert native OS codes to ours (but there are a lot...);
        //   or we can create something like ActiasGetLastError() and combine with the first approach
        return GetLastError();
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ActiasWrite(ActiasHandle fileHandle, ACTIAS_CONST void* pBuffer, USize byteCount, USize* pBytesWritten)
{
    DWORD bytesWritten;
    BOOL result = WriteFile(fileHandle, pBuffer, (DWORD)byteCount, &bytesWritten, NULL);

    if (!result)
    {
        return GetLastError();
    }

    if (pBytesWritten)
    {
        *pBytesWritten = (USize)bytesWritten;
    }

    return ACTIAS_SUCCESS;
}
