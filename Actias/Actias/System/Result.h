/*
 * Actias SDK System
 *
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/System/Platform.h>

ACTIAS_BEGIN_C

typedef enum ActiasResultValues
{
    // General results

    ACTIAS_SUCCESS = 0, //!< Operation completed successfully.

    ACTIAS_FAIL_UNKNOWN           = -1, //!< Operation failed with an unexpected error, try ActiasGetNativeErrorCode.
    ACTIAS_FAIL_NOT_SUPPORTED     = -2, //!< Operation is not supported.
    ACTIAS_FAIL_INSUFFICIENT_SIZE = -3, //!< Insufficient buffer size.

    // IO results

    ACTIAS_FAIL_IO_ERROR = -20, //!< IO error.

    ACTIAS_FAIL_INVALID_STD_DESCRIPTOR = ACTIAS_FAIL_IO_ERROR - 1,  //!< Provided standard file descriptor was not valid.
    ACTIAS_FAIL_PERMISSION_DENIED      = ACTIAS_FAIL_IO_ERROR - 2,  //!< Permission denied.
    ACTIAS_FAIL_NO_FILE_OR_DIRECTORY   = ACTIAS_FAIL_IO_ERROR - 3,  //!< No such file or directory.
    ACTIAS_FAIL_FILE_EXISTS            = ACTIAS_FAIL_IO_ERROR - 4,  //!< File already exists.
    ACTIAS_FAIL_FILE_TOO_LARGE         = ACTIAS_FAIL_IO_ERROR - 5,  //!< File is too large.
    ACTIAS_FAIL_FILENAME_TOO_LONG      = ACTIAS_FAIL_IO_ERROR - 6,  //!< Filename is too long.
    ACTIAS_FAIL_NOT_DIRECTORY          = ACTIAS_FAIL_IO_ERROR - 7,  //!< Not a directory.
    ACTIAS_FAIL_IS_DIRECTORY           = ACTIAS_FAIL_IO_ERROR - 8,  //!< Is a directory.
    ACTIAS_FAIL_DIRECTORY_NOT_EMPTY    = ACTIAS_FAIL_IO_ERROR - 9,  //!< Directory is not empty.
    ACTIAS_FAIL_TOO_MANY_OPEN_FILES    = ACTIAS_FAIL_IO_ERROR - 10, //!< Too many files are open.
    ACTIAS_FAIL_INVALID_SEEK           = ACTIAS_FAIL_IO_ERROR - 11, //!< Invalid seek operation.
    ACTIAS_FAIL_DEAD_LOCK              = ACTIAS_FAIL_IO_ERROR - 12, //!< Resource deadlock would occur.
    ACTIAS_FAIL_READ_NOT_ALLOWED       = ACTIAS_FAIL_IO_ERROR - 13, //!< Read operation is not allowed.
    ACTIAS_FAIL_WRITE_NOT_ALLOWED      = ACTIAS_FAIL_IO_ERROR - 14, //!< Write operation is not allowed.
    ACTIAS_FAIL_NOT_OPEN               = ACTIAS_FAIL_IO_ERROR - 15, //!< File or stream is not open.

    // Runtime results

    ACTIAS_FAIL_RT_ERROR = ACTIAS_FAIL_IO_ERROR - 30, //!< Runtime error.

    ACTIAS_FAIL_RT_INITIALIZED             = ACTIAS_FAIL_RT_ERROR - 1, //!< Runtime was already initialized.
    ACTIAS_FAIL_RT_NOT_INITIALIZED         = ACTIAS_FAIL_RT_ERROR - 2, //!< Runtime was not initialized.
    ACTIAS_FAIL_SYMBOL_NOT_FOUND           = ACTIAS_FAIL_RT_ERROR - 3, //!< Runtime was unable to find an exported symbol.
    ACTIAS_FAIL_ACBX_INVALID_SIGNATURE     = ACTIAS_FAIL_RT_ERROR - 4, //!< ACBX file signature was not valid.
    ACTIAS_FAIL_ACBX_SECTION_OUT_OF_BOUNDS = ACTIAS_FAIL_RT_ERROR - 5, //!< ACBX file section was out of bounds.
} ActiasResultValues;

//! \brief Indicates operation status.
//!
//! Success codes are always non-negative.
typedef Int32 ActiasResult;

ACTIAS_END_C
