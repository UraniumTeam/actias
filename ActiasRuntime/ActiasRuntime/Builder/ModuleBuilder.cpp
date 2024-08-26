#include <Actias/IO/IStream.hpp>
#include <Actias/Strings/FixedString.hpp>
#include <Actias/Strings/String.hpp>
#include <Actias/System/Runtime.h>
#include <ActiasRuntime/Builder/ModuleBuilder.hpp>
#include <array>

namespace Actias::Runtime
{
    // OS-specific libraries to ignore
    constexpr std::array g_SystemLibraries = {
        StringSlice("KERNEL32"),
        StringSlice("UCRTBASED"),
        StringSlice("VCRUNTIME"),
        StringSlice("MSVCP140D"),
    };

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

        m_pImportHeader = reinterpret_cast<ACBXImportTableHeader*>(pointer);
        pointer         = AlignUpPtr(pointer + sizeof(ACBXImportTableHeader), ACBX_FILE_ALIGNMENT);

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

        for (USize i = 0; i < m_pFileInfoHeader->RelocationBlockCount; ++i)
        {
            if (pointer + sizeof(ACBXRelocationBlockHeader) - pRawData > static_cast<SSize>(rawSize))
            {
                return Err(ResultCode::InsufficientSize);
            }

            const auto* pHeader     = reinterpret_cast<ACBXRelocationBlockHeader*>(pointer);
            const auto* pRelocation = reinterpret_cast<const ACBXRelocationEntry*>(pHeader + 1);

            pointer += sizeof(ACBXRelocationBlockHeader);
            while (pRelocation->EntryData)
            {
                const USize offset = ACBX_GetRelocationOffset(pRelocation);
                m_Relocations.Push(pHeader->BaseAddress + offset);

                ++pRelocation;

                pointer += sizeof(ACBXRelocationEntry);
            }

            pointer = AlignUpPtr(pointer + sizeof(ACBXRelocationEntry), ACBX_FILE_ALIGNMENT);
        }

        const auto entrySize = AlignUp(sizeof(ACBXExportTableEntry), ACBX_FILE_ALIGNMENT);

        pointer = pRawData + m_pExportHeader->Address;
        pointer = AlignUpPtr(pointer + entrySize * m_pExportHeader->EntryCount, ACBX_FILE_ALIGNMENT);

        if (pointer - pRawData > static_cast<SSize>(rawSize))
        {
            return Err(ResultCode::InsufficientSize);
        }

        return OK{};
    }

    Result<ModuleInfo, ResultCode> ModuleBuilder::Build()
    {
        ActiasSystemProperties systemProperties;
        ActiasGetSystemProperties(&systemProperties);

        const auto sectionBaseAddress = AlignUp(m_TotalHeaderSize, systemProperties.PageSize);

        const auto lastSection = m_Sections.Back();
        const auto imageSize = AlignUp(sectionBaseAddress + lastSection->Address + lastSection->Size, systemProperties.PageSize);

        void* handle = ActiasVirtualAlloc(nullptr, imageSize, ACTIAS_MEMORY_PROTECTION_READ_WRITE);
        ActiasCopyMemory(handle, m_RawData.Data(), m_TotalHeaderSize);

        m_pMapped = ac_byte_cast(handle);

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
                ActiasInlineZeroMemory(m_pMapped + section->Address + section->RawSize, diff);
            }

            auto* pRawSection = &m_RawData[section->RawAddress];
            ActiasCopyMemory(m_pMapped + section->Address, pRawSection, section->RawSize);
        }

        for (UInt64 reloc : m_Relocations)
        {
            UInt64& address = *reinterpret_cast<UInt64*>(m_pMapped + reloc + sectionBaseAddress);
            address += reinterpret_cast<UInt64>(m_pMapped) + sectionBaseAddress;
            ACTIAS_Assert(*reinterpret_cast<Byte*>(address) == *reinterpret_cast<Byte*>(address));
        }

        ModuleInfo moduleInfo{};
        moduleInfo.ImageSize        = static_cast<UInt64>(imageSize);
        moduleInfo.Handle           = static_cast<ActiasHandle>(handle);
        moduleInfo.ReferenceCounter = 1;
        return moduleInfo;
    }

    VoidResult<ResultCode> ModuleBuilder::ImportAll()
    {
        ActiasSystemProperties systemProperties;
        ActiasGetSystemProperties(&systemProperties);

        const auto sectionBaseAddress = AlignUp(m_TotalHeaderSize, systemProperties.PageSize);
        const auto* pImportHeader     = m_pImportHeader;
        const auto* pLibraries        = reinterpret_cast<ACBXImportTableEntry*>(m_pMapped + pImportHeader->Address);

        for (USize i = 0; i < pImportHeader->EntryCount; ++i)
        {
            const auto& libEntry = pLibraries[i];
            const char* pLibName = reinterpret_cast<const char*>(m_pMapped + libEntry.NameAddress);

            FixedString<512> libName = pLibName;

            auto isSystemLibrary = false;
            for (const auto& systemLibrary : g_SystemLibraries)
            {
                if (libName.StartsWith(systemLibrary, false))
                {
                    isSystemLibrary = true;
                    break;
                }
            }

            if (isSystemLibrary)
            {
                continue;
            }

            libName += ACTIAS_NATIVE_DL_EXTENSION;

            ActiasHandle hLibrary;
            auto loadResult = ActiasLoadNativeModule(libName.Data(), &hLibrary);
            if (loadResult != ACTIAS_SUCCESS)
            {
                return Err(ResultCode::NoFileOrDirectory);
            }

            auto* importEntries = reinterpret_cast<USize*>(m_pMapped + libEntry.Address + sectionBaseAddress);
            for (USize j = 0; j < libEntry.EntryCount; ++j)
            {
                auto& importEntry = importEntries[j];

                const auto* nameEntry = reinterpret_cast<ACBXImportNameEntry*>(m_pMapped + importEntry + sectionBaseAddress);
                const char* pName     = nameEntry->Name;

                ActiasProc address;
                auto findResult = ActiasFindNativeSymbolAddress(hLibrary, pName, &address);
                if (findResult != ACTIAS_SUCCESS)
                {
                    return Err(ResultCode::UnknownError);
                }

                importEntry = reinterpret_cast<USize>(address);
            }
        }

        for (auto* section : m_Sections)
        {
            const auto protResult = ActiasVirtualProtect(
                m_pMapped + section->Address, section->Size, section->SectionFlags & ACTIAS_MEMORY_PROTECTION_ALL);

            if (protResult != ACTIAS_SUCCESS)
            {
                return Err(ResultCode::UnknownError);
            }
        }

        return OK{};
    }
} // namespace Actias::Runtime
