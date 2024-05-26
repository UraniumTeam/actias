#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Base/Byte.hpp>
#include <Actias/Containers/ArraySlice.hpp>

namespace Actias::SDK::ELF
{
    namespace MagicConst
    {
        inline constexpr UInt32 Class64 = 2;
        inline constexpr UInt8 Mag[4]   = { 0x7f, 'E', 'L', 'F' };
    } // namespace MagicConst

    namespace IdentIndex
    {
        inline constexpr UInt32 Class   = 4;
        inline constexpr UInt32 Data    = 5;
        inline constexpr UInt32 Version = 6;
        inline constexpr UInt32 OSABI   = 7;
        inline constexpr UInt32 Pad     = 8;
    } // namespace IdentIndex

    struct ELFHeader
    {
        UInt8 Ident[16];
        UInt16 Type;
        UInt16 Machine;
        UInt32 Version;
        UInt64 Entry;
        UInt64 Phoff;
        UInt64 Shoff;
        UInt32 Flags;
        UInt16 Ehsize;
        UInt16 Phentsize;
        UInt16 Phnum;
        UInt16 Shentsize;
        UInt16 Shnum;
        UInt16 Shstrndx;

        [[nodiscard]] inline bool IsValid() const noexcept
        {
            for (UInt32 identIndex = 0; identIndex < 4; ++identIndex)
            {
                if (Ident[identIndex] != MagicConst::Mag[identIndex])
                    return false;
            }

            return Ident[IdentIndex::Class] == MagicConst::Class64;
        }
    };

    enum class SegmentFlags : UInt32
    {
        Read    = 0x4,
        Write   = 0x2,
        Execute = 0x1,
    };

    ACTIAS_ENUM_OPERATORS(SegmentFlags);

    struct ProgramHeader
    {
        UInt32 Type;
        SegmentFlags Flags;
        UInt64 Offset;
        UInt64 Vaddr;
        UInt64 Paddr;
        UInt64 Filesz;
        UInt64 Memsz;
        UInt64 Align;

        inline static ProgramHeader* Find(ELFHeader* elfHeader)
        {
            Byte* pByte = reinterpret_cast<Byte*>(elfHeader);
            return reinterpret_cast<ProgramHeader*>(pByte + elfHeader->Phoff);
        }
    };

    enum class SectionType : UInt32
    {
        Null            = 0,
        ProgramData     = 1,
        SymbolTable     = 2,
        StringTable     = 3,
        Rela            = 4,
        SymbolHashTable = 5,
        Dynamic         = 6,
        Note            = 7,
        NoBits          = 8,
        Rel             = 9,
        DynSymbolTable  = 11,
        Count           = 12,
    };

    struct SectionHeader
    {
        UInt32 Name;
        SectionType Type;
        UInt64 Flags;
        UInt64 Addr;
        UInt64 Offset;
        UInt64 Size;
        UInt32 Link;
        UInt32 Info;
        UInt64 Addralign;
        UInt64 Entsize;

        inline static SectionHeader* Find(ELFHeader* elfHeader)
        {
            Byte* pByte = reinterpret_cast<Byte*>(elfHeader);
            return reinterpret_cast<SectionHeader*>(pByte + elfHeader->Shoff);
        }
    };

    enum class SymbolVisibility
    {
        Default   = 0,
        Internal  = 1,
        Hidden    = 2,
        Protected = 3,
    };

    struct Symbol
    {
        UInt32 Name;
        UInt8 Info;
        SymbolVisibility Visibility;
        UInt16 Index;
        UInt64 Value;
        UInt64 Size;
    };

    bool IsUnixELFFile(const ArraySlice<Byte>& buffer);
} // namespace Actias::SDK::ELF
