#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Base/Byte.hpp>
#include <Actias/System/Core.h>

namespace Actias::SDK::PE
{
    enum class DirectoryEntryID
    {
        Export        = 0,  //!< Export Directory.
        Import        = 1,  //!< Import Directory.
        Resource      = 2,  //!< Resource Directory.
        Exception     = 3,  //!< Exception Directory.
        Security      = 4,  //!< Security Directory.
        BaseReloc     = 5,  //!< Base Relocation Table.
        Debug         = 6,  //!< Debug Directory.
        Copyright     = 7,  //!< Copyright (X86 usage).
        Architecture  = 7,  //!< Architecture Specific Data.
        GlobalPtr     = 8,  //!< RVA of GP.
        TLS           = 9,  //!< TLS Directory.
        LoadConfig    = 10, //!< Load Configuration Directory.
        BoundImport   = 11, //!< Bound Import Directory in headers.
        IAT           = 12, //!< Import Address Table.
        DelayImport   = 13, //!< Delay Load Import Descriptors.
        COMDescriptor = 14, //!< COM Runtime descriptor.

        Count = 16
    };

    enum class SectionFlags : UInt32
    {
        CntCode              = 0x00000020, //!< Section contains code.
        CntInitializedData   = 0x00000040, //!< Section contains initialized data.
        CntUninitializedData = 0x00000080, //!< Section contains uninitialized data.
        LnkInfo              = 0x00000200, //!< Section contains comments or some other type of information.
        LnkRemove            = 0x00000800, //!< Section contents will not become part of image.
        LnkComdat            = 0x00001000, //!< Section contents comdat.
        NoDeferSpecExc       = 0x00004000, //!< Reset speculative exceptions handling bits in the Tlb entries for this section.
        Gprel                = 0x00008000, //!< Section content can be accessed relative to Gp.
        MemFardata           = 0x00008000,
        MemPurgeable         = 0x00020000,
        Mem16Bit             = 0x00020000,
        MemLocked            = 0x00040000,
        MemPreload           = 0x00080000,
        LnkNrelocOvfl        = 0x01000000, //!< Section contains extended relocations.
        MemDiscardable       = 0x02000000, //!< Section can be discarded.
        MemNotCached         = 0x04000000, //!< Section is not cachable.
        MemNotPaged          = 0x08000000, //!< Section is not pageable.
        MemShared            = 0x10000000, //!< Section is shareable.
        MemExecute           = 0x20000000, //!< Section is executable.
        MemRead              = 0x40000000, //!< Section is readable.
        MemWrite             = 0x80000000  //!< Section is writeable.
    };

    ACTIAS_ENUM_OPERATORS(SectionFlags);

    struct PEVirtualAddress
    {
        UInt32 Value;

        inline static PEVirtualAddress Null() noexcept
        {
            return PEVirtualAddress{ 0 };
        }

        template<class T>
        inline T* ToPointer(ActiasHandle pImageBuffer) const noexcept
        {
            auto* bytes = reinterpret_cast<Byte*>(pImageBuffer) + Value;
            return reinterpret_cast<T*>(bytes);
        }
    };
} // namespace Actias::SDK::PE
