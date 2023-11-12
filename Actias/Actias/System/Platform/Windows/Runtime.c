#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Runtime.h>

ActiasResult ACTIAS_ABI ActiasLoadNativeModule(const char* pFilePath, ActiasHandle* pHandle)
{
    HMODULE handle = LoadLibraryA(pFilePath);
    if (handle == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pHandle = (ActiasHandle)handle;
    return ACTIAS_SUCCESS;
}

ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasUnloadNativeModule(ActiasHandle moduleHandle)
{
    BOOL result = FreeLibrary((HANDLE)moduleHandle);
    if (result)
    {
        return ACTIAS_SUCCESS;
    }

    return ACTIAS_FAIL_UNKNOWN;
}

ActiasResult ACTIAS_ABI ActiasFindNativeSymbolAddress(ActiasHandle moduleHandle, const char* pSymbolName, ActiasProc* pAddress)
{
    FARPROC address = GetProcAddress((HANDLE)moduleHandle, pSymbolName);
    if (address == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pAddress = (ActiasProc)address;
    return ACTIAS_SUCCESS;
}
