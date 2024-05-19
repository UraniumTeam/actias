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
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

//! \brief Standard file descriptors (stdin, stdout, stderr)
enum ActiasStandardDescriptorValues
{
    ACTIAS_STDIN  = 0x0000, //!< Standard input file descriptor.
    ACTIAS_STDOUT = 0x0001, //!< Standard output file descriptor.
    ACTIAS_STDERR = 0x0002, //!< Standard error file descriptor.

    ACTIAS_STD_DESCRIPTOR_MIN = ACTIAS_STDIN,
    ACTIAS_STD_DESCRIPTOR_MAX = ACTIAS_STDERR,
};

//! \brief Get a standard file handle.
//!
//! \param descriptor - Standard file descriptor to the handle for.
//! \param pHandle - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(Int32 descriptor, ActiasHandle* pHandle);

//! \brief Opens a file with the specified filename and flags.
//!
//! \param filename - The name of the file to open.
//! \param flags - The flags specifying the mode of opening the file.
//!
//! \return If successful, returns a file handle. On failure, returns NULL.
ACTIAS_SYSTEM_API ActiasHandle ACTIAS_ABI ActiasOpen(const char* filename, ActiasFlags flags);

//! \brief Read to a file handle.
//!
//! \param fileHandle - The file handle to read to.
//! \param pBuffer - A pointer to the buffer with the data to be read.
//! \param byteCount - The number of bytes to be read.
//! \param pBytesRead - A pointer to the variable that receives the number of bytes read, can be NULL.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasRead(ActiasHandle fileHandle, void* pBuffer, USize byteCount,
                                                     USize* pBytesRead);

//! \brief Write to a file handle.
//!
//! \param fileHandle - The file handle to write to.
//! \param pBuffer - A pointer to the buffer with the data to be written.
//! \param byteCount - The number of bytes to be written.
//! \param pBytesWritten - A pointer to the variable that receives the number of bytes written, can be NULL.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasWrite(ActiasHandle fileHandle, const void* pBuffer, USize byteCount,
                                                      USize* pBytesWritten);

ACTIAS_END_C
