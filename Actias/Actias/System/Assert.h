/*
 * Copyright 2024 the Actias project authors (see the file AUTHORS
 * for a complete list)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
