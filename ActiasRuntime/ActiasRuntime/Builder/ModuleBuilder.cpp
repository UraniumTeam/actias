#include <Actias/IO/IStream.hpp>
#include <ActiasRuntime/Builder/ModuleBuilder.hpp>

namespace Actias::Runtime
{
    VoidResult<IO::ResultCode> ModuleBuilder::LoadFromStream(IO::IStream* pStream)
    {
        auto lengthResult = pStream->Length();
        ACTIAS_GuardResult(lengthResult);

        auto length = lengthResult.Unwrap();
        m_RawData   = HeapArray<Byte>::CreateUninitialized(length);

        auto readResult = pStream->ReadToBuffer(m_RawData.Data(), length);
        ACTIAS_GuardResult(readResult);

        return OK{};
    }

    VoidResult<ResultCode> ModuleBuilder::ParseHeaders()
    {
        if (m_pFileInfoHeader)
        {
            return Err(ResultCode::NotSupported);
        }

        const auto rawSize = m_RawData.Length();
        if (rawSize < sizeof(ACBXFileInformationHeader))
        {
            return Err(ResultCode::InsufficientSize);
        }

        for (USize i = 0; i < ACBX_SIGNATURE_SIZE; ++i)
        {
            if (m_RawData[i] != static_cast<Byte>(ACBXSignature[i]))
            {
                return Err(ResultCode::InvalidSignature);
            }
        }

        auto* pRawData    = m_RawData.Data();
        m_pFileInfoHeader = reinterpret_cast<ACBXFileInformationHeader*>(pRawData);

        auto* pointer = AlignUpPtr(pRawData + sizeof(ACBXFileInformationHeader), ACBX_FILE_ALIGNMENT);

        if (pointer + sizeof(ACBXExportTableHeader) - pRawData > static_cast<SSize>(rawSize))
        {
            return Err(ResultCode::InsufficientSize);
        }

        m_pExportHeader = reinterpret_cast<ACBXExportTableHeader*>(pointer);
        pointer         = AlignUpPtr(pointer + sizeof(ACBXExportTableHeader), ACBX_FILE_ALIGNMENT);

        m_TotalHeaderSize = std::numeric_limits<USize>::max();
        for (USize i = 0; i < m_pFileInfoHeader->SectionCount; ++i)
        {
            if (pointer + sizeof(ACBXSectionHeader) - pRawData > static_cast<SSize>(rawSize))
            {
                return Err(ResultCode::InsufficientSize);
            }

            auto* pHeader = reinterpret_cast<ACBXSectionHeader*>(pointer);
            if (pHeader->RawAddress > rawSize || pHeader->RawAddress + pHeader->RawSize > rawSize)
            {
                return Err(ResultCode::SectionOutOfBounds);
            }

            if (pHeader->RawAddress < m_TotalHeaderSize)
            {
                m_TotalHeaderSize = pHeader->RawAddress;
            }

            m_Sections.Push(pHeader);
            pointer = AlignUpPtr(pointer + sizeof(ACBXSectionHeader), ACBX_FILE_ALIGNMENT);
        }

        m_Sections.Sort([](ACBXSectionHeader* left, ACBXSectionHeader* right) {
            return left->Address < right->Address;
        });

        const auto entrySize = AlignUp(sizeof(ACBXExportTableEntry), ACBX_FILE_ALIGNMENT);

        pointer = pRawData + m_pExportHeader->Address;
        pointer = AlignUpPtr(pointer + entrySize * m_pExportHeader->EntryCount, ACBX_FILE_ALIGNMENT);

        if (pointer - pRawData > static_cast<SSize>(rawSize))
        {
            return Err(ResultCode::InsufficientSize);
        }

        return OK{};
    }

    Result<ActiasHandle, ResultCode> ModuleBuilder::Build()
    {
        ActiasSystemProperties systemProperties;
        ActiasGetSystemProperties(&systemProperties);

        auto sectionBaseAddress = AlignUp(m_TotalHeaderSize, systemProperties.PageSize);

        auto lastSection = m_Sections.Back();
        auto imageSize   = AlignUp(sectionBaseAddress + lastSection->Address + lastSection->Size, systemProperties.PageSize);

        ActiasHandle handle = ActiasVirtualAlloc(nullptr, imageSize, ACTIAS_MEMORY_PROTECTION_READ_WRITE);
        ActiasCopyMemory(handle, m_RawData.Data(), m_TotalHeaderSize);

        *reinterpret_cast<UInt64*>(handle) = static_cast<UInt64>(imageSize);

        auto* pImageBase = ac_byte_cast(handle);
        for (auto* section : m_Sections)
        {
            if (section->RawAddress == 0 || section->Size == 0)
            {
                continue;
            }

            section->Address = sectionBaseAddress + section->Address;

            if (section->Size > section->RawSize)
            {
                const auto diff = section->Size - section->RawSize;
                ActiasInlineZeroMemory(pImageBase + section->Address + section->RawSize, diff);
            }

            auto* pRawSection = &m_RawData[section->RawAddress];
            ActiasCopyMemory(pImageBase + section->Address, pRawSection, section->RawSize);

            const auto protResult = ActiasVirtualProtect(
                pImageBase + section->Address, section->Size, section->SectionFlags & ACTIAS_MEMORY_PROTECTION_ALL);

            if (protResult != ACTIAS_SUCCESS)
            {
                return Err(ResultCode::UnknownError);
            }
        }

        return handle;
    }
} // namespace Actias::Runtime
