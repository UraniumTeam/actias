#pragma once
#include <Actias/System/Base.h>

#if ACTIAS_COMPILER_MSVC | ACTIAS_COMPILER_MS_CLANG
#    include <Actias/System/Compiler/AtomicImplMSVC.h>
#else
#    error Atomics are not currently supported with this compiler
#endif

ACTIAS_BEGIN_C

enum ActiasMemoryOrderValues
{
    ACTIAS_MEMORY_ORDER_RELAXED = 0,
    ACTIAS_MEMORY_ORDER_ACQUIRE = 1,
    ACTIAS_MEMORY_ORDER_CONSUME = ACTIAS_MEMORY_ORDER_ACQUIRE,
    ACTIAS_MEMORY_ORDER_RELEASE = 2,
    ACTIAS_MEMORY_ORDER_ACQ_REL = 3,
    ACTIAS_MEMORY_ORDER_SEQ_CST = ACTIAS_MEMORY_ORDER_ACQ_REL,
};

typedef Int32 ActiasMemoryOrder;

#define ACTIAS_PlaceAquireFenceIfNeeded(memoryOrder)                                                                             \
    {                                                                                                                            \
        switch (memoryOrder)                                                                                                     \
        {                                                                                                                        \
        case ACTIAS_MEMORY_ORDER_ACQUIRE:                                                                                        \
        case ACTIAS_MEMORY_ORDER_ACQ_REL:                                                                                        \
            ACTIAS_ThreadFenceAcquire();                                                                                         \
            break;                                                                                                               \
        }                                                                                                                        \
    }

#define ACTIAS_PlaceReleaseFenceIfNeeded(memoryOrder)                                                                            \
    {                                                                                                                            \
        switch (memoryOrder)                                                                                                     \
        {                                                                                                                        \
        case ACTIAS_MEMORY_ORDER_RELEASE:                                                                                        \
        case ACTIAS_MEMORY_ORDER_ACQ_REL:                                                                                        \
            ACTIAS_ThreadFenceRelease();                                                                                         \
            break;                                                                                                               \
        }                                                                                                                        \
    }

ACTIAS_END_C
