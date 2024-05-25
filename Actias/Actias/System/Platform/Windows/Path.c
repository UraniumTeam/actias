#include <Actias/System/Path.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>

ActiasResult ACTIAS_ABI ActiasGetCurrentDirectory(char* pBuffer, USize bufferByteSize)
{
    LPWSTR wideBuffer = (LPWSTR)alloca(bufferByteSize * sizeof(WCHAR));
    DWORD wideLength  = GetCurrentDirectoryW((DWORD)bufferByteSize, wideBuffer);

    int unicodeLength = WideCharToMultiByte(CP_UTF8, 0, wideBuffer, (int)wideLength, NULL, 0, NULL, NULL);
    if (unicodeLength < 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    if (unicodeLength >= bufferByteSize - 1)
    {
        return ACTIAS_FAIL_INSUFFICIENT_SIZE;
    }

    if (WideCharToMultiByte(CP_UTF8, 0, wideBuffer, (int)wideLength, pBuffer, unicodeLength, NULL, NULL) < 0)
    {
        return ACTIAS_FAIL_INVALID_ENCODING;
    }

    pBuffer[unicodeLength] = 0;
    return ACTIAS_SUCCESS;
}
