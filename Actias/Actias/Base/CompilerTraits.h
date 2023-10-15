#pragma once
#include <stddef.h>
#include <stdint.h>

#if defined __cplusplus

#    define ACTIAS_BEGIN_C                                                                                                       \
        extern "C"                                                                                                               \
        {
#    define ACTIAS_END_C }

#    define ACTIAS_CONST const

#else

#    define ACTIAS_BEGIN_C
#    define ACTIAS_END_C

#    define ACTIAS_CONST

#endif

ACTIAS_BEGIN_C

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

typedef float Float32;
typedef double Float64;

#if ACTIAS_ARCH_64_BIT
typedef UInt64 USize;
typedef Int64 SSize;
#else
typedef UInt32 USize;
typedef Int32 SSize;
#endif

ACTIAS_END_C

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

#    ifndef ACTIAS_FORCE_INLINE
#        define ACTIAS_FORCE_INLINE inline
#    endif
#elif defined _MSC_VER
#    define ACTIAS_COMPILER_MSVC 1

#    define ACTIAS_PUSH_MSVC_WARNING(warn) __pragma(warning(push)) __pragma(warning(disable : warn))
#    define ACTIAS_POP_MSVC_WARNING __pragma(warning(pop))

#    define ACTIAS_PUSH_CLANG_WARNING(...)
#    define ACTIAS_POP_CLANG_WARNING

#    define ACTIAS_PRETTY_FUNCTION __FUNCSIG__

#    ifndef ACTIAS_FORCE_INLINE
#        define ACTIAS_FORCE_INLINE __forceinline
#    endif
#endif

#if ACTIAS_COMPILER_MSVC || ACTIAS_COMPILER_MS_CLANG
#    define ACTIAS_DEBUG_BREAK __debugbreak()
#else
#    define ACTIAS_DEBUG_BREAK raise(SIGTRAP)
#endif