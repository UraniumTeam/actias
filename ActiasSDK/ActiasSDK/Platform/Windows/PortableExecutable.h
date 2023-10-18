#pragma once
#include <Actias/Base/Byte.h>
#include <Actias/Containers/ArraySlice.h>
#include <ActiasSDK/Platform/INativeExecutable.h>
#include <ActiasSDK/Platform/Windows/Parser/PEModuleBuffer.h>

namespace Actias::SDK::PE
{
    class PortableExecutable : public Object<INativeExecutable>
    {
        Ptr<PEModuleBuffer> m_pVirtualBuffer;

    public:
        ACTIAS_RTTI_Class(PortableExecutable, "8DC33AFD-F7A2-4313-9C54-3ACD23332D05");

        static ExecutableParseResult<INativeExecutable*> Load(const ArraySlice<Byte>& rawBuffer);
    };
} // namespace Actias::SDK::PE
