/*
 * Copyright 2023 the Actias project authors (see the file AUTHORS
 * for a complete list)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#define ACTIAS_PATH_SEPARATOR '/'

#if defined _WIN32 || defined _WIN64 || defined _WINDOWS
#    define ACTIAS_WINDOWS 1

#    define ACTIAS_ABI

#    define ACTIAS_NATIVE_DL_EXTENSION ".dll"
#    define ACTIAS_NATIVE_EX_EXTENSION ".exe"

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

#    define ACTIAS_ByteSwapUInt16(value) __builtin_bswap16(value)
#    define ACTIAS_ByteSwapUInt32(value) __builtin_bswap32(value)
#    define ACTIAS_ByteSwapUInt64(value) __builtin_bswap64(value)

#    define ACTIAS_EXPORT __attribute__((visibility("default")))
#    define ACTIAS_IMPORT __attribute__((visibility("default")))

#else
#    error Unsupported platform
#endif
