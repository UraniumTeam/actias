#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Base/Byte.hpp>

namespace Actias::SDK::PE
{
    struct ExportDirectory
    {
        UInt32 Characteristics;
        UInt32 TimeDateStamp;
        UInt16 MajorVersion;
        UInt16 MinorVersion;
        UInt32 Name;
        UInt32 Base;
        UInt32 NumberOfFunctions;
        UInt32 NumberOfNames;
        UInt32 AddressOfFunctions;    //!< RVA from base of image
        UInt32 AddressOfNames;        //!< RVA from base of image
        UInt32 AddressOfNameOrdinals; //!< RVA from base of image
    };
} // namespace Actias::SDK::PE
