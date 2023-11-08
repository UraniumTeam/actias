#pragma once
#include <Actias/ACBX/Loader.h>
#include <Actias/System/Core.h>

#if ActiasRuntime_EXPORTS
#    define ACTIAS_RUNTIME_API ACTIAS_EXPORT
#else
#    define ACTIAS_RUNTIME_API ACTIAS_IMPORT
#endif

// __vectorcall functions are decorated with @@bytesize-of-parameters
// REVIEW: maybe we should decorate them at runtime to ensure correctness on all platforms and compilers.

#define ActiasRtLoadModule_ProcName "ActiasRtLoadModule@@16"
#define ActiasRtUnloadModule_ProcName "ActiasRtUnloadModule@@8"
#define ActiasRtFindSymbolAddress_ProcName "ActiasRtFindSymbolAddress@@24"

ACTIAS_BEGIN_C

typedef ActiasResult ACTIAS_ABI ActiasRtLoadModuleProc(const ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pModuleHandle);
typedef ActiasResult ACTIAS_ABI ActiasRtUnloadModuleProc(ActiasHandle moduleHandle);
typedef ActiasResult ACTIAS_ABI ActiasRtFindSymbolAddressProc(ActiasHandle moduleHandle, const char* pSymbolName,
                                                              ActiasProc* pAddress);

ACTIAS_RUNTIME_API ActiasRtLoadModuleProc ActiasRtLoadModule;
ACTIAS_RUNTIME_API ActiasRtUnloadModuleProc ActiasRtUnloadModule;
ACTIAS_RUNTIME_API ActiasRtFindSymbolAddressProc ActiasRtFindSymbolAddress;

ACTIAS_END_C
