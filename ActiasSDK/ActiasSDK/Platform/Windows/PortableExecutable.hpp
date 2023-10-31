#pragma once
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/ArraySlice.hpp>
#include <ActiasSDK/Platform/INativeExecutable.hpp>
#include <ActiasSDK/Platform/Windows/Parser/PEModuleBuffer.hpp>

namespace Actias::SDK::PE
{
    struct ExportDirectory;

    class PortableExecutable final : public Object<INativeExecutable>
    {
        List<Byte> m_RawBuffer;
        Ptr<PEModuleBuffer> m_pVirtualBuffer;
        List<SectionHeader*> m_SectionHeaders;

        Byte* m_pMappedBase                 = nullptr;
        NTHeaderBase* m_pNTHeaders          = nullptr;
        ExportDirectory* m_pExportDirectory = nullptr;

    public:
        ACTIAS_RTTI_Class(PortableExecutable, "8DC33AFD-F7A2-4313-9C54-3ACD23332D05");

        static ExecutableParseResult<INativeExecutable*> Load(const ArraySlice<Byte>& rawBuffer);

        void ACTIAS_ABI CreateInformationHeader(ACBXFileInformationHeader* pHeader) override;
        void ACTIAS_ABI CreateSectionHeader(UInt32 sectionID, ACBXSectionHeader* pHeader) override;
        void ACTIAS_ABI CreateExportTableHeader(ACBXExportTableHeader* pHeader) override;
        void ACTIAS_ABI CreateExportTableEntry(UInt64 entryID, ACBXExportTableEntry* pEntry,
                                               ISymbolNameAllocator* pNameAllocator) override;
        void ACTIAS_ABI CopySection(UInt32 sectionID, Byte* pDestination) override;
    };
} // namespace Actias::SDK::PE
