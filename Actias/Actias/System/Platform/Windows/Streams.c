#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Streams.h>

inline DWORD ACTIAS_ABI ActiasConvertStandardDescriptor(Int32 descriptor)
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

ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(Int32 descriptor, ActiasHandle* pHandle)
{
    DWORD winStdHandle = ActiasConvertStandardDescriptor(descriptor);

    if (winStdHandle == 0)
    {
        return ACTIAS_FAIL_INVALID_STD_DESCRIPTOR;
    }

    HANDLE handle = GetStdHandle(winStdHandle);

    if (*pHandle == INVALID_HANDLE_VALUE)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pHandle = (ActiasHandle)handle;

    return ACTIAS_SUCCESS;
}

ActiasHandle ACTIAS_ABI ActiasOpen(const char* filename, ActiasFlags protection)
{
    OFSTRUCT fileInfo = { 0 };
    HFILE handle      = OpenFile(filename, &fileInfo, protection);
    if (handle == HFILE_ERROR)
    {
        return NULL;
    }

    return (ActiasHandle)(handle);
}

ActiasResult ACTIAS_ABI ActiasRead(ActiasHandle fileHandle, void* pBuffer, USize byteCount, USize* pBytesRead)
{
    DWORD bytesRead;
    BOOL result = ReadFile((HANDLE)fileHandle, pBuffer, (DWORD)byteCount, &bytesRead, NULL);

    if (result == FALSE)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    if (pBytesRead)
    {
        *pBytesRead = (USize)bytesRead;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasWrite(ActiasHandle fileHandle, const void* pBuffer, USize byteCount, USize* pBytesWritten)
{
    DWORD bytesWritten;
    BOOL result = WriteFile((HANDLE)fileHandle, pBuffer, (DWORD)byteCount, &bytesWritten, NULL);

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
