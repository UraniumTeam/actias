#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Time.h>

ActiasTime ACTIAS_ABI ActiasGetCurrentTimeUTC()
{
    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);
    return ActiasConvertFiletimeToUnixSeconds(fileTime);
}

ActiasResult ACTIAS_ABI ActiasConvertUTCToLocalTime(const ActiasDateTimeInfo* pDateTime, ActiasDateTimeInfo* pResult)
{
    SYSTEMTIME utcTime, localTime;
    ActiasConvertDateTimeInfoToSystemTime(pDateTime, &utcTime);
    if (!SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &localTime))
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    ActiasConvertSystemTimeToDateTimeInfo(&localTime, pResult);
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasConvertLocalTimeToUTC(const ActiasDateTimeInfo* pDateTime, ActiasDateTimeInfo* pResult)
{
    SYSTEMTIME localTime, utcTime;
    ActiasConvertDateTimeInfoToSystemTime(pDateTime, &localTime);
    if (!TzSpecificLocalTimeToSystemTime(NULL, &localTime, &utcTime))
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    ActiasConvertSystemTimeToDateTimeInfo(&utcTime, pResult);
    return ACTIAS_SUCCESS;
}
