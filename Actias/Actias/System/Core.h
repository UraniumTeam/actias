/*
 * Actias SDK System
 *
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/Base/CompilerTraits.h>

ACTIAS_BEGIN_C

typedef void* ActiasHandle;

//! \brief Indicates operation status.
//!
//! Success codes are always non-negative.
typedef enum ActiasResult
{
    ACTIAS_SUCCESS                     = 0,  //!< Operation completed successfully
    ACTIAS_FAIL_UNKNOWN                = -1, //!< Operation failed with an unexpected error, try ActiasGetNativeErrorCode
    ACTIAS_FAIL_INVALID_STD_DESCRIPTOR = -2, //!< Provided standard file descriptor was not valid.
} ActiasResult;

//! \brief Basic flags type.
typedef enum ActiasFlags
{
    ACTIAS_FLAGS_NONE = 0 //!< No flags, zero.
} ActiasFlags;

//! \brief Get last OS native error code.
Int32 ActiasGetNativeErrorCode(void);

ACTIAS_END_C
