#pragma once
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/ArraySlice.hpp>
#include <ActiasSDK/Parser/Result.hpp>
#include <ActiasSDK/Platform/INativeExecutable.hpp>
#include <ActiasSDK/Platform/Unix/Parser/ELFHeader.hpp>

namespace Actias::SDK::ELF
{
    class ExecutableLinkableFormat final : public Object<INativeExecutable>
    {
        List<Byte> m_RawBuffer;

        ELFHeader* m_pELFHeader = nullptr;
        List<SectionHeader*> m_SectionHeaders;
        List<SectionHeader*> m_SectionHeaderTypes[ac_enum_cast(SectionType::Count)] = {};
        List<ProgramHeader*> m_SegmentHeaders;

        UInt64 m_SectionBaseVA = 0;

    public:
        ACTIAS_RTTI_Class(ExecutableLinkableFormat, "122A8CBB-A245-4263-BD5A-4FE0EC4405B6");

        static ExecutableParseResult<INativeExecutable*> Load(const ArraySlice<Byte>& rawBuffer);

        void ACTIAS_ABI CreateInformationHeader(ACBXFileInformationHeader* pHeader) override;
        void ACTIAS_ABI CreateSectionHeader(UInt32 sectionID, ACBXSectionHeader* pHeader) override;
        void ACTIAS_ABI CreateRelocationBlocks(IBlobAllocator* pAllocator) override;
        void ACTIAS_ABI CreateExportTableHeader(ACBXExportTableHeader* pHeader) override;
        void ACTIAS_ABI CreateExportTableEntry(UInt64 entryID, ACBXExportTableEntry* pEntry,
                                               IBlobAllocator* pNameAllocator) override;
        void ACTIAS_ABI CreateImportTableHeader(ACBXImportTableHeader* pHeader) override;
        void ACTIAS_ABI CreateImportTableLibraryHeader(UInt64 libraryID, ACBXImportTableEntry* pEntry,
                                                       IBlobAllocator* pNameAllocator) override;
        void ACTIAS_ABI CopySection(UInt32 sectionID, Byte* pDestination) override;
    };
} // namespace Actias::SDK::ELF
