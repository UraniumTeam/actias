#pragma once
#include <stddef.h>
#include <stdint.h>

#if defined NDEBUG
#    define ACTIAS_RELEASE 1
#else
#    define ACTIAS_DEBUG 1
#endif

#define ACTIAS_MAKE_STR(txt) #txt

#if defined __clang__
#    define ACTIAS_COMPILER_CLANG 1

#    if defined _MSC_VER
#        define ACTIAS_COMPILER_MS_CLANG 1
#    endif

#    define ACTIAS_PUSH_MSVC_WARNING(...)
#    define ACTIAS_POP_MSVC_WARNING

#    define ACTIAS_PUSH_CLANG_WARNING(warn)                                                                                      \
        _Pragma("clang diagnostic push") _Pragma(ACTIAS_MAKE_STR(clang diagnostic ignored warn))
#    define ACTIAS_POP_CLANG_WARNING _Pragma("clang diagnostic pop")

#    define ACTIAS_PRETTY_FUNCTION __PRETTY_FUNCTION__

#    ifndef ACTIAS_FINLINE
#        define ACTIAS_FINLINE inline
#    endif
#elif defined _MSC_VER
#    define ACTIAS_COMPILER_MSVC 1

#    define ACTIAS_PUSH_MSVC_WARNING(warn) __pragma(warning(push)) __pragma(warning(disable : warn))
#    define ACTIAS_POP_MSVC_WARNING __pragma(warning(pop))

#    define ACTIAS_PUSH_CLANG_WARNING(...)
#    define ACTIAS_POP_CLANG_WARNING

#    define ACTIAS_PRETTY_FUNCTION __FUNCSIG__

#    ifndef ACTIAS_FINLINE
#        define ACTIAS_FINLINE __forceinline
#    endif
#endif

#if ACTIAS_COMPILER_MSVC || ACTIAS_COMPILER_MS_CLANG
#    define ACTIAS_DEBUG_BREAK __debugbreak()
#else
#    define ACTIAS_DEBUG_BREAK raise(SIGTRAP)
#endif

#if defined __cplusplus

#    define ACTIAS_BEGIN_C                                                                                                       \
        extern "C"                                                                                                               \
        {
#    define ACTIAS_END_C }

#else

#    define ACTIAS_BEGIN_C
#    define ACTIAS_END_C

#endif
