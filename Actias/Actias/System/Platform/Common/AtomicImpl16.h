#pragma once
#include <Actias/System/Platform/Common/AtomicImpl.h>

ACTIAS_BEGIN_C

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicLoad16Explicit(const ActiasAtomic16* pSource, ActiasMemoryOrder memoryOrder)
{
    UInt16 result = ActiasAtomicLoad16Relaxed(pSource);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicLoad16(const ActiasAtomic16* pSource)
{
    return ActiasAtomicLoad16Explicit(pSource, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore16Explicit(ActiasAtomic16* pDestination, UInt16 source,
                                                            ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    ActiasAtomicStore16Relaxed(pDestination, source);
}

ACTIAS_FORCE_INLINE static void ActiasAtomicStore16(ActiasAtomic16* pDestination, UInt16 source)
{
    ActiasAtomicStore16Explicit(pDestination, source, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicFetchAdd16Explicit(ActiasAtomic16* pDestination, Int16 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt16 result = ActiasAtomicFetchAdd16Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicFetchAdd16(ActiasAtomic16* pDestination, Int16 value)
{
    return ActiasAtomicFetchAdd16Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicFetchAnd16Explicit(ActiasAtomic16* pDestination, UInt16 value,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt16 result = ActiasAtomicFetchAnd16Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicFetchAnd16(ActiasAtomic16* pDestination, UInt16 value)
{
    return ActiasAtomicFetchAnd16Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicFetchOr16Explicit(ActiasAtomic16* pDestination, UInt16 value,
                                                                ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt16 result = ActiasAtomicFetchOr16Relaxed(pDestination, value);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicFetchOr16(ActiasAtomic16* pDestination, UInt16 value)
{
    return ActiasAtomicFetchOr16Explicit(pDestination, value, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicExchange16Explicit(ActiasAtomic16* pDestination, UInt16 desired,
                                                                 ActiasMemoryOrder memoryOrder)
{
    ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder);
    UInt16 result = ActiasAtomicExchange16Relaxed(pDestination, desired);
    ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder);
    return result;
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicExchange16(ActiasAtomic16* pDestination, UInt16 desired)
{
    return ActiasAtomicExchange16Explicit(pDestination, desired, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchageWeak16Explicit(ActiasAtomic16* pDestination, UInt16* pExpected,
                                                                               UInt16 desired,
                                                                               ActiasMemoryOrder successMemoryOrder,
                                                                               ActiasMemoryOrder failureMemoryOrder)
{
    if (successMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || successMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL
        || failureMemoryOrder == ACTIAS_MEMORY_ORDER_RELEASE || failureMemoryOrder == ACTIAS_MEMORY_ORDER_ACQ_REL)
    {
        ACTIAS_ThreadFenceRelease();
    }

    ActiasBool result = ActiasAtomicCompareExchangeWeak16Relaxed(pDestination, pExpected, desired);
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

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchageWeak16(ActiasAtomic16* pDestination, UInt16* pExpected,
                                                                       UInt16 desired)
{
    return ActiasAtomicCompareExchageWeak16Explicit(
        pDestination, pExpected, desired, ACTIAS_MEMORY_ORDER_SEQ_CST, ACTIAS_MEMORY_ORDER_SEQ_CST);
}

ACTIAS_END_C
