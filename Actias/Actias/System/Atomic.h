#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

ACTIAS_SYSTEM_API Int8 ACTIAS_ABI ActiasAtomicAddI8(volatile Int8* pAddend, Int8 Value);

ACTIAS_SYSTEM_API Int16 ACTIAS_ABI ActiasAtomicAddI16(volatile Int16* pAddend, Int16 Value);

ACTIAS_SYSTEM_API Int32 ACTIAS_ABI ActiasAtomicAddI32(volatile Int32* pAddend, Int32 Value);

ACTIAS_SYSTEM_API Int64 ACTIAS_ABI ActiasAtomicAddI64(volatile Int64* pAddend, Int64 Value);

ACTIAS_END_C
