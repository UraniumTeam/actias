#pragma once
#include <Actias/Memory/Object.hpp>
#include <Actias/System/Core.h>

#if TestLibrary_EXPORTS
#    define TESTLIB_API ACTIAS_EXPORT
#else
#    define TESTLIB_API ACTIAS_IMPORT
#endif

extern "C" TESTLIB_API Int32 ACTIAS_ABI AddNumbers(Int32 a, Int32 b);
extern "C" TESTLIB_API Int32 ACTIAS_ABI AddNumbersEx(Int32 a, Int32 b, Int32 c);
extern "C" TESTLIB_API const char* ACTIAS_ABI GetMessage();
