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

            m_Sections.Push(pHeader);
            pointer = AlignUpPtr(pointer + sizeof(ACBXSectionHeader), ACBX_FILE_ALIGNMENT);
        }

        if (pointer + sizeof(ACBXExportTableHeader) - pRawData > static_cast<SSize>(rawSize))
        {
            return Err(ResultCode::InsufficientSize);
        }

        const auto entrySize = AlignUp(sizeof(ACBXExportTableEntry), ACBX_FILE_ALIGNMENT);

        m_pExportHeader = reinterpret_cast<ACBXExportTableHeader*>(pointer);
        pointer         = pRawData + m_pExportHeader->Address;
        pointer         = AlignUpPtr(pointer + entrySize * m_pExportHeader->EntryCount, ACBX_FILE_ALIGNMENT);

        m_TotalHeaderSize = pointer - pRawData;
        if (m_TotalHeaderSize > rawSize)
        {
            return Err(ResultCode::InsufficientSize);
        }

        return OK{};
    }
} // namespace Actias::Runtime
