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

#define ACTIAS_RtRunLoader_ProcName "ActiasRtRunLoader@@16"

ACTIAS_BEGIN_C

typedef ActiasResult ACTIAS_ABI ActiasRtRunLoaderProc(ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pModuleHandle);
ACTIAS_RUNTIME_API ActiasRtRunLoaderProc ActiasRtRunLoader;

ACTIAS_END_C
