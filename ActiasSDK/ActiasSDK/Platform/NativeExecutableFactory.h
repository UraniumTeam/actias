#pragma once
#include <Actias/Base/Byte.h>
#include <Actias/Containers/ArraySlice.h>
#include <Actias/Strings/StringSlice.h>
#include <ActiasSDK/Parser/Result.h>

namespace Actias::SDK
{
    class INativeExecutable;

    ExecutableParseResult<INativeExecutable*> LoadNativeExecutable(const ArraySlice<Byte>& rawBuffer);
} // namespace Actias::SDK
