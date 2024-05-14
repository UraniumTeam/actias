#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/ArraySlice.hpp>
#include <Actias/Containers/List.hpp>
#include <ActiasSDK/Parser/Result.hpp>
#include <ActiasSDK/Platform/Windows/Parser/PEDataDirectory.hpp>

namespace Actias::SDK::PE
{
    namespace MagicConst
    {
        inline constexpr UInt32 NT  = 0x00004550;
        inline constexpr UInt16 DOS = 0x5A4D;

        inline constexpr UInt16 OptionalHdr32 = 0x10b;
        inline constexpr UInt16 OptionalHdr64 = 0x20b;
    } // namespace MagicConst

    inline constexpr UInt32 MaxPEOffset          = 1024;
    inline constexpr UInt32 SectionShortNameSize = 8;

    struct DOSHeader
    {
        UInt16 Magic;
        UInt16 Cblp;
        UInt16 Cp;
        UInt16 Crlc;
        UInt16 Cparhdr;
        UInt16 Minalloc;
        UInt16 Maxalloc;
        UInt16 Ss;
        UInt16 Sp;
        UInt16 Csum;
        UInt16 Ip;
        UInt16 Cs;
        UInt16 Lfarlc;
        UInt16 Ovno;
        UInt16 Res[4];
        UInt16 Oemid;
        UInt16 Oeminfo;
        UInt16 Res2[10];
        UInt32 Lfanew;

        [[nodiscard]] inline bool IsValid() const noexcept
        {
            return Magic == MagicConst::DOS && Lfanew <= MaxPEOffset;
        }
    };

    struct FileHeader
    {
        UInt16 Machine;
        UInt16 NumberOfSections;
        UInt32 TimeDateStamp;
        UInt32 PointerToSymbolTable;
        UInt32 NumberOfSymbols;
        UInt16 SizeOfOptionalHeader;
        UInt16 Characteristics;
    };

    struct OptionalHeaderBase
    {
        UInt16 Magic;
        UInt8 MajorLinkerVersion;
        UInt8 MinorLinkerVersion;
        UInt32 SizeOfCode;
        UInt32 SizeOfInitializedData;
        UInt32 SizeOfUninitializedData;
        UInt32 AddressOfEntryPoint;
        UInt32 BaseOfCode;

        [[nodiscard]] inline ArchPointerSize GetArchPointerSize() const noexcept
        {
            switch (Magic)
            {
            case MagicConst::OptionalHdr32:
                return ArchPointerSize::Arch32Bit;
            case MagicConst::OptionalHdr64:
                return ArchPointerSize::Arch64Bit;
            default:
                return ArchPointerSize::Invalid;
            }
        }

        [[nodiscard]] inline static ArchPointerSize GetArchPointerSize(const void* pBuffer) noexcept
        {
            const auto* pHeader = static_cast<const OptionalHeaderBase*>(pBuffer);
            return pHeader->GetArchPointerSize();
        }
    };

    struct SectionHeader
    {
        char Name[SectionShortNameSize];
        UInt32 VirtualSize;
        PEVirtualAddress VirtualAddress;
        UInt32 SizeOfRawData;
        UInt32 PointerToRawData;
        UInt32 PointerToRelocations;
        UInt32 PointerToLinenumbers;
        UInt16 NumberOfRelocations;
        UInt16 NumberOfLinenumbers;
        SectionFlags Characteristics;
    };

    template<ArchPointerSize P>
    struct OptionalHeader
    {
    };

    template<>
    struct OptionalHeader<ArchPointerSize::Arch32Bit> : OptionalHeaderBase
    {
        UInt32 BaseOfData;
        UInt32 ImageBase;
        UInt32 SectionAlignment;
        UInt32 FileAlignment;
        UInt16 MajorOperatingSystemVersion;
        UInt16 MinorOperatingSystemVersion;
        UInt16 MajorImageVersion;
        UInt16 MinorImageVersion;
        UInt16 MajorSubsystemVersion;
        UInt16 MinorSubsystemVersion;
        UInt32 Win32VersionValue;
        UInt32 SizeOfImage;
        UInt32 SizeOfHeaders;
        UInt32 CheckSum;
        UInt16 Subsystem;
        UInt16 DllCharacteristics;
        UInt32 SizeOfStackReserve;
        UInt32 SizeOfStackCommit;
        UInt32 SizeOfHeapReserve;
        UInt32 SizeOfHeapCommit;
        UInt32 LoaderFlags;
        UInt32 NumberOfRvaAndSizes;
        DataDirectory DataDirectory[ac_enum_cast(DirectoryEntryID::Count)];
    };

    template<>
    struct OptionalHeader<ArchPointerSize::Arch64Bit> : OptionalHeaderBase
    {
        UInt64 ImageBase;
        UInt32 SectionAlignment;
        UInt32 FileAlignment;
        UInt16 MajorOperatingSystemVersion;
        UInt16 MinorOperatingSystemVersion;
        UInt16 MajorImageVersion;
        UInt16 MinorImageVersion;
        UInt16 MajorSubsystemVersion;
        UInt16 MinorSubsystemVersion;
        UInt32 Win32VersionValue;
        UInt32 SizeOfImage;
        UInt32 SizeOfHeaders;
        UInt32 CheckSum;
        UInt16 Subsystem;
        UInt16 DllCharacteristics;
        UInt64 SizeOfStackReserve;
        UInt64 SizeOfStackCommit;
        UInt64 SizeOfHeapReserve;
        UInt64 SizeOfHeapCommit;
        UInt32 LoaderFlags;
        UInt32 NumberOfRvaAndSizes;
        DataDirectory DataDirectory[ac_enum_cast(DirectoryEntryID::Count)];
    };

    template<ArchPointerSize P>
    struct NTHeader;

    struct NTHeaderBase
    {
        UInt32 Signature;
        FileHeader FileHeader;

        template<ArchPointerSize P>
        inline NTHeader<P>* As() noexcept
        {
            return static_cast<NTHeader<P>*>(this);
        }

        template<ArchPointerSize P>
        inline const NTHeader<P>* As() const noexcept
        {
            return static_cast<const NTHeader<P>*>(this);
        }

        [[nodiscard]] inline OptionalHeaderBase* GetOptionalHeader() noexcept;

        [[nodiscard]] inline ArchPointerSize GetArchPointerSize() const noexcept;

        [[nodiscard]] inline UInt32 GetImageSize() const noexcept;

        [[nodiscard]] inline UInt32 GetHeadersSize() const noexcept;

        [[nodiscard]] inline UInt64 GetImageBase() const noexcept;

        [[nodiscard]] List<SectionHeader*> GetSectionHeaders() noexcept;

        [[nodiscard]] inline DataDirectory* GetDirectory() noexcept;

        [[nodiscard]] inline PEVirtualAddress GetDirectoryEntry(DirectoryEntryID id) noexcept
        {
            auto* pEntry = GetDirectory() + ac_enum_cast(id);
            return pEntry->VirtualAddress;
        }

        [[nodiscard]] inline ExportDirectory* GetExportDirectoryEntry(ActiasHandle imageHandle) noexcept
        {
            auto va = GetDirectoryEntry(DirectoryEntryID::Export);
            return va.ToPointer<ExportDirectory>(imageHandle);
        }

        [[nodiscard]] inline ImportDescriptor* GetImportDescriptors(ActiasHandle imageHandle) noexcept
        {
            auto va = GetDirectoryEntry(DirectoryEntryID::Import);
            return va.ToPointer<ImportDescriptor>(imageHandle);
        }

        [[nodiscard]] inline BaseRelocation* GetBaseRelocations(ActiasHandle imageHandle) noexcept
        {
            auto va = GetDirectoryEntry(DirectoryEntryID::BaseReloc);
            return va.ToPointer<BaseRelocation>(imageHandle);
        }

        [[nodiscard]] inline bool Is64Bit() const noexcept
        {
            return GetArchPointerSize() == ArchPointerSize::Arch64Bit;
        }

        [[nodiscard]] inline bool IsValid() const noexcept
        {
            return Signature == MagicConst::NT;
        }
    };

    template<ArchPointerSize P>
    struct NTHeader : NTHeaderBase
    {
        OptionalHeader<P> OptionalHeader;
    };

    using NTHeader32 = NTHeader<ArchPointerSize::Arch32Bit>;
    using NTHeader64 = NTHeader<ArchPointerSize::Arch64Bit>;

    inline OptionalHeaderBase* NTHeaderBase::GetOptionalHeader() noexcept
    {
        return &As<ArchPointerSize::Arch32Bit>()->OptionalHeader;
    }

    inline ArchPointerSize NTHeaderBase::GetArchPointerSize() const noexcept
    {
        return As<ArchPointerSize::Arch32Bit>()->OptionalHeader.GetArchPointerSize();
    }

#define ACTIAS_NT_OPTIONAL_ENTRY(Field)                                                                                          \
    Is64Bit() ? As<ArchPointerSize::Arch64Bit>()->OptionalHeader.Field : As<ArchPointerSize::Arch32Bit>()->OptionalHeader.Field

    inline UInt32 NTHeaderBase::GetImageSize() const noexcept
    {
        return ACTIAS_NT_OPTIONAL_ENTRY(SizeOfImage);
    }

    inline UInt32 NTHeaderBase::GetHeadersSize() const noexcept
    {
        return ACTIAS_NT_OPTIONAL_ENTRY(SizeOfHeaders);
    }

    inline UInt64 NTHeaderBase::GetImageBase() const noexcept
    {
        return ACTIAS_NT_OPTIONAL_ENTRY(ImageBase);
    }

    inline DataDirectory* NTHeaderBase::GetDirectory() noexcept
    {
        return ACTIAS_NT_OPTIONAL_ENTRY(DataDirectory);
    }

#undef ACTIAS_NT_OPTIONAL_ENTRY

    ExecutableParseResult<NTHeaderBase*> ParseNTHeaders(const ArraySlice<Byte>& buffer);

    bool IsWindowsPEFile(const ArraySlice<Byte>& buffer);
} // namespace Actias::SDK::PE
