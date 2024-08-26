#include <Actias/System/Platform/Common/RuntimeDef.h>
#include <Actias/System/Runtime.h>

static ActiasHandle g_CurrentRuntimeModule = NULL;
static ActiasHandle g_CurrentActiasModule  = NULL;

inline static ActiasResult LoadRuntimeProc(char* procName, ActiasProc* pAddress)
{
    if (g_CurrentRuntimeModule == NULL)
    {
        return ACTIAS_FAIL_RT_NOT_INITIALIZED;
    }

    return ActiasFindNativeSymbolAddress(g_CurrentRuntimeModule, procName, pAddress);
}

ActiasResult ACTIAS_ABI ActiasInit(ActiasHandle moduleHandle)
{
    if (g_CurrentRuntimeModule)
    {
        return ACTIAS_FAIL_RT_INITIALIZED;
    }

    g_CurrentActiasModule = moduleHandle;
    return ActiasLoadNativeModule("./ActiasRuntime" ACTIAS_NATIVE_DL_EXTENSION, &g_CurrentRuntimeModule);
}

ActiasResult ACTIAS_ABI ActiasShutdown()
{
    ActiasResult result    = ActiasUnloadNativeModule(g_CurrentRuntimeModule);
    g_CurrentActiasModule  = NULL;
    g_CurrentRuntimeModule = NULL;
    return result;
}

ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasGetCurrentModule(ActiasHandle* pHandle)
{
    if (g_CurrentActiasModule)
    {
        if (!pHandle)
            return ACTIAS_FAIL_INVALID_ARGUMENT;

        *pHandle = g_CurrentActiasModule;
        return ACTIAS_SUCCESS;
    }

    if (g_CurrentRuntimeModule == NULL)
    {
        return ACTIAS_FAIL_RT_NOT_INITIALIZED;
    }

    return ACTIAS_FAIL_NOT_SUPPORTED;
}

ActiasResult ACTIAS_ABI ActiasLoadModuleEx(const ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pModuleHandle)
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

ActiasResult ACTIAS_ABI ActiasFindSymbolAddress(ActiasHandle moduleHandle, const char* pSymbolName, ActiasProc* pAddress)
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
