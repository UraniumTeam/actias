#pragma once
#include <Actias/System/CompilerTraits.h>
#include <Actias/System/Memory.h>
#include <Actias/System/Time.h>

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

#define ACTIAS_WIN_TICKS_PER_SECOND 10000000
#define ACTIAS_WIN_UNIX_EPOCH_DIFFERENCE 11644473600

inline static ActiasTime ActiasConvertWindowsTickToUnixSeconds(Int64 windowsTicks)
{
    return windowsTicks / ACTIAS_WIN_TICKS_PER_SECOND - ACTIAS_WIN_UNIX_EPOCH_DIFFERENCE;
}

inline static ActiasTime ActiasConvertFiletimeToUnixSeconds(FILETIME fileTime)
{
    LARGE_INTEGER ftInt;
    ftInt.HighPart = fileTime.dwHighDateTime;
    ftInt.LowPart  = fileTime.dwLowDateTime;
    return ActiasConvertWindowsTickToUnixSeconds((Int64)ftInt.QuadPart);
}

inline static void ActiasConvertDateTimeInfoToSystemTime(const ActiasDateTimeInfo* pDateTime, SYSTEMTIME* pResult)
{
    pResult->wYear         = pDateTime->Year + 1900;
    pResult->wMonth        = pDateTime->Month;
    pResult->wDay          = pDateTime->Day;
    pResult->wDayOfWeek    = pDateTime->DayOfWeek;
    pResult->wHour         = pDateTime->Hour;
    pResult->wMinute       = pDateTime->Minute;
    pResult->wSecond       = pDateTime->Second;
    pResult->wMilliseconds = 0;
}

inline static void ActiasConvertSystemTimeToDateTimeInfo(const SYSTEMTIME* pSystemTime, ActiasDateTimeInfo* pResult)
{
    pResult->Year      = pSystemTime->wYear - 1900;
    pResult->Month     = pSystemTime->wMonth;
    pResult->Day       = pSystemTime->wDay;
    pResult->DayOfWeek = pSystemTime->wDayOfWeek;
    pResult->Hour      = pSystemTime->wHour;
    pResult->Minute    = pSystemTime->wMinute;
    pResult->Second    = pSystemTime->wSecond;
}

#define ACTIAS_MB_FLAGS MB_ERR_INVALID_CHARS

inline static LPWSTR ActiasConvertUTF8ToWide(const char* pString, USize length)
{
    int wideCount = MultiByteToWideChar(CP_UTF8, ACTIAS_MB_FLAGS, pString, (int)length, NULL, 0);
    if (wideCount <= 0)
    {
        return NULL;
    }

    LPWSTR pResult     = (LPWSTR)ActiasAlloc(((USize)wideCount + 1) * sizeof(WCHAR));
    pResult[wideCount] = L'\0';

    if (MultiByteToWideChar(CP_UTF8, ACTIAS_MB_FLAGS, pString, (int)length, pResult, wideCount) <= 0)
    {
        ActiasFree(pResult);
        return NULL;
    }

    return pResult;
}

ACTIAS_END_C
