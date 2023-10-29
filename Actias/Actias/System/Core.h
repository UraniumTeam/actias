/*
 * Actias SDK System
 *
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/Base/Platform.h>

#if ActiasSystem_EXPORTS
#    define ACTIAS_SYSTEM_API ACTIAS_EXPORT
#else
#    define ACTIAS_SYSTEM_API
#endif

ACTIAS_BEGIN_C

typedef void* ActiasHandle;

typedef enum ActiasResultValues
{
    ACTIAS_SUCCESS                     = 0,  //!< Operation completed successfully
    ACTIAS_FAIL_UNKNOWN                = -1, //!< Operation failed with an unexpected error, try ActiasGetNativeErrorCode
    ACTIAS_FAIL_INVALID_STD_DESCRIPTOR = -2, //!< Provided standard file descriptor was not valid.
} ActiasResultValues;

//! \brief Indicates operation status.
//!
//! Success codes are always non-negative.
typedef Int32 ActiasResult;

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

//! \brief Copy one memory buffer to another.
//!
//! \param pDestination - A pointer to the memory to copy to.
//! \param pSource - A pointer to the memory to copy from.
//! \param byteSize - The number of bytes to copy.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasCopyMemory(void* pDestination, ACTIAS_CONST void* pSource, USize byteSize);

ACTIAS_END_C
