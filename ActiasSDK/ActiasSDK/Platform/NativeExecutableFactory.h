#pragma once
#include <Actias/System/Core.h>
#include <ActiasSDK/Base/Base.h>
#include <ActiasSDK/Parser/Result.h>

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

ACTIAS_SDK_API Actias::SDK::ExecutableParseError ActiasLoadNativeExecutable(Actias::SDK::INativeExecutable** ppExecutable,
                                                                            ActiasNativeExecutableLoadInfo* pLoadInfo);

ACTIAS_END_C
