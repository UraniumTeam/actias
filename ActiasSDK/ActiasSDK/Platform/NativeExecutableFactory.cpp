#include <Actias/Utils/Result.hpp>
#include <ActiasSDK/Platform/NativeExecutableFactory.hpp>
#include <ActiasSDK/Platform/Unix/ExecutableLinkableFormat.hpp>
#include <ActiasSDK/Platform/Unix/Parser/ELFHeader.hpp>
#include <ActiasSDK/Platform/Windows/Parser/NTHeader.hpp>
#include <ActiasSDK/Platform/Windows/PortableExecutable.hpp>

namespace Actias::SDK
{
    ExecutableParseResult<INativeExecutable*> LoadNativeExecutable(const ArraySlice<Byte>& rawBuffer)
    {
        if (PE::IsWindowsPEFile(rawBuffer))
        {
            return PE::PortableExecutable::Load(rawBuffer);
        }

        if (ELF::IsUnixELFFile(rawBuffer))
        {
            return ELF::ExecutableLinkableFormat::Load(rawBuffer);
        }

        return Err(ExecutableParseError::UnknownExecutableFormat);
    }
} // namespace Actias::SDK

using namespace Actias;
using namespace Actias::SDK;

extern "C" ExecutableParseError ACTIAS_ABI ActiasLoadNativeExecutable(INativeExecutable** ppExecutable,
                                                                      const ActiasNativeExecutableLoadInfo* pLoadInfo)
{
    ArraySlice rawBuffer(static_cast<Byte*>(pLoadInfo->pRawData), pLoadInfo->RawDataByteSize);
    auto result = LoadNativeExecutable(rawBuffer);
    if (result)
    {
        *ppExecutable = result.Unwrap();
        return ExecutableParseError::None;
    }

    return result.UnwrapErr();
}
