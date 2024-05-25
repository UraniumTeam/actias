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

inline UINT ACTIAS_ABI ActiasConvertFileOpenFlags(ActiasFlags flags)
{
    switch (flags)
    {
    case ACTIAS_FILE_OPEN_NONE:
        return 0;
    case ACTIAS_FILE_OPEN_READ_ONLY:
        return OF_READ;
    case ACTIAS_FILE_OPEN_WRITE_ONLY:
        return OF_WRITE;
    case ACTIAS_FILE_OPEN_APPEND:
        return OF_WRITE; //!
    case ACTIAS_FILE_OPEN_CREATE:
        return OF_CREATE;
    case ACTIAS_FILE_OPEN_CREATE_NEW:
        return OF_CREATE; //!
    case ACTIAS_FILE_OPEN_TRUNCATE:
        return OF_CREATE; //!
    case ACTIAS_FILE_OPEN_READ_WRITE:
        return OF_READWRITE;
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

ActiasResult ACTIAS_ABI ActiasOpenFile(const char* filename, ActiasFlags protection, ActiasHandle* fileHandle)
{
    OFSTRUCT fileInfo = { 0 };
    HFILE handle      = OpenFile(filename, &fileInfo, ActiasConvertFileOpenFlags(protection));
    if (handle == HFILE_ERROR)
    {
        *fileHandle = NULL;
        return ACTIAS_FAIL_UNKNOWN;
    }

    if (protection == ACTIAS_FILE_OPEN_APPEND)
    {
        ActiasSeekFile((ActiasHandle)((USize)handle), 0, ACTIAS_FILE_SEEKMODE_END);
    }

    /*
    if(ACTIAS_FILE_OPEN_CREATE_NEW)
    if(ACTIAS_FILE_OPEN_TRUNCATE) 
    */

    *fileHandle = (ActiasHandle)((USize)handle);

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasCloseFile(ActiasHandle fileHandle)
{
    BOOL result = CloseHandle(fileHandle);
    if (result == 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasReadFile(ActiasHandle fileHandle, void* pBuffer, USize byteCount, USize* pBytesRead)
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

ActiasResult ACTIAS_ABI ActiasWriteFile(ActiasHandle fileHandle, const void* pBuffer, USize byteCount, USize* pBytesWritten)
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

ActiasResult ACTIAS_ABI ActiasSeekFile(ActiasHandle fileHandle, USize offset, ActiasFileSeekModeBits seekMode)
{
    DWORD result = SetFilePointer(fileHandle, (LONG)offset, NULL, seekMode);
    if (result == INVALID_SET_FILE_POINTER)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasTellFile(ActiasHandle fileHandle, USize* position)
{
    const LONG zeroOffset = { 0 };
    SetFilePointer(fileHandle, zeroOffset, (PLONG)&position, FILE_CURRENT);
    if (&position == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasFlushFile(ActiasHandle fileHandle)
{
    BOOL result = FlushFileBuffers(fileHandle);
    if (result == 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}
