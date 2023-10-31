#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/ArraySlice.hpp>
#include <ActiasSDK/Parser/Result.hpp>

namespace Actias::SDK::PE
{
    struct NTHeaderBase;
    struct SectionHeader;

    void CopySectionData(const ArraySlice<Byte>& rawBuffer, const SectionHeader* pSection, Byte* pDestination);

    class PEModuleBuffer : public BasicObject
    {
        ArraySlice<Byte> m_Buffer;

    public:
        ACTIAS_RTTI_Class(PEModuleBuffer, "2A4DD169-5429-4246-B185-F01DACD5D29E");

        PEModuleBuffer(Byte* pBuffer, USize byteSize);
        ~PEModuleBuffer();

        [[nodiscard]] static ExecutableParseResult<PEModuleBuffer*> MapToVirtual(const ArraySlice<Byte>& rawBuffer,
                                                                                 NTHeaderBase* pHeaders,
                                                                                 const ArraySlice<SectionHeader*>& sections);

        [[nodiscard]] inline ActiasHandle GetImageHandle()
        {
            return m_Buffer.Data();
        }

        [[nodiscard]] inline USize ByteSize() const
        {
            return m_Buffer.Length();
        }
    };
} // namespace Actias::SDK::PE
