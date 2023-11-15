#pragma once
#include <Actias/System/Platform/Common/AtomicImpl.h>

ACTIAS_BEGIN_C

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicLoad8Explicit(const ActiasAtomic8* pSource, ActiasMemoryOrder memoryOrder)
{
    UInt8 result = ActiasAtomicLoad8Relaxed(pSource);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore8Explicit(ActiasAtomic8* pDestination, UInt8 source,
                                                           ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    ActiasAtomicStore8Relaxed(pDestination, source);
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicFetchAdd8Explicit(ActiasAtomic8* pDestination, Int8 value,
                                                               ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt8 result = ActiasAtomicFetchAdd8Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicFetchAnd8Explicit(ActiasAtomic8* pDestination, UInt8 value,
                                                               ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt8 result = ActiasAtomicFetchAnd8Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicFetchOr8Explicit(ActiasAtomic8* pDestination, UInt8 value,
                                                              ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt8 result = ActiasAtomicFetchOr8Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicExchange8Explicit(ActiasAtomic8* pDestination, UInt8 desired,
                                                               ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt8 result = ActiasAtomicExchange8Relaxed(pDestination, desired);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchageWeak8Explicit(ActiasAtomic8* pDestination, UInt8* pExpected,
                                                                              UInt8 desired, ActiasMemoryOrder successMemoryOrder,
                                                                              ActiasMemoryOrder failureMemoryOrder)
{
    if (successMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || successMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL
        || failureMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || failureMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL)
    {
        ACTIAS_ThreadFenceRelease();
    }

    ActiasBool result = ActiasAtomicCompareExchangeWeak8Relaxed(pDestination, pExpected, desired);
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
