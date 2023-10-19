#pragma once
#include <Actias/Base/Byte.h>
#include <Actias/Containers/ArraySlice.h>
#include <ActiasSDK/Platform/INativeExecutable.h>
#include <ActiasSDK/Platform/Windows/Parser/PEModuleBuffer.h>

namespace Actias::SDK::PE
{
    class PortableExecutable final : public Object<INativeExecutable>
    {
        List<Byte> m_RawBuffer;
        Ptr<PEModuleBuffer> m_pVirtualBuffer;
        List<SectionHeader*> m_SectionHeaders;
        NTHeaderBase* m_pNTHeaders = nullptr;

    public:
        ACTIAS_RTTI_Class(PortableExecutable, "8DC33AFD-F7A2-4313-9C54-3ACD23332D05");

        static ExecutableParseResult<INativeExecutable*> Load(const ArraySlice<Byte>& rawBuffer);

        void CreateInformationHeader(ACBXFileInformationHeader* pHeader) override;
        void CreateSectionHeader(UInt32 sectionID, ACBXSectionHeader* pHeader) override;
        void CopySection(UInt32 sectionID, Byte* pDestination) override;
    };
} // namespace Actias::SDK::PE
