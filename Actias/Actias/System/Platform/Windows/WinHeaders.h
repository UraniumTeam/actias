#pragma once
#include <Actias/System/CompilerTraits.h>
#include <Actias/System/Memory.h>

#ifndef ACTIAS_WINDOWS
#    error This header must be used in Windows specific code only
#endif

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

ACTIAS_PUSH_MSVC_WARNING(5105)
#include <Windows.h>
ACTIAS_POP_MSVC_WARNING

ACTIAS_BEGIN_C

ACTIAS_FORCE_INLINE static ActiasResult ActiasCloseHandle(HANDLE handle)
{
    if (CloseHandle(handle))
    {
        return ACTIAS_SUCCESS;
    }

    return ACTIAS_FAIL_UNKNOWN;
}

#define ACTIAS_MB_FLAGS MB_ERR_INVALID_CHARS

inline static LPWSTR ActiasConvertUTF8ToWide(const char* pString, USize length)
{
    int wideCount = MultiByteToWideChar(CP_UTF8, ACTIAS_MB_FLAGS, pString, (int)length, NULL, 0);
    if (wideCount <= 0)
    {
        return NULL;
    }

    LPWSTR pResult = (LPWSTR)ActiasAlloc(((USize)wideCount + 1) * sizeof(WCHAR));

    if (MultiByteToWideChar(CP_UTF8, ACTIAS_MB_FLAGS, pString, (int)length, pResult, wideCount) <= 0)
    {
        ActiasFree(pResult);
        return NULL;
    }

    return pResult;
}

ACTIAS_END_C
