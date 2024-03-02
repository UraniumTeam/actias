#pragma once
#include <Actias/System/Base.h>

ACTIAS_BEGIN_C

#define ACTIAS_MAKE_ATOMIC_TYPE(bits)                                                                                            \
    typedef struct ActiasAtomic##bits                                                                                            \
    {                                                                                                                            \
        UInt##bits Data;                                                                                                         \
    } ActiasAtomic##bits

ACTIAS_MAKE_ATOMIC_TYPE(8);
ACTIAS_MAKE_ATOMIC_TYPE(16);
ACTIAS_MAKE_ATOMIC_TYPE(32);
ACTIAS_MAKE_ATOMIC_TYPE(64);

#undef ACTIAS_MAKE_ATOMIC_TYPE

#define ACTIAS_VolatileLoad(value, bits) ((volatile ActiasAtomic##bits*)value)->Data

#define ACTIAS_CompilerFenceConsume() (0)
#define ACTIAS_CompilerFenceAcquire() asm volatile("" ::: "memory")
#define ACTIAS_CompilerFenceRelease() asm volatile("" ::: "memory")
#define ACTIAS_CompilerFenceSeqCst() asm volatile("" ::: "memory")

#define ACTIAS_ThreadFenceConsume() (0)
#define ACTIAS_ThreadFenceAcquire() asm volatile("" ::: "memory")
#define ACTIAS_ThreadFenceRelease() asm volatile("" ::: "memory")

#if ACTIAS_ARCH_64_BIT
#    define ACTIAS_ThreadFenceSeqCst() asm volatile("lock; orl $0, (%%rsp)" ::: "memory")
#else
#    define ACTIAS_ThreadFenceSeqCst() asm volatile("lock; orl $0, (%%esp)" ::: "memory")
#endif

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
    UInt8 previous;
    asm volatile("lock; xaddb %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt8 ActiasAtomicFetchAnd8Relaxed(ActiasAtomic8* pDestination, UInt8 value)
{
    UInt8 previous;
    UInt8 temp;
    asm volatile("1:     movb    %1, %0\n"
                 "       movb    %0, %2\n"
                 "       andb    %3, %2\n"
                 "       lock; cmpxchgb %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt8 ActiasAtomicFetchOr8Relaxed(ActiasAtomic8* pDestination, UInt8 value)
{
    UInt8 previous;
    UInt8 temp;
    asm volatile("1:     movb    %1, %0\n"
                 "       movb    %0, %2\n"
                 "       orb     %3, %2\n"
                 "       lock; cmpxchgb %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicExchange8Relaxed(ActiasAtomic8* pDestination, UInt8 desired)
{
    UInt8 previous;
    asm volatile("xchgb %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(desired));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt8 ActiasAtomicCompareExchange8Relaxed(ActiasAtomic8* pDestination, UInt8 expected, UInt8 desired)
{
    UInt8 previous;
    asm volatile("lock; cmpxchgb %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(expected));
    return previous;
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchangeWeak8Relaxed(ActiasAtomic8* pDestination, UInt8* expected,
                                                                              UInt8 desired)
{
    UInt8 e = *expected;
    UInt8 previous;
    ActiasBool matched;
    asm volatile("lock; cmpxchgb %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
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
    UInt16 previous;
    asm volatile("lock; xaddw %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt16 ActiasAtomicFetchAnd16Relaxed(ActiasAtomic16* pDestination, UInt16 value)
{
    UInt16 previous;
    UInt16 temp;
    asm volatile("1:     movw    %1, %0\n"
                 "       movw    %0, %2\n"
                 "       andw    %3, %2\n"
                 "       lock; cmpxchgw %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt16 ActiasAtomicFetchOr16Relaxed(ActiasAtomic16* pDestination, UInt16 value)
{
    UInt16 previous;
    UInt16 temp;
    asm volatile("1:     movw    %1, %0\n"
                 "       movw    %0, %2\n"
                 "       orw     %3, %2\n"
                 "       lock; cmpxchgw %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicExchange16Relaxed(ActiasAtomic16* pDestination, UInt16 desired)
{
    UInt16 previous;
    asm volatile("xchgw %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(desired));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt16 ActiasAtomicCompareExchange16Relaxed(ActiasAtomic16* pDestination, UInt16 expected,
                                                                       UInt16 desired)
{
    UInt16 previous;
    asm volatile("lock; cmpxchgw %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(expected));
    return previous;
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchangeWeak16Relaxed(ActiasAtomic16* pDestination, UInt16* expected,
                                                                               UInt16 desired)
{
    UInt16 e = *expected;
    UInt16 previous;
    ActiasBool matched;
    asm volatile("lock; cmpxchgw %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
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
    UInt32 previous;
    asm volatile("lock; xaddl %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt32 ActiasAtomicFetchAnd32Relaxed(ActiasAtomic32* pDestination, UInt32 value)
{
    UInt32 previous;
    UInt32 temp;
    asm volatile("1:     movl    %1, %0\n"
                 "       movl    %0, %2\n"
                 "       andl    %3, %2\n"
                 "       lock; cmpxchgl %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt32 ActiasAtomicFetchOr32Relaxed(ActiasAtomic32* pDestination, UInt32 value)
{
    UInt32 previous;
    UInt32 temp;
    asm volatile("1:     movl    %1, %0\n"
                 "       movl    %0, %2\n"
                 "       orl     %3, %2\n"
                 "       lock; cmpxchgl %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicExchange32Relaxed(ActiasAtomic32* pDestination, UInt32 desired)
{
    UInt32 previous;
    asm volatile("xchgl %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(desired));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt32 ActiasAtomicCompareExchange32Relaxed(ActiasAtomic32* pDestination, UInt32 expected,
                                                                       UInt32 desired)
{
    UInt32 previous;
    asm volatile("lock; cmpxchgl %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(expected));
    return previous;
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchangeWeak32Relaxed(ActiasAtomic32* pDestination, UInt32* expected,
                                                                               UInt32 desired)
{
    UInt32 e = *expected;
    UInt32 previous;
    ActiasBool matched;
    asm volatile("lock; cmpxchgl %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
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
    UInt64 previous;
    asm volatile("lock; xaddq %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt64 ActiasAtomicFetchAnd64Relaxed(ActiasAtomic64* pDestination, UInt64 value)
{
    UInt64 previous;
    UInt64 temp;
    asm volatile("1:     movq    %1, %0\n"
                 "       movq    %0, %2\n"
                 "       andq    %3, %2\n"
                 "       lock; cmpxchgq %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE UInt64 ActiasAtomicFetchOr64Relaxed(ActiasAtomic64* pDestination, UInt64 value)
{
    UInt64 previous;
    UInt64 temp;
    asm volatile("1:     movq    %1, %0\n"
                 "       movq    %0, %2\n"
                 "       orq     %3, %2\n"
                 "       lock; cmpxchgq %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(pDestination->Data), "=&r"(temp)
                 : "r"(value));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicExchange64Relaxed(ActiasAtomic64* pDestination, UInt64 desired)
{
    UInt64 previous;
    asm volatile("xchgq %0, %1" : "=r"(previous), "+m"(pDestination->Data) : "0"(desired));
    return previous;
}

ACTIAS_FORCE_INLINE static UInt64 ActiasAtomicCompareExchange64Relaxed(ActiasAtomic64* pDestination, UInt64 expected,
                                                                       UInt64 desired)
{
    UInt64 previous;
    asm volatile("lock; cmpxchgq %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(expected));
    return previous;
}

ACTIAS_FORCE_INLINE static ActiasBool ActiasAtomicCompareExchangeWeak64Relaxed(ActiasAtomic64* pDestination, UInt64* expected,
                                                                               UInt64 desired)
{
    UInt64 e = *expected;
    UInt64 previous;
    ActiasBool matched;
    asm volatile("lock; cmpxchgq %2, %1" : "=a"(previous), "+m"(pDestination->Data) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

#endif

ACTIAS_END_C
