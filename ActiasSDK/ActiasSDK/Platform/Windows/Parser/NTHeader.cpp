#include <ActiasSDK/Platform/Windows/Parser/NTHeader.h>

namespace Actias::SDK::PE
{
    List<SectionHeader*> NTHeaderBase::GetSectionHeaders() noexcept
    {
        const auto count = FileHeader.NumberOfSections;

        List<SectionHeader*> result;
        result.Reserve(count);

        auto* sectionHeaders = Is64Bit()
            ? reinterpret_cast<SectionHeader*>(&As<ArchPointerSize::Arch64Bit>()->OptionalHeader + 1)
            : reinterpret_cast<SectionHeader*>(&As<ArchPointerSize::Arch32Bit>()->OptionalHeader + 1);

        for (USize i = 0; i < count; ++i)
        {
            result.Push(sectionHeaders + i);
        }

        return result;
    }

    ExecutableParseResult<NTHeaderBase*> ParseNTHeaders(const ArraySlice<Byte>& buffer)
    {
        if (buffer.Length() < sizeof(DOSHeader))
        {
            return Err(ExecutableParseError::InsufficientSize);
        }

        auto* pBuffer    = buffer.Data();
        auto* pDosHeader = reinterpret_cast<DOSHeader*>(pBuffer);
        if (!pDosHeader->IsValid())
        {
            return Err(ExecutableParseError::InvalidDOSHeader);
        }

        const auto ntOffset = pDosHeader->Lfanew;
        auto* ntHeaders     = reinterpret_cast<NTHeaderBase*>(pBuffer + ntOffset);

        if (buffer.Length() < sizeof(NTHeader32) + sizeof(DOSHeader))
        {
            return Err(ExecutableParseError::InsufficientSize);
        }

        if (!ntHeaders->IsValid())
        {
            return Err(ExecutableParseError::InvalidNTHeader);
        }

        switch (ntHeaders->GetArchPointerSize())
        {
        case ArchPointerSize::Arch32Bit:
            break;
        case ArchPointerSize::Arch64Bit:
            if (buffer.Length() < sizeof(NTHeader64) + sizeof(DOSHeader))
            {
                return Err(ExecutableParseError::InsufficientSize);
            }

            break;
        default:
            return Err(ExecutableParseError::InvalidNTHeader);
        }

        return ntHeaders;
    }

    bool IsWindowsPEFile(const ArraySlice<Byte>& buffer)
    {
        auto result = ParseNTHeaders(buffer);
        return result.IsOk();
    }
} // namespace Actias::SDK::PE
