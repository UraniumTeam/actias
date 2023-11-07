#include <Actias/System/Core.h>
#include <sys/sysinfo.h>
#include <errno.h>

Int32 ACTIAS_ABI ActiasGetNativeErrorCode(void)
{
    return errno;
}

void ACTIAS_ABI ActiasGetSystemProperties(ActiasSystemProperties* pProperties)
{
    pProperties->PageSize       = 4 * 1024;
    pProperties->ProcessorCount = get_nprocs_conf();
}
