#pragma once
#include <Actias/System/Core.h>

#if TestLibrary_EXPORTS
#    define TESTLIB_API ACTIAS_EXPORT
#else
#    define TESTLIB_API ACTIAS_IMPORT
#endif

extern "C" TESTLIB_API Int32 AddNumbers(Int32 a, Int32 b);
