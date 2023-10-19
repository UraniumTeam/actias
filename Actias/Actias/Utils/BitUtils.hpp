#pragma once
#include <Actias/Base/Base.hpp>

namespace Actias::Bits
{
    //! \brief Count trailing zeros in a number.
    //! 
    //! Uses a compiler-specific function that does not import any symbols,
    //! even on debug builds without optimizations.
    //! 
    //! \param value - The number to count trailing zeros in.
    //! 
    //! \return The number of trailing zeros in the provided value.
    ACTIAS_FORCE_INLINE UInt32 CountTrailingZeros(UInt32 value) noexcept
    {
#ifdef ACTIAS_COMPILER_MSVC
        unsigned long tz = 0;
        return _BitScanForward(&tz, value) ? tz : 32;
#else
        return __builtin_ctz(value);
#endif
    }

    //! \brief Count leading zeros in a number.
    //!
    //! Uses a compiler-specific function that does not import any symbols,
    //! even on debug builds without optimizations.
    //!
    //! \param value - The number to count leading zeros in.
    //!
    //! \return The number of leading zeros in the provided value.
    ACTIAS_FORCE_INLINE UInt32 CountLeadingZeros(UInt32 value) noexcept
    {
#ifdef ACTIAS_COMPILER_MSVC
        unsigned long lz = 0;
        return _BitScanReverse(&lz, value) ? 31 - lz : 32;
#else
        return __builtin_clz(value);
#endif
    }
} // namespace Actias::Bits
