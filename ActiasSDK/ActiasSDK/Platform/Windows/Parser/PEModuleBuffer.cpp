#include <Actias/System/Memory.h>
#include <ActiasSDK/Platform/Windows/Parser/NTHeader.h>
#include <ActiasSDK/Platform/Windows/Parser/PEModuleBuffer.h>

namespace Actias::SDK::PE
{
    PEModuleBuffer::PEModuleBuffer(Byte* pBuffer, USize byteSize)
        : m_Buffer(pBuffer, byteSize)
    {
    }

    PEModuleBuffer::~PEModuleBuffer()
    {
        ActiasVirtualFree(m_Buffer.Data(), m_Buffer.Length());
    }

    void CopySectionData(const ArraySlice<Byte>& rawBuffer, const SectionHeader* pSection, Byte* pDestination)
    {
        auto size        = pSection->SizeOfRawData;
        const auto* pRaw = &rawBuffer[pSection->PointerToRawData];
        ActiasCopyMemory(pDestination, pRaw, size);
    }

    static VoidResult<ExecutableParseError> MapSections(const ArraySlice<Byte>& rawBuffer,
                                                        const ArraySlice<Byte>& destinationBuffer, NTHeaderBase* pHeaders,
                                                        const ArraySlice<SectionHeader*>& sections)
    {
        auto headersSize = pHeaders->GetHeadersSize();

        UInt32 firstRawSection = std::numeric_limits<UInt32>::max();

        for (const auto* section : sections)
        {
            if (section->PointerToRawData == 0 || section->SizeOfRawData == 0)
            {
                continue;
            }

            auto size = section->SizeOfRawData;

            if ((section->VirtualAddress.Value + static_cast<USize>(size)) > destinationBuffer.Length()
                || (section->VirtualAddress.Value >= destinationBuffer.Length() && size != 0))
            {
                return Err(ExecutableParseError::SectionOutOfBounds);
            }

            auto* pMapped = &destinationBuffer[section->VirtualAddress.Value];
            CopySectionData(rawBuffer, section, pMapped);

            if (section->PointerToRawData < firstRawSection)
            {
                firstRawSection = section->PointerToRawData;
            }
        }

        if (headersSize == 0 && firstRawSection != std::numeric_limits<UInt32>::max())
        {
            headersSize = firstRawSection;
        }

        rawBuffer.CopyDataTo(destinationBuffer, headersSize);
        return OK{};
    }

    ExecutableParseResult<PEModuleBuffer*> PEModuleBuffer::MapToVirtual(const ArraySlice<Byte>& rawBuffer, NTHeaderBase* pHeaders,
                                                                        const ArraySlice<SectionHeader*>& sections)
    {
        ActiasSystemProperties systemProps;
        ActiasGetSystemProperties(&systemProps);
        USize pageSize = systemProps.PageSize;

        const auto imageSize = AlignUp(static_cast<USize>(pHeaders->GetImageSize()), pageSize);

        auto* pLocalCopy = ActiasVirtualAlloc(nullptr, imageSize, ACTIAS_MEMORY_PROTECTION_READ_WRITE_BIT);

        auto* pResult  = AllocateObject<PEModuleBuffer>(static_cast<Byte*>(pLocalCopy), imageSize);
        auto mapResult = MapSections(rawBuffer, pResult->m_Buffer, pHeaders, sections);
        ACTIAS_GuardResult(mapResult);

        return pResult;
    }
} // namespace Actias::SDK::PE
