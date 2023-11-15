#pragma once
#include <Actias/System/Platform/Common/AtomicImpl.h>

ACTIAS_BEGIN_C

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicLoad32Explicit(const ActiasAtomic32* pSource, ActiasMemoryOrder memoryOrder)
{
    UInt32 result = ActiasAtomicLoad32Relaxed(pSource);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicLoad32(const ActiasAtomic32* pSource)
{
    return ActiasAtomicLoad32Explicit(pSource, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore32Explicit(ActiasAtomic32* pDestination, UInt32 source,
                                                            ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    ActiasAtomicStore32Relaxed(pDestination, source);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore32(ActiasAtomic32* pDestination, UInt32 source)
{
    ActiasAtomicStore32Explicit(pDestination, source, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicFetchAdd32Explicit(ActiasAtomic32* pDestination, Int32 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt32 result = ActiasAtomicFetchAdd32Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicFetchAdd32(ActiasAtomic32* pDestination, Int32 value)
{
    return ActiasAtomicFetchAdd32Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicFetchAnd32Explicit(ActiasAtomic32* pDestination, UInt32 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt32 result = ActiasAtomicFetchAnd32Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicFetchAnd32(ActiasAtomic32* pDestination, UInt32 value)
{
    return ActiasAtomicFetchAnd32Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicFetchOr32Explicit(ActiasAtomic32* pDestination, UInt32 value,
                                                                ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt32 result = ActiasAtomicFetchOr32Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicFetchOr32(ActiasAtomic32* pDestination, UInt32 value)
{
    return ActiasAtomicFetchOr32Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicExchange32Explicit(ActiasAtomic32* pDestination, UInt32 desired,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt32 result = ActiasAtomicExchange32Relaxed(pDestination, desired);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicExchange32(ActiasAtomic32* pDestination, UInt32 desired)
{
    return ActiasAtomicExchange32Explicit(pDestination, desired, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchageWeak32Explicit(ActiasAtomic32* pDestination, UInt32* pExpected,
                                                                               UInt32 desired,
                                                                               ActiasMemoryOrder successMemoryOrder,
                                                                               ActiasMemoryOrder failureMemoryOrder)
{
    if (successMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || successMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL
        || failureMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || failureMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL)
    {
        ACTIAS_ThreadFenceRelease();
    }

    ActiasBool result = ActiasAtomicCompareExchangeWeak32Relaxed(pDestination, pExpected, desired);
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

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchageWeak32(ActiasAtomic32* pDestination, UInt32* pExpected,
                                                                       UInt32 desired)
{
    return ActiasAtomicCompareExchageWeak32Explicit(
        pDestination, pExpected, desired, ACTIAS_MEMORY_ORDER_SEQ_CST, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_END_C
