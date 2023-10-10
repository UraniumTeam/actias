#pragma once

#if defined _WIN32 || defined _WIN64 || defined _WINDOWS
#    define ACTIAS_WINDOWS 1

#    define ACTIAS_DLL_EXTENSION ".dll"
#    define ACTIAS_EXE_EXTENSION ".exe"
#    define ACTIAS_PATH_SEPARATOR '\\'
#    define ACTIAS_OS_NAME "Windows"

#    define ACTIAS_ByteSwapUInt16(value) _byteswap_ushort(value)
#    define ACTIAS_ByteSwapUInt32(value) _byteswap_ulong(value)
#    define ACTIAS_ByteSwapUInt64(value) _byteswap_uint64(value)

#    define ACTIAS_EXPORT __declspec(dllexport)
#    define ACTIAS_IMPORT __declspec(dllimport)

#elif defined __linux__
#    define ACTIAS_LINUX 1

#    define ACTIAS_DLL_EXTENSION ".so"
#    define ACTIAS_EXE_EXTENSION ""
#    define ACTIAS_PATH_SEPARATOR '/'
#    define ACTIAS_OS_NAME "Linux"

#    define ACTIAS_ByteSwapUInt16(value) __builtin_bswap16(value)
#    define ACTIAS_ByteSwapUInt32(value) __builtin_bswap32(value)
#    define ACTIAS_ByteSwapUInt64(value) __builtin_bswap64(value)

#    define ACTIAS_EXPORT __attribute__((visibility("default"))) [[gnu::ms_abi]]
#    define ACTIAS_IMPORT __attribute__((visibility("default"))) [[gnu::ms_abi]]

#else
#    error Unsupported platform
#endif
