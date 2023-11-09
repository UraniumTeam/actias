#include <Actias/System/Runtime.h>
#include <dlfcn.h>

ActiasResult ACTIAS_ABI ActiasLoadNativeModule(const char* pFilePath, ActiasHandle* pHandle)
{
    void* handle = dlopen(pFilePath, RTLD_LAZY);
    if (handle == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pHandle = handle;
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasUnloadNativeModule(ActiasHandle moduleHandle)
{
    int result = dlclose(moduleHandle);
    if (result != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasFindNativeSymbolAddress(ActiasHandle moduleHandle, const char* pSymbolName, ActiasProc* pAddress)
{
    void* result = dlsym(moduleHandle, pSymbolName);
    if (result == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pAddress = (ActiasProc)result;
    return ACTIAS_SUCCESS;
}
