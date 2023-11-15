#pragma once
#include <Actias/System/Platform/Common/AtomicImpl.h>

ACTIAS_BEGIN_C

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicLoad64Explicit(const ActiasAtomic64* pSource, ActiasMemoryOrder memoryOrder)
{
    UInt64 result = ActiasAtomicLoad64Relaxed(pSource);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore64Explicit(ActiasAtomic64* pDestination, UInt64 source,
                                                            ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    ActiasAtomicStore64Relaxed(pDestination, source);
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchAdd64Explicit(ActiasAtomic64* pDestination, Int64 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicFetchAdd64Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchAnd64Explicit(ActiasAtomic64* pDestination, UInt64 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicFetchAnd64Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicFetchOr64Explicit(ActiasAtomic64* pDestination, UInt64 value,
                                                                ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicFetchOr64Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicExchange64Explicit(ActiasAtomic64* pDestination, UInt64 desired,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt64 result = ActiasAtomicExchange64Relaxed(pDestination, desired);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
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

ACTIAS_END_C
