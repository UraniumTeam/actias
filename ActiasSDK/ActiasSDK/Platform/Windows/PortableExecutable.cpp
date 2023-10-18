#include <ActiasSDK/Platform/Windows/PortableExecutable.h>

namespace Actias::SDK::PE
{
    ExecutableParseResult<INativeExecutable*> PortableExecutable::Load(const ArraySlice<Byte>& rawBuffer)
    {
        auto* pResult = AllocateObject<PortableExecutable>();

        auto mapResult = PEModuleBuffer::MapToVirtual(rawBuffer);
        ACTIAS_GuardResult(mapResult);

        pResult->m_pVirtualBuffer = mapResult.Unwrap();
        return pResult;
    }
} // namespace Actias::SDK::PE
