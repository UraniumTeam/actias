#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Base/Byte.hpp>
#include <ActiasSDK/Platform/Windows/Parser/PECommon.hpp>

namespace Actias::SDK::PE
{
    struct DataDirectory
    {
        PEVirtualAddress VirtualAddress;
        UInt32 Size;
    };

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
        UInt32 AddressOfFunctions;    //!< RVA from base of image.
        UInt32 AddressOfNames;        //!< RVA from base of image.
        UInt32 AddressOfNameOrdinals; //!< RVA from base of image.
    };

    struct ImportDescriptor
    {
        UInt32 OriginalFirstThunk; // RVA to original unbound IAT (PIMAGE_THUNK_DATA).
        UInt32 TimeDateStamp;
        UInt32 ForwarderChain;
        UInt32 Name;
        UInt32 FirstThunk; // RVA to IAT.
    };

    struct ImportByName
    {
        UInt16 Hint;
        char Name[1];
    };

    struct BaseRelocation
    {
        PEVirtualAddress VirtualAddress;
        UInt32 SizeOfBlock;
    };
} // namespace Actias::SDK::PE
