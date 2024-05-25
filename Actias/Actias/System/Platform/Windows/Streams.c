#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Streams.h>

inline static ActiasResult ACTIAS_ABI ActiasConvertWin32Error(DWORD error)
{
    switch (error)
    {
    case ERROR_ALREADY_EXISTS:
    case ERROR_FILE_EXISTS:
        return ACTIAS_FAIL_FILE_EXISTS;
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        return ACTIAS_FAIL_NO_FILE_OR_DIRECTORY;
    case ERROR_ACCESS_DENIED:
        return ACTIAS_FAIL_PERMISSION_DENIED;
    case ERROR_SHARING_VIOLATION:
    case ERROR_INVALID_PARAMETER:
        return ACTIAS_FAIL_INVALID_ARGUMENT;
    case ERROR_FILE_TOO_LARGE:
        return ACTIAS_FAIL_FILE_TOO_LARGE;
    case ERROR_TOO_MANY_OPEN_FILES:
        return ACTIAS_FAIL_TOO_MANY_OPEN_FILES;
    case ERROR_SEEK:
        return ACTIAS_FAIL_INVALID_SEEK;
    case ERROR_NOT_SUPPORTED:
        return ACTIAS_FAIL_NOT_SUPPORTED;
    default:
        return ACTIAS_FAIL_UNKNOWN;
    }
}

inline static DWORD ACTIAS_ABI ActiasConvertStandardDescriptor(Int32 descriptor)
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

inline static DWORD ACTIAS_ABI ActiasGetFileAccessFlags(ActiasFileOpenMode openMode)
{
    switch (openMode)
    {
    case ACTIAS_FILE_OPEN_MODE_NONE:
        return 0;
    case ACTIAS_FILE_OPEN_MODE_READ_ONLY:
        return GENERIC_READ;
    case ACTIAS_FILE_OPEN_MODE_WRITE_ONLY:
    case ACTIAS_FILE_OPEN_MODE_CREATE:
    case ACTIAS_FILE_OPEN_MODE_CREATE_NEW:
        return GENERIC_WRITE;
    case ACTIAS_FILE_OPEN_MODE_APPEND:
    case ACTIAS_FILE_OPEN_MODE_TRUNCATE:
    case ACTIAS_FILE_OPEN_MODE_READ_WRITE:
        return GENERIC_READ | GENERIC_WRITE;
    default:
        return 0;
    }
}

inline static DWORD ACTIAS_ABI ActiasGetFileShareMode(ActiasFileOpenMode openMode)
{
    if (openMode == ACTIAS_FILE_OPEN_MODE_READ_ONLY)
    {
        return FILE_SHARE_READ;
    }

    return 0;
}

inline static DWORD ACTIAS_ABI ActiasGetFileCreationDisposition(ActiasFileOpenMode openMode)
{
    switch (openMode)
    {
    case ACTIAS_FILE_OPEN_MODE_NONE:
        return 0;
    case ACTIAS_FILE_OPEN_MODE_READ_ONLY:
    case ACTIAS_FILE_OPEN_MODE_WRITE_ONLY:
    case ACTIAS_FILE_OPEN_MODE_READ_WRITE:
    case ACTIAS_FILE_OPEN_MODE_APPEND:
        return OPEN_EXISTING;
    case ACTIAS_FILE_OPEN_MODE_CREATE:
        return CREATE_ALWAYS;
    case ACTIAS_FILE_OPEN_MODE_CREATE_NEW:
        return CREATE_NEW;
    case ACTIAS_FILE_OPEN_MODE_TRUNCATE:
        return TRUNCATE_EXISTING;
    default:
        return 0;
    }
}

inline static DWORD ACTIAS_ABI ActiasConvertFileSeekMode(ActiasFileSeekMode seekMode)
{
    switch (seekMode)
    {
    case ACTIAS_FILE_SEEK_MODE_BEGIN:
        return FILE_BEGIN;
    case ACTIAS_FILE_SEEK_MODE_CURRENT:
        return FILE_CURRENT;
    case ACTIAS_FILE_SEEK_MODE_END:
        return FILE_END;
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

ActiasResult ACTIAS_ABI ActiasOpenFile(const char* filePath, ActiasFileOpenMode openMode, ActiasHandle* pHandle)
{
    USize pathLength = strlen(filePath);
    int wideLength   = MultiByteToWideChar(CP_UTF8, 0, filePath, (int)pathLength, NULL, 0);
    if (wideLength < 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    LPWSTR widePath = ACTIAS_StackAlloc(WCHAR, wideLength + 1);
    if (MultiByteToWideChar(CP_UTF8, 0, filePath, (int)pathLength, widePath, wideLength) < 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    widePath[wideLength] = 0;

    DWORD access   = ActiasGetFileAccessFlags(openMode);
    DWORD share    = ActiasGetFileShareMode(openMode);
    DWORD creation = ActiasGetFileCreationDisposition(openMode);

    HANDLE hFile = CreateFileW(widePath, access, share, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    *pHandle = (ActiasHandle)hFile;
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasCloseFile(ActiasHandle fileHandle)
{
    if (!CloseHandle((HANDLE)fileHandle))
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasReadFile(ActiasHandle fileHandle, void* pBuffer, USize byteCount, USize* pBytesRead)
{
    DWORD bytesRead;
    if (!ReadFile((HANDLE)fileHandle, pBuffer, (DWORD)byteCount, &bytesRead, NULL))
    {
        return ActiasConvertWin32Error(GetLastError());
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
    if (!WriteFile((HANDLE)fileHandle, pBuffer, (DWORD)byteCount, &bytesWritten, NULL))
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    if (pBytesWritten)
    {
        *pBytesWritten = (USize)bytesWritten;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasSeekFile(ActiasHandle fileHandle, USize offset, ActiasFileSeekMode seekMode)
{
    LARGE_INTEGER distance = { .QuadPart = offset };
    if (!SetFilePointerEx(fileHandle, distance, NULL, ActiasConvertFileSeekMode(seekMode)))
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasTellFile(ActiasHandle fileHandle, USize* position)
{
    LARGE_INTEGER distance = { 0 };
    if (!SetFilePointerEx(fileHandle, distance, (PLARGE_INTEGER)position, FILE_CURRENT))
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasGetFileStats(ActiasHandle fileHandle, ActiasFileStats* pResult)
{
    HANDLE hFile = (HANDLE)fileHandle;
    FILETIME creationFT, accessFT, writeFT;
    if (!GetFileTime(hFile, &creationFT, &accessFT, &writeFT))
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize))
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    pResult->CreationTimeUTC         = ActiasConvertFiletimeToUnixSeconds(creationFT);
    pResult->LastAccessTimeUTC       = ActiasConvertFiletimeToUnixSeconds(accessFT);
    pResult->LastModificationTimeUTC = ActiasConvertFiletimeToUnixSeconds(writeFT);
    pResult->ByteSize                = (UInt64)fileSize.QuadPart;

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasRemoveFile(const char* filePath)
{
    USize pathLength = strlen(filePath);
    int wideLength   = MultiByteToWideChar(CP_UTF8, 0, filePath, (int)pathLength, NULL, 0);
    if (wideLength < 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    LPWSTR widePath = ACTIAS_StackAlloc(WCHAR, wideLength + 1);
    if (MultiByteToWideChar(CP_UTF8, 0, filePath, (int)pathLength, widePath, wideLength) < 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    if (!DeleteFileW(widePath))
    {
        return ActiasConvertWin32Error(GetLastError());
    }

    return ACTIAS_SUCCESS;
}
