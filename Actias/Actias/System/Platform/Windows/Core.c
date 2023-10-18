#include <Actias/System/Core.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>

Int32 ActiasGetNativeErrorCode(void)
{
    return GetLastError();
}

void ActiasGetSystemProperties(ActiasSystemProperties* pProperties)
{
    SYSTEM_INFO windowsInfo;
    GetSystemInfo(&windowsInfo);

    pProperties->PageSize       = windowsInfo.dwPageSize;
    pProperties->ProcessorCount = windowsInfo.dwNumberOfProcessors;
}
