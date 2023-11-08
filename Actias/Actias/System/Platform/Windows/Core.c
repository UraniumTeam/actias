#include <Actias/System/Core.h>
#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <intrin.h>
#include <signal.h>

Int32 ACTIAS_ABI ActiasGetNativeErrorCode(void)
{
    return GetLastError();
}

#define ACTIAS_HANDLE_STD_SIGNAL(sig)                                                                                            \
    case ACTIAS_##sig:                                                                                                           \
        return raise(sig) == 0 ? ACTIAS_SUCCESS : ACTIAS_FAIL_UNKNOWN

ActiasResult ACTIAS_ABI ActiasRaiseSignal(Int32 signal)
{
    switch (signal)
    {
        ACTIAS_HANDLE_STD_SIGNAL(SIGABRT);
        ACTIAS_HANDLE_STD_SIGNAL(SIGFPE);
        ACTIAS_HANDLE_STD_SIGNAL(SIGILL);
        ACTIAS_HANDLE_STD_SIGNAL(SIGINT);
        ACTIAS_HANDLE_STD_SIGNAL(SIGSEGV);
        ACTIAS_HANDLE_STD_SIGNAL(SIGTERM);
    case ACTIAS_SIGTRAP:
    default:
        __debugbreak();
        return ACTIAS_SUCCESS;
    }
}

#undef ACTIAS_HANDLE_STD_SIGNAL

void ACTIAS_ABI ActiasGetSystemProperties(ActiasSystemProperties* pProperties)
{
    SYSTEM_INFO windowsInfo;
    GetSystemInfo(&windowsInfo);

    pProperties->PageSize       = windowsInfo.dwPageSize;
    pProperties->ProcessorCount = windowsInfo.dwNumberOfProcessors;
}
