/*
 * Copyright 2024 the Actias project authors (see the file AUTHORS
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
#include <Actias/System/Platform.h>

ACTIAS_BEGIN_C

enum ActiasResultValues
{
    // General results

    ACTIAS_SUCCESS = 0, //!< Operation completed successfully.

    ACTIAS_FAIL_UNKNOWN               = -1, //!< Operation failed with an unexpected error, try ActiasGetNativeErrorCode.
    ACTIAS_FAIL_NOT_SUPPORTED         = -2, //!< Operation is not supported.
    ACTIAS_FAIL_INSUFFICIENT_SIZE     = -3, //!< Insufficient buffer size.
    ACTIAS_FAIL_INVALID_ENCODING      = -4, //!< UTF-8 string has incorrect encoding.
    ACTIAS_FAIL_INVALID_ARGUMENT      = -5, //!< Argument value has not been accepted.
    ACTIAS_FAIL_ARGUMENT_OUT_OF_RANGE = -6, //!< Argument value is outside the allowable range of values.

    ACTIAS_TIMEOUT = 1, //!< Operation timed out.

    // IO results

    ACTIAS_FAIL_IO_ERROR = -100, //!< IO error.

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

    ACTIAS_FAIL_RT_ERROR = ACTIAS_FAIL_IO_ERROR - 100, //!< Runtime error.

    ACTIAS_FAIL_RT_INITIALIZED             = ACTIAS_FAIL_RT_ERROR - 1, //!< Runtime was already initialized.
    ACTIAS_FAIL_RT_NOT_INITIALIZED         = ACTIAS_FAIL_RT_ERROR - 2, //!< Runtime was not initialized.
    ACTIAS_FAIL_SYMBOL_NOT_FOUND           = ACTIAS_FAIL_RT_ERROR - 3, //!< Runtime was unable to find an exported symbol.
    ACTIAS_FAIL_ACBX_INVALID_SIGNATURE     = ACTIAS_FAIL_RT_ERROR - 4, //!< ACBX file signature was not valid.
    ACTIAS_FAIL_ACBX_SECTION_OUT_OF_BOUNDS = ACTIAS_FAIL_RT_ERROR - 5, //!< ACBX file section was out of bounds.

    // Wait results

    ACTIAS_FAIL_WAIT_ERROR = ACTIAS_FAIL_RT_ERROR - 100, //!< Wait error.

    ACTIAS_FAIL_THREAD_WAIT_ERROR = ACTIAS_FAIL_WAIT_ERROR - 1, //!< Thread wait error.
    ACTIAS_FAIL_MUTEX_WAIT_ERROR  = ACTIAS_FAIL_WAIT_ERROR - 2, //!< Mutex wait error.

    ACTIAS_THREAD_WAIT_TIMEOUT = 1 - ACTIAS_FAIL_WAIT_ERROR, //!< Thread wait timeout.
    ACTIAS_MUTEX_WAIT_TIMEOUT  = 2 - ACTIAS_FAIL_WAIT_ERROR, //!< Mutex wait timeout.
};

//! \brief Indicates operation status.
//!
//! Success codes are always non-negative.
typedef Int32 ActiasResult;

ACTIAS_END_C
