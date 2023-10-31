#pragma once
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/IBlob.hpp>
#include <ActiasSDK/Base/Base.hpp>

namespace Actias::SDK
{
    class INativeExecutable;
} // namespace Actias::SDK

ACTIAS_BEGIN_C

typedef struct ActiasExecutableBuildInfo
{
    Actias::SDK::INativeExecutable* pNativeExecutable;
} ActiasExecutableBuildInfo;

ACTIAS_SDK_API void ACTIAS_ABI ActiasBuildExecutable(Actias::IBlob** ppExecutableData,
                                                     const ActiasExecutableBuildInfo* pBuildInfo);

ACTIAS_END_C
