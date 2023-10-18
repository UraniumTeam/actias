#include <Actias/System/Memory.h>
#include <ActiasSDK/Platform/Windows/Parser/PEModuleBuffer.h>
#include <ActiasSDK/Platform/Windows/Parser/NTHeader.h>

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

    static VoidResult<ExecutableParseError> MapSections(const ArraySlice<Byte>& rawBuffer,
                                                        const ArraySlice<Byte>& destinationBuffer, NTHeaderBase* pHeaders)
    {
        auto headersSize = pHeaders->GetHeadersSize();
        auto sections    = pHeaders->GetSectionHeaders();

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
                return Err(ExecutableParseError::SectionOutOfBounds(reinterpret_cast<const Byte*>(section) - rawBuffer.Data()));
            }

            auto* pMapped    = &destinationBuffer[section->VirtualAddress.Value];
            const auto* pRaw = &rawBuffer[section->PointerToRawData];
            ActiasCopyMemory(pMapped, pRaw, size);
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

    ExecutableParseResult<PEModuleBuffer*> PEModuleBuffer::MapToVirtual(const ArraySlice<Byte>& rawBuffer)
    {
        const auto headerResult = ParseNTHeaders(rawBuffer);
        ACTIAS_GuardResult(headerResult);

        ActiasSystemProperties systemProps;
        ActiasGetSystemProperties(&systemProps);
        USize pageSize = systemProps.PageSize;

        auto* pHeaders       = headerResult.Unwrap();
        const auto imageSize = AlignUp(pHeaders->GetImageSize(), pageSize);

        auto* pLocalCopy = ActiasVirtualAlloc(nullptr, imageSize, ACTIAS_MEMORY_PROTECTION_READ_WRITE_EXECUTE_BIT);

        auto* pResult  = AllocateObject<PEModuleBuffer>(pLocalCopy, imageSize);
        auto mapResult = MapSections(rawBuffer, pResult->m_Buffer, pHeaders);
        ACTIAS_GuardResult(mapResult);

        return pResult;
    }
} // namespace Actias::SDK::PE
