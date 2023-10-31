#pragma once
#include <ActiasRuntime/Base/Base.hpp>
#include <Actias/ACBX/Loader.h>

extern "C" ACTIAS_RUNTIME_API ActiasResult ACTIAS_ABI ActiasRtRunLoader(ActiasHandle* pModuleHandle, ACBXLoaderRunInfo* pRunInfo);
