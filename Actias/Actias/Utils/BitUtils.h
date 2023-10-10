#pragma once
#include <Actias/Base/Base.h>

namespace Actias::Bits
{
#ifdef ACTIAS_COMPILER_MSVC

    ACTIAS_FORCE_INLINE UInt32 CountTrailingZeros(UInt32 value) noexcept
    {
        unsigned long tz = 0;
        return _BitScanForward(&tz, value) ? tz : 32;
    }

    ACTIAS_FORCE_INLINE UInt32 CountLeadingZeros(UInt32 value) noexcept
    {
        unsigned long lz = 0;
        return _BitScanReverse(&lz, value) ? 31 - lz : 32;
    }

#else

    ACTIAS_FORCE_INLINE UInt32 CountTrailingZeros(UInt32 value) noexcept
    {
        return __builtin_ctz(value);
    }

    ACTIAS_FORCE_INLINE UInt32 CountLeadingZeros(UInt32 value) noexcept
    {
        return __builtin_clz(value);
    }

#endif
} // namespace Actias
