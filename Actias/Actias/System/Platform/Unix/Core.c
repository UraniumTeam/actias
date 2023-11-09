#include <Actias/System/Core.h>
#include <errno.h>
#include <signal.h>
#include <sys/sysinfo.h>

Int32 ACTIAS_ABI ActiasGetNativeErrorCode(void)
{
    return errno;
}

#define ACTIAS_HANDLE_STD_SIGNAL(sig)                                                                                            \
    case ACTIAS_##sig:                                                                                                           \
        return raise(sig) == 0 ? ACTIAS_SUCCESS : ACTIAS_FAIL_UNKNOWN

ActiasResult ACTIAS_ABI ActiasRaiseSignal(Int32 signal)
{
    switch (signal)
    {
    default:
        ACTIAS_HANDLE_STD_SIGNAL(SIGTRAP);
        ACTIAS_HANDLE_STD_SIGNAL(SIGABRT);
        ACTIAS_HANDLE_STD_SIGNAL(SIGFPE);
        ACTIAS_HANDLE_STD_SIGNAL(SIGILL);
        ACTIAS_HANDLE_STD_SIGNAL(SIGINT);
        ACTIAS_HANDLE_STD_SIGNAL(SIGSEGV);
        ACTIAS_HANDLE_STD_SIGNAL(SIGTERM);
    }
}

#undef ACTIAS_HANDLE_STD_SIGNAL

void ACTIAS_ABI ActiasGetSystemProperties(ActiasSystemProperties* pProperties)
{
    pProperties->PageSize       = 4 * 1024;
    pProperties->ProcessorCount = get_nprocs_conf();
}
