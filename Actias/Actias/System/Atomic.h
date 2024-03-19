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

// clang-format off

#include <Actias/System/Platform/Common/AtomicImpl8.h>
#include <Actias/System/Platform/Common/AtomicImpl16.h>
#include <Actias/System/Platform/Common/AtomicImpl32.h>

#if ACTIAS_ARCH_64_BIT
#    include <Actias/System/Platform/Common/AtomicImpl64.h>
#endif

// clang-format on
