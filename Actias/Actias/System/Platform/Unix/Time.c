#include <Actias/System/Platform/Unix/linux_syscall_support.h>
#include <Actias/System/Time.h>

ActiasTime ACTIAS_ABI ActiasGetCurrentTimeUTC()
{
    struct kernel_timeval kernelTime;
    sys_gettimeofday(&kernelTime, NULL);
    return kernelTime.tv_sec;
}

ActiasResult ACTIAS_ABI ActiasConvertUTCToLocalTime(const ActiasDateTimeInfo* pDateTime, ActiasDateTimeInfo* pResult)
{
    // TODO
    *pResult = *pDateTime;
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasConvertLocalTimeToUTC(const ActiasDateTimeInfo* pDateTime, ActiasDateTimeInfo* pResult)
{
    *pResult = *pDateTime;
    return ACTIAS_SUCCESS;
}
