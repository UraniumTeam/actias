#pragma once
#include <Actias/System/Core.h>

#ifndef ACTIAS_AssertSignal
#    define ACTIAS_AssertSignal(signal, expr, ...)                                                                               \
        do                                                                                                                       \
        {                                                                                                                        \
            if (!(expr))                                                                                                         \
            {                                                                                                                    \
                ActiasRaiseSignal(signal);                                                                                       \
            }                                                                                                                    \
        }                                                                                                                        \
        while (ACTIAS_FALSE)
#endif

#ifndef ACTIAS_AssertCritical
#    define ACTIAS_AssertCritical(expr, ...) ACTIAS_AssertSignal(ACTIAS_SIGABRT, expr, __VA_ARGS__)
#endif

#ifndef ACTIAS_AssertDebug
#    if ACTIAS_DEBUG
#        define ACTIAS_AssertDebug(expr, ...) ACTIAS_AssertSignal(ACTIAS_SIGTRAP, expr, __VA_ARGS__)
#    else
#        define ACTIAS_AssertDebug(expr, ...) ACTIAS_Unused(expr)
#    endif
#endif

#ifndef ACTIAS_Assert
#    define ACTIAS_Assert(expr, ...) ACTIAS_AssertDebug(expr, __VA_ARGS__)
#endif

#define ACTIAS_Unreachable(...) ACTIAS_Assert(ACTIAS_FALSE, __VA_ARGS__)
