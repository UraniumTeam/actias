#pragma once
#include <stddef.h>
#include <stdint.h>

#if defined __cplusplus
#    define ACTIAS_BEGIN_C                                                                                                       \
        extern "C"                                                                                                               \
        {
#    define ACTIAS_END_C }

#    define ACTIAS_RESTRICT __restrict

#    define ACTIAS_FALSE false
#    define ACTIAS_TRUE true
#else
#    define ACTIAS_BEGIN_C
#    define ACTIAS_END_C

#    if defined _MSC_VER
#        define ACTIAS_RESTRICT __restrict
#    else
#        define ACTIAS_RESTRICT restrict
#    endif

#    define ACTIAS_FALSE 0
#    define ACTIAS_TRUE 1
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

#define ACTIAS_VERSION_PATCH_BIT_COUNT 12u
#define ACTIAS_VERSION_MINOR_BIT_COUNT 10u

#define ACTIAS_MAKE_VERSION(major, minor, patch)                                                                                 \
    (((UInt32)(major)) << (ACTIAS_VERSION_PATCH_BIT_COUNT + ACTIAS_VERSION_MINOR_BIT_COUNT))                                     \
        | (((UInt32)(minor)) << ACTIAS_VERSION_PATCH_BIT_COUNT) | ((UInt32)(patch))

#define ACTIAS_VERSION_1_0 ACTIAS_MAKE_VERSION(1, 0, 0)

#if defined NDEBUG
#    define ACTIAS_RELEASE 1
#else
#    define ACTIAS_DEBUG 1
#endif

#define ACTIAS_MAKE_STR(txt) #txt

#define ACTIAS_UNUSED(param) (void)(param)

#if defined __clang__
#    define ACTIAS_COMPILER_CLANG 1

#    define ACTIAS_NO_BUILTIN(value) __attribute__((no_builtin(#value)))

#    if defined _MSC_VER
#        define ACTIAS_COMPILER_MS_CLANG 1
#    endif

#    define ACTIAS_PUSH_MSVC_WARNING(...)
#    define ACTIAS_POP_MSVC_WARNING

#    define ACTIAS_PUSH_CLANG_WARNING(warn)                                                                                      \
        _Pragma("clang diagnostic push") _Pragma(ACTIAS_MAKE_STR(clang diagnostic ignored warn))
#    define ACTIAS_POP_CLANG_WARNING _Pragma("clang diagnostic pop")

#    define ACTIAS_PRETTY_FUNCTION __PRETTY_FUNCTION__

#    define ACTIAS_BUILTIN_ASSUME(expr) __builtin_assume(expr)

#    ifndef ACTIAS_FORCE_INLINE
#        define ACTIAS_FORCE_INLINE inline
#    endif
#elif defined _MSC_VER
#    define ACTIAS_COMPILER_MSVC 1

#    define ACTIAS_NO_BUILTIN(value)

#    define ACTIAS_PUSH_MSVC_WARNING(warn) __pragma(warning(push)) __pragma(warning(disable : warn))
#    define ACTIAS_POP_MSVC_WARNING __pragma(warning(pop))

#    define ACTIAS_PUSH_CLANG_WARNING(...)
#    define ACTIAS_POP_CLANG_WARNING

#    define ACTIAS_PRETTY_FUNCTION __FUNCSIG__

#    define ACTIAS_BUILTIN_ASSUME(expr) __assume(expr)

#    ifndef ACTIAS_FORCE_INLINE
#        define ACTIAS_FORCE_INLINE __forceinline
#    endif
#endif
