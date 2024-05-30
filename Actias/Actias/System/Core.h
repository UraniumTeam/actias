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
#include <Actias/System/Base.h>
#include <Actias/System/Platform/Common/InlineCopyMemory.h>
#include <Actias/System/Platform/Common/InlineSetMemory.h>
#include <Actias/System/Platform/Common/InlineStringLength.h>
#include <Actias/System/Result.h>

ACTIAS_BEGIN_C

typedef struct ActiasHandleStruct* ActiasHandle;
typedef Int64(ACTIAS_ABI* ActiasProc)();

enum ActiasFlagValues
{
    ACTIAS_FLAGS_NONE = 0 //!< No flags, zero.
};

enum ActiasSignalValues
{
    ACTIAS_SIGABRT = 1, //!< Abnormal termination.
    ACTIAS_SIGFPE  = 2, //!< Erroneous arithmetic operation.
    ACTIAS_SIGILL  = 3, //!< Invalid program image.
    ACTIAS_SIGINT  = 4, //!< External interrupt.
    ACTIAS_SIGSEGV = 5, //!< Segmentation fault.
    ACTIAS_SIGTERM = 6, //!< Termination request.
    ACTIAS_SIGTRAP = 7, //!< Breakpoint trap.
};

enum ActiasOSFamilyValues
{
    ACTIAS_OS_FAMILY_WINDOWS = 1, //!< Windows OS family.
    ACTIAS_OS_FAMILY_UNIX    = 2, //!< Unix OS family.
};

enum ActiasOSValues
{
    ACTIAS_OS_WINDOWS = 1, //!< Windows operating system.
    ACTIAS_OS_LINUX   = 2, //!< Linux operating system.
};

//! \brief Basic flags type.
typedef UInt32 ActiasFlags;

//! \brief Basic system properties.
typedef struct ActiasSystemProperties
{
    const char* OSName;    //!< Name of the OS the current process is running on.
    UInt32 PageSize;       //!< Size of memory page in bytes.
    UInt32 ProcessorCount; //!< Number of logical processors.
    UInt16 OSFamily;       //!< Family of the OS the current process is running on, see ActiasOSFamilyValues.
    UInt16 OS;             //!< OS the current process is running on, see ActiasOSValues.
} ActiasSystemProperties;

inline static UInt64 ACTIAS_ABI ActiasAlignUp(UInt64 x, UInt64 a)
{
    return (x + (a - 1)) & ~(a - 1);
}

//! \brief Get last OS native error code.
ACTIAS_SYSTEM_API Int32 ACTIAS_ABI ActiasGetNativeErrorCode(void);

ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasRaiseSignal(Int32 signal);

//! \brief Get system properties.
//!
//! \param pProperties - A pointer to the variable that receives system properties.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasGetSystemProperties(ActiasSystemProperties* pProperties);

//! \brief Copy one memory buffer to another (with specific alignment requirements and non-temporal instructions).
//!
//! The provided destination must have 32-byte alignment and the size must have 256-byte alignment.
//!
//! \param pDestination - A pointer to the memory to copy to.
//! \param pSource - A pointer to the memory to copy from.
//! \param byteSize - The number of bytes to copy.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasStreamMemory(void* ACTIAS_RESTRICT pDestination, const void* ACTIAS_RESTRICT pSource,
                                                     USize byteSize);

//! \brief Copy one memory buffer to another (with specific alignment requirements).
//!
//! The provided destination must have 32-byte alignment and the size must have 256-byte alignment.
//!
//! \param pDestination - A pointer to the memory to copy to.
//! \param pSource - A pointer to the memory to copy from.
//! \param byteSize - The number of bytes to copy.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasCopyAlignedMemory(void* ACTIAS_RESTRICT pDestination, const void* ACTIAS_RESTRICT pSource,
                                                          USize byteSize);

//! \brief Copy one memory buffer to another.
//!
//! \param pDestination - A pointer to the memory to copy to.
//! \param pSource - A pointer to the memory to copy from.
//! \param byteSize - The number of bytes to copy.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasCopyMemory(void* ACTIAS_RESTRICT pDestination, const void* ACTIAS_RESTRICT pSource,
                                                   USize byteSize);

//! \brief Copy a value to each of the first bytes of the provided memory buffer.
//!
//! \param pDestination - A pointer to the memory to fill.
//! \param value - The fill byte.
//! \param byteCount - The number of bytes to fill.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasSetMemory(void* pDestination, UInt8 value, USize byteCount);

//! \brief Inline function to set each of the first bytes of the provided memory buffer to zero.
//!
//! \param pDestination - A pointer to the memory to fill.
//! \param byteCount - The number of bytes to fill.
inline static void ACTIAS_ABI ActiasInlineZeroMemory(void* pDestination, USize byteCount)
{
    ActiasInlineSetMemory(pDestination, 0, byteCount);
}

//! \brief Set each of the first bytes of the provided memory buffer to zero.
//!
//! \param pDestination - A pointer to the memory to fill.
//! \param byteCount - The number of bytes to fill.
inline static void ACTIAS_ABI ActiasZeroMemory(void* pDestination, USize byteCount)
{
    ActiasSetMemory(pDestination, 0, byteCount);
}

ACTIAS_END_C
