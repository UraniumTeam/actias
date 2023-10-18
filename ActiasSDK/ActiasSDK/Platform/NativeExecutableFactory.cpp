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

        return Err(ExecutableParseError::UnknownExecutableFormat());
    }
} // namespace Actias::SDK
