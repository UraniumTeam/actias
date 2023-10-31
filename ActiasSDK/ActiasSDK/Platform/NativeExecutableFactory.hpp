#pragma once
#include <Actias/System/Core.h>
#include <ActiasSDK/Base/Base.hpp>
#include <ActiasSDK/Parser/Result.hpp>

namespace Actias::SDK
{
    class INativeExecutable;
} // namespace Actias::SDK

ACTIAS_BEGIN_C

typedef struct ActiasNativeExecutableLoadInfo
{
    void* pRawData;
    UInt64 RawDataByteSize;
} ActiasNativeExecutableLoadInfo;

ACTIAS_SDK_API Actias::SDK::ExecutableParseError ACTIAS_ABI
ActiasLoadNativeExecutable(Actias::SDK::INativeExecutable** ppExecutable, const ActiasNativeExecutableLoadInfo* pLoadInfo);

ACTIAS_END_C
