#include <Actias/System/Platform/Common/RuntimeDef.h>
#include <Actias/System/Runtime.h>

static ActiasHandle g_CurrentRuntimeModule = NULL;

inline static ActiasResult LoadRuntimeProc(char* procName, ActiasProc* pAddress)
{
    if (g_CurrentRuntimeModule == NULL)
    {
        return ACTIAS_FAIL_RT_NOT_INITIALIZED;
    }

    return ActiasFindNativeSymbolAddress(g_CurrentRuntimeModule, procName, pAddress);
}

ActiasResult ACTIAS_ABI ActiasInit(void)
{
    if (g_CurrentRuntimeModule)
    {
        return ACTIAS_FAIL_RT_INITIALIZED;
    }

    return ActiasLoadNativeModule("ActiasRuntime", &g_CurrentRuntimeModule);
}

ActiasResult ACTIAS_ABI ActiasLoadModuleEx(ACTIAS_CONST ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pModuleHandle)
{
    ActiasProc address     = NULL;
    ActiasResult symResult = LoadRuntimeProc(ActiasRtLoadModule_ProcName, &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        return symResult;
    }

    ActiasRtLoadModuleProc* load = (ActiasRtLoadModuleProc*)address;
    return load(pRunInfo, pModuleHandle);
}

ActiasResult ACTIAS_ABI ActiasUnloadModule(ActiasHandle moduleHandle)
{
    ActiasProc address     = NULL;
    ActiasResult symResult = LoadRuntimeProc(ActiasRtUnloadModule_ProcName, &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        return symResult;
    }

    ActiasRtUnloadModuleProc* unload = (ActiasRtUnloadModuleProc*)address;
    return unload(moduleHandle);
}

ActiasResult ACTIAS_ABI ActiasFindSymbolAddress(ActiasHandle moduleHandle, ACTIAS_CONST char* pSymbolName, ActiasProc* pAddress)
{
    ActiasProc address     = NULL;
    ActiasResult symResult = LoadRuntimeProc(ActiasRtFindSymbolAddress_ProcName, &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        return symResult;
    }

    ActiasRtFindSymbolAddressProc* find = (ActiasRtFindSymbolAddressProc*)address;
    return find(moduleHandle, pSymbolName, pAddress);
}
