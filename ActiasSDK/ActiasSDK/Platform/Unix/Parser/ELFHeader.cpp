#include <ActiasSDK/Platform/Unix/Parser/ELFHeader.hpp>

namespace Actias::SDK::ELF
{
    bool IsUnixELFFile(const ArraySlice<Byte>& buffer)
    {
        if (buffer.Length() < sizeof(ELFHeader))
        {
            return false;
        }

        auto* pHeader = reinterpret_cast<ELFHeader*>(buffer.Data());
        return pHeader->IsValid();
    }
} // namespace Actias::SDK::ELF
