#pragma once
#include <Actias/Base/Base.h>
#include <Actias/Base/Byte.h>
#include <Actias/System/Core.h>

namespace Actias::SDK::PE
{
    enum class DirectoryEntryID
    {
        Export        = 0,  //!< Export Directory
        Import        = 1,  //!< Import Directory
        Resource      = 2,  //!< Resource Directory
        Exception     = 3,  //!< Exception Directory
        Security      = 4,  //!< Security Directory
        BaseReloc     = 5,  //!< Base Relocation Table
        Debug         = 6,  //!< Debug Directory
        Copyright     = 7,  //!< (X86 usage)
        Architecture  = 7,  //!< Architecture Specific Data
        GlobalPtr     = 8,  //!< RVA of GP
        TLS           = 9,  //!< TLS Directory
        LoadConfig    = 10, //!< Load Configuration Directory
        BoundImport   = 11, //!< Bound Import Directory in headers
        IAT           = 12, //!< Import Address Table
        DelayImport   = 13, //!< Delay Load Import Descriptors
        COMDescriptor = 14, //!< COM Runtime descriptor

        Count = 16
    };

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
            auto* bytes = ac_byte_cast(pImageBuffer) + Value;
            return reinterpret_cast<T*>(bytes);
        }
    };
} // namespace Actias::SDK::PE
