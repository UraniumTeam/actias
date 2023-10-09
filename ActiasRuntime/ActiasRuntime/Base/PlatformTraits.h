#pragma once
#include <csignal>
#include <malloc.h>

#if defined _WIN32 || defined _WIN64 || defined _WINDOWS
#    define ACTIAS_WINDOWS 1

#    define ACTIAS_DLL_EXTENSION ".dll"
#    define ACTIAS_EXE_EXTENSION ".exe"
#    define ACTIAS_PATH_SEPARATOR '\\'
#    define ACTIAS_OS_NAME "Windows"

#    define ACTIAS_AlignedMalloc(size, alignment) _aligned_malloc(size, alignment)
#    define ACTIAS_AlignedFree(ptr) _aligned_free(ptr)

#    define ACTIAS_ByteSwapUInt16(value) _byteswap_ushort(value)
#    define ACTIAS_ByteSwapUInt32(value) _byteswap_ulong(value)
#    define ACTIAS_ByteSwapUInt64(value) _byteswap_uint64(value)

#    define ACTIAS_DLL_EXPORT __declspec(dllexport)
#    define ACTIAS_DLL_IMPORT __declspec(dllimport)

#elif defined __linux__
#    define ACTIAS_LINUX 1

#    define ACTIAS_DLL_EXTENSION ".so"
#    define ACTIAS_EXE_EXTENSION ""
#    define ACTIAS_PATH_SEPARATOR '/'
#    define ACTIAS_OS_NAME "Linux"

#    define ACTIAS_AlignedMalloc(size, alignment) ::memalign(alignment, size)
#    define ACTIAS_AlignedFree(ptr) ::free(ptr)

#    define ACTIAS_ByteSwapUInt16(value) __builtin_bswap16(value)
#    define ACTIAS_ByteSwapUInt32(value) __builtin_bswap32(value)
#    define ACTIAS_ByteSwapUInt64(value) __builtin_bswap64(value)

#    define ACTIAS_DLL_EXPORT __attribute__((visibility("default")))
#    define ACTIAS_DLL_IMPORT __attribute__((visibility("default")))

#else
#    error Unsupported platform
#endif
