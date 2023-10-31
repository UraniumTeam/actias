#include <Actias/System/Platform/Windows/WinHeaders.h>
#include <Actias/System/Runtime.h>

ActiasResult ACTIAS_ABI ActiasLoadModule(ACTIAS_CONST char* pFilePath, ActiasHandle* pHandle)
{
    ACBXLoaderRunInfo runInfo;
    runInfo.pModulePath = pFilePath;
    return ActiasLoadModuleEx(&runInfo, pHandle);
}

ActiasResult ACTIAS_ABI ActiasLoadModuleEx(ACTIAS_CONST ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pHandle)
{
}
