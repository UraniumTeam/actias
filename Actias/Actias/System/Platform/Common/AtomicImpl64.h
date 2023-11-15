#pragma once
#include <Actias/System/Platform/Common/AtomicImpl.h>

ACTIAS_BEGIN_C

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicLoad64Explicit(const ActiasAtomic64* pSource, ActiasMemoryOrder memoryOrder)
{
    UInt64 result = ActiasAtomicLoad64Relaxed(pSource);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicLoad64(const ActiasAtomic64* pSource)
{
    return ActiasAtomicLoad64Explicit(pSource, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore64Explicit(ActiasAtomic64* pDestination, UInt64 source,
                                                            ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    ActiasAtomicStore64Relaxed(pDestination, source);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore64(ActiasAtomic64* pDestination, UInt64 source)
{
    ActiasAtomicStore64Explicit(pDestination, source, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchAdd64Explicit(ActiasAtomic64* pDestination, Int64 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicFetchAdd64Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchAdd64(ActiasAtomic64* pDestination, Int64 value)
{
    return ActiasAtomicFetchAdd64Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchAnd64Explicit(ActiasAtomic64* pDestination, UInt64 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicFetchAnd64Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchAnd64(ActiasAtomic64* pDestination, UInt64 value)
{
    return ActiasAtomicFetchAnd64Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchOr64Explicit(ActiasAtomic64* pDestination, UInt64 value,
                                                                ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicFetchOr64Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchOr64(ActiasAtomic64* pDestination, UInt64 value)
{
    return ActiasAtomicFetchOr64Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicExchange64Explicit(ActiasAtomic64* pDestination, UInt64 desired,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicExchange64Relaxed(pDestination, desired);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicExchange64(ActiasAtomic64* pDestination, UInt64 desired)
{
    return ActiasAtomicExchange64Explicit(pDestination, desired, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchageWeak64Explicit(ActiasAtomic64* pDestination, UInt64* pExpected,
                                                                               UInt64 desired,
                                                                               ActiasMemoryOrder successMemoryOrder,
                                                                               ActiasMemoryOrder failureMemoryOrder)
{
    if (successMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || successMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL
        || failureMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || failureMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL)
    {
        ACTIAS_ThreadFenceRelease();
    }

    ActiasBool result = ActiasAtomicCompareExchangeWeak64Relaxed(pDestination, pExpected, desired);
    if (result)
    {
        ACTIAS_PlaceAquireFenceIfNeeded(successMemoryOrder);
    }
    else
    {
        ACTIAS_PlaceAquireFenceIfNeeded(failureMemoryOrder);
    }

    return result;
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchageWeak64(ActiasAtomic64* pDestination, UInt64* pExpected,
                                                                       UInt64 desired)
{
    return ActiasAtomicCompareExchageWeak64Explicit(
        pDestination, pExpected, desired, ACTIAS_MEMORY_ORDER_SEQ_CST, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_END_C
