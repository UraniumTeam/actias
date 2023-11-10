#pragma once

#define ACTIAS_PATH_SEPARATOR '/'

#if defined _WIN32 || defined _WIN64 || defined _WINDOWS
#    define ACTIAS_WINDOWS 1

#    define ACTIAS_ABI

#    define ACTIAS_NATIVE_DL_EXTENSION ".dll"
#    define ACTIAS_NATIVE_EX_EXTENSION ".exe"
#    define ACTIAS_OS_NAME "Windows"

#    define ACTIAS_ByteSwapUInt16(value) _byteswap_ushort(value)
#    define ACTIAS_ByteSwapUInt32(value) _byteswap_ulong(value)
#    define ACTIAS_ByteSwapUInt64(value) _byteswap_uint64(value)

#    define ACTIAS_EXPORT __declspec(dllexport)
#    define ACTIAS_IMPORT __declspec(dllimport)

#elif defined __linux__
#    define ACTIAS_LINUX 1

#    define ACTIAS_ABI __attribute__((ms_abi))

#    define ACTIAS_NATIVE_DL_EXTENSION ".so"
#    define ACTIAS_NATIVE_EX_EXTENSION ""
#    define ACTIAS_OS_NAME "Linux"

#    define ACTIAS_ByteSwapUInt16(value) __builtin_bswap16(value)
#    define ACTIAS_ByteSwapUInt32(value) __builtin_bswap32(value)
#    define ACTIAS_ByteSwapUInt64(value) __builtin_bswap64(value)

#    define ACTIAS_EXPORT __attribute__((visibility("default")))
#    define ACTIAS_IMPORT __attribute__((visibility("default")))

#else
#    error Unsupported platform
#endif
