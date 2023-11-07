/*
 * Actias SDK System
 *
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/System/Platform/Common/InlineCopyMemory.h>
#include <Actias/System/Platform/Common/InlineSetMemory.h>
#include <Actias/System/Result.h>

#if ActiasSystem_EXPORTS
#    define ACTIAS_SYSTEM_API ACTIAS_EXPORT
#else
#    define ACTIAS_SYSTEM_API ACTIAS_IMPORT
#endif

ACTIAS_BEGIN_C

typedef void* ActiasHandle;
typedef Int64(ACTIAS_ABI* ActiasProc)();

typedef enum ActiasFlagValues
{
    ACTIAS_FLAGS_NONE = 0 //!< No flags, zero.
} ActiasFlagValues;

//! \brief Basic flags type.
typedef UInt32 ActiasFlags;

//! \brief Basic system properties.
typedef struct ActiasSystemProperties
{
    UInt32 PageSize;       //!< Size of memory page in bytes.
    UInt32 ProcessorCount; //!< Number of logical processors.
} ActiasSystemProperties;

//! \brief Get last OS native error code.
ACTIAS_SYSTEM_API Int32 ACTIAS_ABI ActiasGetNativeErrorCode(void);

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
inline void ACTIAS_ABI ActiasInlineZeroMemory(void* pDestination, USize byteCount)
{
    ActiasInlineSetMemory(pDestination, 0, byteCount);
}

//! \brief Set each of the first bytes of the provided memory buffer to zero.
//!
//! \param pDestination - A pointer to the memory to fill.
//! \param byteCount - The number of bytes to fill.
inline void ACTIAS_ABI ActiasZeroMemory(void* pDestination, USize byteCount)
{
    ActiasSetMemory(pDestination, 0, byteCount);
}

ACTIAS_END_C
