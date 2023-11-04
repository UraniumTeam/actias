#include <ActiasSDK/Platform/Windows/Parser/Exports.hpp>
#include <ActiasSDK/Platform/Windows/Parser/NTHeader.hpp>
#include <ActiasSDK/Platform/Windows/PortableExecutable.hpp>

namespace Actias::SDK::PE
{
    inline UInt32 ConvertSectionFlags(SectionFlags flags)
    {
        UInt32 result = 0;
        if (AnyFlagsActive(flags, SectionFlags::MemRead))
        {
            result |= ACBX_SECTION_READ_BIT;
        }
        if (AnyFlagsActive(flags, SectionFlags::MemWrite))
        {
            result |= ACBX_SECTION_WRITE_BIT;
        }
        if (AnyFlagsActive(flags, SectionFlags::MemExecute))
        {
            result |= ACBX_SECTION_EXECUTE_BIT;
        }

        return result;
    }

    inline USize GetExportNameLength(const char* pName)
    {
        // We ignore any decorations here
        const char* pEnd = pName;
        while (*pEnd != '\0' && *pEnd != '@')
        {
            ++pEnd;
        }

        return pEnd - pName;
    }

    ExecutableParseResult<INativeExecutable*> PortableExecutable::Load(const ArraySlice<Byte>& rawBuffer)
    {
        Ptr pResult = AllocateObject<PortableExecutable>();
        pResult->m_RawBuffer.Resize(rawBuffer.Length());
        rawBuffer.CopyDataTo(pResult->m_RawBuffer);

        const auto headerResult = ParseNTHeaders(pResult->m_RawBuffer);
        ACTIAS_GuardResult(headerResult);

        auto* pHeaders = headerResult.Unwrap();
        auto sections  = pHeaders->GetSectionHeaders();

        auto mapResult = PEModuleBuffer::MapToVirtual(pResult->m_RawBuffer, pHeaders, sections);
        ACTIAS_GuardResult(mapResult);

        auto* pMapped = mapResult.Unwrap();

        auto sectionBase = std::numeric_limits<UInt64>::max();
        for (const auto* section : sections)
        {
            if (section->VirtualAddress.Value < sectionBase)
            {
                sectionBase = section->VirtualAddress.Value;
            }
        }

        pResult->m_SectionBaseVA    = sectionBase;
        pResult->m_pVirtualBuffer   = pMapped;
        pResult->m_SectionHeaders   = sections;
        pResult->m_pNTHeaders       = pHeaders;
        pResult->m_pExportDirectory = pHeaders->GetExportDirectoryEntry(pMapped->GetImageHandle());
        pResult->m_pMappedBase      = ac_byte_cast(pMapped->GetImageHandle());

        return pResult.Detach();
    }

    void PortableExecutable::CreateInformationHeader(ACBXFileInformationHeader* pHeader)
    {
        pHeader->EntryPointAddress = m_pNTHeaders->GetOptionalHeader()->AddressOfEntryPoint;
        pHeader->TargetArch        = static_cast<UInt16>(m_pNTHeaders->Is64Bit() ? ACBX_ARCH_TYPE_x86_64 : ACBX_ARCH_TYPE_x86);
        pHeader->SectionCount      = static_cast<UInt16>(m_SectionHeaders.Size());
        pHeader->AttributeFlags    = static_cast<UInt32>(ACBX_ATTRIBUTE_LIBRARY_BIT);
        pHeader->SDKVersion        = ACTIAS_VERSION_1_0;
    }

    void PortableExecutable::CreateSectionHeader(UInt32 sectionID, ACBXSectionHeader* pHeader)
    {
        const auto* pSection = m_SectionHeaders[sectionID];

        pHeader->RawSize            = pSection->SizeOfRawData;
        pHeader->Size               = pSection->VirtualSize;
        pHeader->Address            = pSection->VirtualAddress.Value - m_SectionBaseVA;
        pHeader->RelocationsAddress = pSection->PointerToRelocations;
        pHeader->SectionFlags       = ConvertSectionFlags(pSection->Characteristics);
    }

    void ACTIAS_ABI PortableExecutable::CreateExportTableHeader(ACBXExportTableHeader* pHeader)
    {
        pHeader->EntryCount = m_pExportDirectory->NumberOfNames;
    }

    void ACTIAS_ABI PortableExecutable::CreateExportTableEntry(UInt64 entryID, ACBXExportTableEntry* pEntry,
                                                               ISymbolNameAllocator* pNameAllocator)
    {
        const auto* pExportDirectory = m_pExportDirectory;
        const auto* pAddressTable    = reinterpret_cast<UInt32*>(m_pMappedBase + pExportDirectory->AddressOfFunctions);
        const auto* pNameTable       = reinterpret_cast<UInt32*>(m_pMappedBase + pExportDirectory->AddressOfNames);
        const auto* pNameOrdinals    = reinterpret_cast<UInt16*>(m_pMappedBase + pExportDirectory->AddressOfNameOrdinals);

        const auto* pName       = m_pMappedBase + pNameTable[entryID];
        const auto nameByteSize = GetExportNameLength(reinterpret_cast<const char*>(pName));

        const auto ordinal    = pNameOrdinals[entryID];
        pEntry->SymbolAddress = pAddressTable[ordinal];

        auto* pAllocatedName = pNameAllocator->Allocate(nameByteSize + 1, pEntry->NameAddress);
        ActiasCopyMemory(pAllocatedName, pName, nameByteSize);
        static_cast<char*>(pAllocatedName)[nameByteSize] = '\0';
    }

    void PortableExecutable::CopySection(UInt32 sectionID, Byte* pDestination)
    {
        CopySectionData(m_RawBuffer, m_SectionHeaders[sectionID], pDestination);
    }
} // namespace Actias::SDK::PE
