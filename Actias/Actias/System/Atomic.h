#pragma once

// clang-format off

#include <Actias/System/Platform/Common/AtomicImpl8.h>
#include <Actias/System/Platform/Common/AtomicImpl16.h>
#include <Actias/System/Platform/Common/AtomicImpl32.h>

#if ACTIAS_ARCH_64_BIT
#    include <Actias/System/Platform/Common/AtomicImpl64.h>
#endif

// clang-format on
