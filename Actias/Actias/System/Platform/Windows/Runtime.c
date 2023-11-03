#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Runtime.h>

ActiasResult ACTIAS_ABI ActiasLoadNativeModule(ACTIAS_CONST char* pFilePath, ActiasHandle* pHandle)
{
    HMODULE handle = LoadLibraryA(pFilePath);
    if (handle == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pHandle = handle;
    return ACTIAS_SUCCESS;
}

ActiasResult ACTIAS_ABI ActiasFindNativeSymbolAddress(ActiasHandle moduleHandle, ACTIAS_CONST char* pSymbolName,
                                                      ActiasProc* pAddress)
{
    FARPROC address = GetProcAddress(moduleHandle, pSymbolName);
    if (address == NULL)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }

    *pAddress = (ActiasProc)address;
    return ACTIAS_SUCCESS;
}
