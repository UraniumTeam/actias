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

        pResult->m_SectionBaseVA      = sectionBase;
        pResult->m_pVirtualBuffer     = pMapped;
        pResult->m_SectionHeaders     = sections;
        pResult->m_pNTHeaders         = pHeaders;
        pResult->m_pExportDirectory   = pHeaders->GetExportDirectoryEntry(pMapped->GetImageHandle());
        pResult->m_pImportDescriptors = pHeaders->GetImportDescriptors(pMapped->GetImageHandle());
        pResult->m_pMappedBase        = reinterpret_cast<Byte*>(pMapped->GetImageHandle());

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

        const StringSlice nameRaw(reinterpret_cast<const char*>(m_pMappedBase + pNameTable[entryID]));
        const StringSlice name(nameRaw.begin(), nameRaw.FindFirstOf('@'));

        const auto ordinal    = pNameOrdinals[entryID];
        pEntry->SymbolAddress = pAddressTable[ordinal];

        auto* pAllocatedName = pNameAllocator->Allocate(name.Size() + 1, pEntry->NameAddress);
        ActiasCopyMemory(pAllocatedName, name.Data(), name.Size());
        static_cast<char*>(pAllocatedName)[name.Size()] = '\0';
    }

    void ACTIAS_ABI PortableExecutable::CreateImportTableHeader(ACBXImportTableHeader* pHeader)
    {
        const auto* pImportDescriptors = m_pImportDescriptors;

        if (!pImportDescriptors)
        {
            pHeader->EntryCount = 0;
            return;
        }

        UInt64 count = 0;
        for (USize i = 0;; ++i)
        {
            const auto currentSize = reinterpret_cast<const Byte*>(&pImportDescriptors[i + 1]) - m_pMappedBase;
            if (currentSize > static_cast<SSize>(m_pVirtualBuffer->ByteSize()))
            {
                ACTIAS_Unreachable();
                break;
            }

            if (pImportDescriptors[i].FirstThunk == 0 && pImportDescriptors[i].OriginalFirstThunk == 0)
            {
                break;
            }

            ++count;
        }

        pHeader->EntryCount = count;
    }

    template<class T>
    inline UInt64 ProcessImportThunks(Byte* pBase, USize mappedSize, UInt32 firstThunk, UInt64 sectionBaseVA)
    {
        auto* pThunks = reinterpret_cast<T*>(pBase + firstThunk);

        UInt64 count = 0;
        for (USize i = 0;; ++i)
        {
            auto& thunk = pThunks[i];

            if (firstThunk + i * sizeof(T) > mappedSize)
            {
                ACTIAS_Unreachable();
                break;
            }
            if (thunk == 0)
            {
                break;
            }

            const auto* pImportByName = reinterpret_cast<ImportByName*>(pBase + thunk);
            ACTIAS_Unused(pImportByName);

            thunk -= static_cast<T>(sectionBaseVA);

            ++count;
        }

        return count;
    }

    void ACTIAS_ABI PortableExecutable::CreateImportTableLibraryHeader(UInt64 libraryID, ACBXImportTableEntry* pEntry,
                                                                       ISymbolNameAllocator* pNameAllocator)
    {
        const auto& importDescriptor = m_pImportDescriptors[libraryID];

        const auto thunk = importDescriptor.FirstThunk;

        StringSlice nameRaw(reinterpret_cast<const char*>(m_pMappedBase + importDescriptor.Name));
        StringSlice name(nameRaw.begin(), nameRaw.FindLastOf('.'));

        auto* pAllocatedName = pNameAllocator->Allocate(name.Size() + 1, pEntry->NameAddress);
        ActiasCopyMemory(pAllocatedName, name.Data(), name.Size());
        static_cast<char*>(pAllocatedName)[name.Size()] = '\0';

        pEntry->Address = thunk - m_SectionBaseVA;

        if (m_pNTHeaders->Is64Bit())
        {
            pEntry->EntryCount = ProcessImportThunks<UInt64>(m_pMappedBase, m_pVirtualBuffer->ByteSize(), thunk, m_SectionBaseVA);
        }
        else
        {
            pEntry->EntryCount = ProcessImportThunks<UInt32>(m_pMappedBase, m_pVirtualBuffer->ByteSize(), thunk, m_SectionBaseVA);
        }
    }

    void ACTIAS_ABI PortableExecutable::CopySection(UInt32 sectionID, Byte* pDestination)
    {
        const auto* pSection = m_SectionHeaders[sectionID];
        const auto* pSource  = pSection->VirtualAddress.ToPointer<Byte>(m_pMappedBase);

        ActiasCopyMemory(pDestination, pSource, pSection->SizeOfRawData);
    }
} // namespace Actias::SDK::PE
