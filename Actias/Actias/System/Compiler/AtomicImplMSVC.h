#pragma once
#include <Actias/System/Base.h>
#include <intrin.h>

ACTIAS_BEGIN_C

#define ACTIAS_MAKE_TOMIC_TYPE(bits)                                                                                             \
    typedef struct ActiasAtomic##bits                                                                                            \
    {                                                                                                                            \
        UInt##bits Data;                                                                                                         \
    } ActiasAtomic##bits

ACTIAS_MAKE_TOMIC_TYPE(8);
ACTIAS_MAKE_TOMIC_TYPE(16);
ACTIAS_MAKE_TOMIC_TYPE(32);
ACTIAS_MAKE_TOMIC_TYPE(64);

#undef ACTIAS_MAKE_TOMIC_TYPE

#define ACTIAS_VolatileLoad(value, bits) ((volatile ActiasAtomic##bits*)value)->Data

//! \brief Consume compiler fence.
#define ACTIAS_CompilerFenceConsume()                                                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
    }                                                                                                                            \
    while (0)

//! \brief Acquire compiler fence.
#define ACTIAS_CompilerFenceAcquire _ReadWriteBarrier

//! \brief Release compiler fence.
#define ACTIAS_CompilerFenceRelease _ReadWriteBarrier

//! \brief Sequential consistency compiler fence.
#define ACTIAS_CompilerFenceSeqCst _ReadWriteBarrier

//! \brief Consume thread fence.
#define ACTIAS_ThreadFenceConsume()                                                                                              \
    do                                                                                                                           \
    {                                                                                                                            \
    }                                                                                                                            \
    while (0)

//! \brief Acquire thread fence.
#define ACTIAS_ThreadFenceAcquire _ReadWriteBarrier

//! \brief Release thread fence.
#define ACTIAS_ThreadFenceRelease _ReadWriteBarrier

//! \brief Sequential consistency thread fence.
#define ACTIAS_ThreadFenceSeqCst __faststorefence

/*
 * 8 Bit operations
 */

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicLoad8Relaxed(const ActiasAtomic8* pSource)
{
    return ACTIAS_VolatileLoad(pSource, 8);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore8Relaxed(ActiasAtomic8* pDestination, UInt8 source)
{
    ACTIAS_VolatileLoad(pDestination, 8) = source;
}

ACTIAS_FORCE_INLINE UInt8 ActiasAtomicFetchAdd8Relaxed(ActiasAtomic8* pDestination, Int8 value)
{
    return _InterlockedExchangeAdd8((volatile char*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt8 ActiasAtomicFetchAnd8Relaxed(ActiasAtomic8* pDestination, UInt8 value)
{
    return _InterlockedAnd8((volatile char*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt8 ActiasAtomicFetchOr8Relaxed(ActiasAtomic8* pDestination, UInt8 value)
{
    return _InterlockedOr8((volatile char*)pDestination, value);
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicExchange8Relaxed(ActiasAtomic8* pDestination, UInt8 desired)
{
    return _InterlockedExchange8((volatile char*)pDestination, desired);
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicCompareExchange8Relaxed(ActiasAtomic8* pDestination, UInt8 expected, UInt8 desired)
{
    return (UInt8)_InterlockedCompareExchange8((volatile char*)pDestination, desired, expected);
}

inline static ActiasBool ActiasAtomicCompareExchangeWeak8Relaxed(ActiasAtomic8* pDestination, UInt8* expected, UInt8 desired)
{
    UInt8 e           = *expected;
    UInt8 previous    = _InterlockedCompareExchange8((volatile char*)pDestination, desired, e);
    ActiasBool result = (previous == e);
    if (!result)
    {
        *expected = previous;
    }

    return result;
}

/*
 * 16 Bit operations
 */

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicLoad16Relaxed(const ActiasAtomic16* pSource)
{
    return ACTIAS_VolatileLoad(pSource, 16);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore16Relaxed(ActiasAtomic16* pDestination, UInt16 source)
{
    ACTIAS_VolatileLoad(pDestination, 16) = source;
}

ACTIAS_FORCE_INLINE UInt16 ActiasAtomicFetchAdd16Relaxed(ActiasAtomic16* pDestination, Int16 value)
{
    return _InterlockedExchangeAdd16((volatile short*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt16 ActiasAtomicFetchAnd16Relaxed(ActiasAtomic16* pDestination, UInt16 value)
{
    return _InterlockedAnd16((volatile short*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt16 ActiasAtomicFetchOr16Relaxed(ActiasAtomic16* pDestination, UInt16 value)
{
    return _InterlockedOr16((volatile short*)pDestination, value);
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicExchange16Relaxed(ActiasAtomic16* pDestination, UInt16 desired)
{
    return _InterlockedExchange16((volatile short*)pDestination, desired);
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicCompareExchange16Relaxed(ActiasAtomic16* pDestination, UInt16 expected,
                                                                       UInt16 desired)
{
    return (UInt16)_InterlockedCompareExchange16((volatile short*)pDestination, desired, expected);
}

inline static ActiasBool ActiasAtomicCompareExchangeWeak16Relaxed(ActiasAtomic16* pDestination, UInt16* expected, UInt16 desired)
{
    UInt16 e          = *expected;
    UInt16 previous   = _InterlockedCompareExchange16((volatile short*)pDestination, desired, e);
    ActiasBool result = (previous == e);
    if (!result)
    {
        *expected = previous;
    }

    return result;
}

/*
 * 32 Bit operations
 */

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicLoad32Relaxed(const ActiasAtomic32* pSource)
{
    return ACTIAS_VolatileLoad(pSource, 32);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore32Relaxed(ActiasAtomic32* pDestination, UInt32 source)
{
    ACTIAS_VolatileLoad(pDestination, 32) = source;
}

ACTIAS_FORCE_INLINE UInt32 ActiasAtomicFetchAdd32Relaxed(ActiasAtomic32* pDestination, Int32 value)
{
    return _InterlockedExchangeAdd((volatile long*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt32 ActiasAtomicFetchAnd32Relaxed(ActiasAtomic32* pDestination, UInt32 value)
{
    return _InterlockedAnd((volatile long*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt32 ActiasAtomicFetchOr32Relaxed(ActiasAtomic32* pDestination, UInt32 value)
{
    return _InterlockedOr((volatile long*)pDestination, value);
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicExchange32Relaxed(ActiasAtomic32* pDestination, UInt32 desired)
{
    return _InterlockedExchange((volatile long*)pDestination, desired);
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicCompareExchange32Relaxed(ActiasAtomic32* pDestination, UInt32 expected,
                                                                       UInt32 desired)
{
    return (UInt32)_InterlockedCompareExchange((volatile long*)pDestination, desired, expected);
}

inline static ActiasBool ActiasAtomicCompareExchangeWeak32Relaxed(ActiasAtomic32* pDestination, UInt32* expected, UInt32 desired)
{
    UInt32 e          = *expected;
    UInt32 previous   = _InterlockedCompareExchange((volatile long*)pDestination, desired, e);
    ActiasBool result = (previous == e);
    if (!result)
    {
        *expected = previous;
    }

    return result;
}

#if ACTIAS_ARCH_64_BIT

/*
 * 64 Bit operations
 */

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicLoad64Relaxed(const ActiasAtomic64* pSource)
{
    return ACTIAS_VolatileLoad(pSource, 64);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore64Relaxed(ActiasAtomic64* pDestination, UInt64 source)
{
    ACTIAS_VolatileLoad(pDestination, 64) = source;
}

ACTIAS_FORCE_INLINE UInt64 ActiasAtomicFetchAdd64Relaxed(ActiasAtomic64* pDestination, Int64 value)
{
    return _InterlockedExchangeAdd64((volatile long long*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt64 ActiasAtomicFetchAnd64Relaxed(ActiasAtomic64* pDestination, UInt64 value)
{
    return _InterlockedAnd64((volatile long long*)pDestination, value);
}

ACTIAS_FORCE_INLINE UInt64 ActiasAtomicFetchOr64Relaxed(ActiasAtomic64* pDestination, UInt64 value)
{
    return _InterlockedOr64((volatile long long*)pDestination, value);
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicExchange64Relaxed(ActiasAtomic64* pDestination, UInt64 desired)
{
    return _InterlockedExchange64((volatile long long*)pDestination, desired);
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicCompareExchange64Relaxed(ActiasAtomic64* pDestination, UInt64 expected,
                                                                       UInt64 desired)
{
    return (UInt64)_InterlockedCompareExchange64((volatile long long*)pDestination, desired, expected);
}

inline static ActiasBool ActiasAtomicCompareExchangeWeak64Relaxed(ActiasAtomic64* pDestination, UInt64* expected, UInt64 desired)
{
    UInt64 e          = *expected;
    UInt64 previous   = _InterlockedCompareExchange64((volatile long long*)pDestination, desired, e);
    ActiasBool result = (previous == e);
    if (!result)
    {
        *expected = previous;
    }

    return result;
}

#endif

ACTIAS_END_C
