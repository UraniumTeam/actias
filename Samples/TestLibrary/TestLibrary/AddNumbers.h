#pragma once
#include <Actias/System/Platform.h>

#if TestLibrary_EXPORTS
#    define TESTLIB_API ACTIAS_EXPORT
#else
#    define TESTLIB_API ACTIAS_IMPORT
#endif

ACTIAS_BEGIN_C

TESTLIB_API Int32 ACTIAS_ABI AddNumbers(Int32 a, Int32 b);
TESTLIB_API Int32 ACTIAS_ABI AddNumbersEx(Int32 a, Int32 b, Int32 c);
TESTLIB_API const char* ACTIAS_ABI GetMessage();

ACTIAS_END_C
