#include <Actias/IO/FileHandle.h>
#include <ActiasSDK/Platform/NativeExecutableFactory.h>
#include <ActiasSDK/Platform/Windows/Parser/NTHeader.h>
#include <ActiasSDK/Platform/Windows/PortableExecutable.h>

namespace Actias::SDK
{
    ExecutableParseResult<INativeExecutable*> LoadNativeExecutable(const ArraySlice<Byte>& rawBuffer)
    {
        if (PE::IsWindowsPEFile(rawBuffer))
        {
            return PE::PortableExecutable::Load(rawBuffer);
        }

        return Err(ExecutableParseError::UnknownExecutableFormat);
    }
} // namespace Actias::SDK

Actias::SDK::ExecutableParseError ActiasLoadNativeExecutable(Actias::SDK::INativeExecutable** ppExecutable,
                                                             ActiasNativeExecutableLoadInfo* pLoadInfo)
{
    Actias::ArraySlice rawBuffer(static_cast<Actias::Byte*>(pLoadInfo->pRawData), pLoadInfo->RawDataByteSize);
    auto result = Actias::SDK::LoadNativeExecutable(rawBuffer);
    if (result)
    {
        *ppExecutable = result.Unwrap();
        return Actias::SDK::ExecutableParseError::None;
    }

    return result.UnwrapErr();
}
