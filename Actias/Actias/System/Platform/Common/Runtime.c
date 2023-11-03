#include <Actias/System/Platform/Common/RuntimeDef.h>
#include <Actias/System/Runtime.h>

static ActiasHandle g_CurrentRuntimeModule = NULL;

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
    if (g_CurrentRuntimeModule == NULL)
    {
        return ACTIAS_FAIL_RT_NOT_INITIALIZED;
    }

    ActiasProc address     = NULL;
    ActiasResult symResult = ActiasFindNativeSymbolAddress(g_CurrentRuntimeModule, ACTIAS_RtRunLoader_ProcName, &address);
    if (symResult != ACTIAS_SUCCESS)
    {
        return symResult;
    }

    ActiasRtRunLoaderProc* runLoader = (ActiasRtRunLoaderProc*)address;

    return runLoader(pRunInfo, pModuleHandle);
}
