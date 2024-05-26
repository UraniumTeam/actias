#include <ActiasSDK/Platform/Unix/ExecutableLinkableFormat.hpp>

namespace Actias::SDK::ELF
{
    inline static UInt32 ConvertSegmentFlags(SegmentFlags flags)
    {
        UInt32 result = 0;
        if (AnyFlagsActive(flags, SegmentFlags::Read))
        {
            result |= ACBX_SECTION_READ_BIT;
        }
        if (AnyFlagsActive(flags, SegmentFlags::Write))
        {
            result |= ACBX_SECTION_WRITE_BIT;
        }
        if (AnyFlagsActive(flags, SegmentFlags::Execute))
        {
            result |= ACBX_SECTION_EXECUTE_BIT;
        }

        return result;
    }

    ExecutableParseResult<INativeExecutable*> ExecutableLinkableFormat::Load(const ArraySlice<Byte>& rawBuffer)
    {
        Ptr pResult = AllocateObject<ExecutableLinkableFormat>();
        pResult->m_RawBuffer.Resize(rawBuffer.Length());
        rawBuffer.CopyDataTo(pResult->m_RawBuffer);

        if (pResult->m_RawBuffer.Size() < sizeof(ELFHeader))
        {
            return Err(ExecutableParseError::InsufficientSize);
        }

        auto* pHeader = reinterpret_cast<ELFHeader*>(pResult->m_RawBuffer.Data());
        if (!pHeader->IsValid())
        {
            return Err(ExecutableParseError::InvalidELFHeader);
        }

        if (pResult->m_RawBuffer.Size() < pHeader->Shoff + pHeader->Shnum * sizeof(SectionHeader))
        {
            return Err(ExecutableParseError::InsufficientSize);
        }

        if (pResult->m_RawBuffer.Size() < pHeader->Phoff + pHeader->Phnum * sizeof(ProgramHeader))
        {
            return Err(ExecutableParseError::InsufficientSize);
        }

        auto* pSections  = SectionHeader::Find(pHeader);
        auto sectionBase = std::numeric_limits<UInt64>::max();
        for (UInt32 sectionIndex = 0; sectionIndex < pHeader->Shnum; ++sectionIndex)
        {
            SectionHeader& section = pSections[sectionIndex];
            pResult->m_SectionHeaders.Push(&section);

            const auto sectionTypeIndex = ac_enum_cast(section.Type);
            if (sectionTypeIndex < ac_enum_cast(SectionType::Count))
            {
                pResult->m_SectionHeaderTypes[sectionTypeIndex].Push(&section);
            }

            if (section.Addr < sectionBase && section.Type != SectionType::Null)
            {
                sectionBase = section.Addr;
            }
        }

        auto* pSegments = ProgramHeader::Find(pHeader);
        for (UInt32 segmentIndex = 0; segmentIndex < pHeader->Phnum; ++segmentIndex)
        {
            pResult->m_SegmentHeaders.Push(&pSegments[segmentIndex]);
        }

        pResult->m_pELFHeader    = pHeader;
        pResult->m_SectionBaseVA = sectionBase;

        return pResult.Detach();
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CreateInformationHeader(ACBXFileInformationHeader* pHeader)
    {
        pHeader->EntryPointAddress    = m_pELFHeader->Entry;
        pHeader->TargetArch           = ACBX_ARCH_TYPE_x86_64;
        pHeader->SectionCount         = static_cast<UInt16>(m_SectionHeaders.Size());
        pHeader->AttributeFlags       = static_cast<UInt32>(ACBX_ATTRIBUTE_LIBRARY_BIT);
        pHeader->SDKVersion           = ACTIAS_VERSION_1_0;
        pHeader->RelocationBlockCount = 0;
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CreateSectionHeader(UInt32 sectionID, ACBXSectionHeader* pHeader)
    {
        const auto* pSection = m_SectionHeaders[sectionID];

        pHeader->RawSize = pSection->Size;
        pHeader->Size    = pSection->Size;
        pHeader->Address = pSection->Addr - m_SectionBaseVA;

        for (ProgramHeader* pSegment : m_SegmentHeaders)
        {
            if (pSegment->Vaddr <= pSection->Addr && pSegment->Vaddr + pSegment->Memsz >= pSection->Addr)
            {
                pHeader->SectionFlags = ConvertSegmentFlags(pSegment->Flags);
                break;
            }
        }
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CreateRelocationBlocks(IBlobAllocator* pAllocator)
    {
        ACTIAS_Unused(pAllocator);
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CreateExportTableHeader(ACBXExportTableHeader* pHeader)
    {
        pHeader->EntryCount = 0;
        for (SectionHeader* pDynSymTable : m_SectionHeaderTypes[ac_enum_cast(SectionType::DynSymbolTable)])
        {
            const UInt32 symbolCount = pDynSymTable->Size / sizeof(Symbol);
            const Symbol* pSymbols   = reinterpret_cast<Symbol*>(pDynSymTable->Offset + m_RawBuffer.Data());
            for (UInt32 symbolIndex = 0; symbolIndex < symbolCount; ++symbolIndex)
            {
                const Symbol& sym = pSymbols[symbolIndex];
                if (sym.Index != 0 && sym.Visibility == SymbolVisibility::Default)
                    ++pHeader->EntryCount;
            }
        }
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CreateExportTableEntry(UInt64 entryID, ACBXExportTableEntry* pEntry,
                                                                     IBlobAllocator* pNameAllocator)
    {
        const char* pDynStr = nullptr;

        SectionHeader* pElfStringTableSection = m_SectionHeaders[m_pELFHeader->Shstrndx];
        const char* elfStringTable = reinterpret_cast<const char*>(m_RawBuffer.Data()) + pElfStringTableSection->Offset;
        for (SectionHeader* pStrTable : m_SectionHeaderTypes[ac_enum_cast(SectionType::StringTable)])
        {
            const char* sectionName = elfStringTable + pStrTable->Name;
            if (Str::ByteCompare(sectionName, ".dynstr") == 0)
            {
                pDynStr = reinterpret_cast<const char*>(m_RawBuffer.Data()) + pStrTable->Offset;
                break;
            }
        }

        for (SectionHeader* pDynSymTable : m_SectionHeaderTypes[ac_enum_cast(SectionType::DynSymbolTable)])
        {
            const UInt32 symbolCount = pDynSymTable->Size / sizeof(Symbol);
            const Symbol* pSymbols   = reinterpret_cast<Symbol*>(pDynSymTable->Offset + m_RawBuffer.Data());
            for (UInt32 symbolIndex = 0; symbolIndex < symbolCount; ++symbolIndex)
            {
                const Symbol& sym = pSymbols[symbolIndex];
                if (sym.Index != 0 && sym.Visibility == SymbolVisibility::Default)
                {
                    pEntry->SymbolAddress = sym.Value;

                    const StringSlice name = pDynStr + sym.Name;
                    void* pAllocatedName   = pNameAllocator->Allocate(name.Size() + 1, pEntry->NameAddress);
                    ActiasCopyMemory(pAllocatedName, name.Data(), name.Size());
                }
            }
        }
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CreateImportTableHeader(ACBXImportTableHeader* pHeader)
    {
        pHeader->EntryCount = 0;
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CreateImportTableLibraryHeader(UInt64 libraryID, ACBXImportTableEntry* pEntry,
                                                                             IBlobAllocator* pNameAllocator)
    {
        ACTIAS_Unused(libraryID);
        ACTIAS_Unused(pEntry);
        ACTIAS_Unused(pNameAllocator);
    }

    void ACTIAS_ABI ExecutableLinkableFormat::CopySection(UInt32 sectionID, Byte* pDestination)
    {
        const auto* pSection = m_SectionHeaders[sectionID];
        const auto* pSource  = pSection->Offset + m_RawBuffer.Data();

        ActiasCopyMemory(pDestination, pSource, pSection->Size);
    }
} // namespace Actias::SDK::ELF
