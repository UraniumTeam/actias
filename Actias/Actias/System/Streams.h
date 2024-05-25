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
#include <Actias/System/Core.h>
#include <Actias/System/Path.h>
#include <Actias/System/Time.h>

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

enum ActiasFileOpenModeValues
{
    ACTIAS_FILE_OPEN_MODE_NONE       = 0, //!< None mode.
    ACTIAS_FILE_OPEN_MODE_READ_ONLY  = 1, //!< Read only open mode.
    ACTIAS_FILE_OPEN_MODE_WRITE_ONLY = 2, //!< Write only open mode.
    ACTIAS_FILE_OPEN_MODE_APPEND     = 3, //!< Append open mode.
    ACTIAS_FILE_OPEN_MODE_CREATE     = 4, //!< Create (new or rewrite existing) open mode.
    ACTIAS_FILE_OPEN_MODE_CREATE_NEW = 5, //!< Create (new only) open mode.
    ACTIAS_FILE_OPEN_MODE_TRUNCATE   = 6, //!< Truncate open mode.
    ACTIAS_FILE_OPEN_MODE_READ_WRITE = 7, //!< Read and write open mode.
};

typedef UInt32 ActiasFileOpenMode;

enum ActiasFileSeekModeValues
{
    ACTIAS_FILE_SEEK_MODE_BEGIN   = 0, //!< Begin seek mode.
    ACTIAS_FILE_SEEK_MODE_CURRENT = 1, //!< Current seek mode.
    ACTIAS_FILE_SEEK_MODE_END     = 2, //!< End seek mode.
};

typedef UInt32 ActiasFileSeekMode;

typedef struct ActiasFileStats
{
    ActiasTime CreationTimeUTC;
    ActiasTime LastAccessTimeUTC;
    ActiasTime LastModificationTimeUTC;
    UInt64 ByteSize;
} ActiasFileStats;

//! \brief Get a standard file handle.
//!
//! \param descriptor - Standard file descriptor to the handle for.
//! \param pHandle    - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(Int32 descriptor, ActiasHandle* pHandle);

//! \brief Opens a file with the specified path and mode, returning a handle to the opened file.
//!
//! \param filePath - The path to the file to open, in UTF-8 encoding.
//! \param openMode - The mode in which to open the file.
//! \param pHandle  - A pointer to a variable that will receive the handle to the opened file.
//!
//! \return ActiasResult indicating the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasOpenFile(const char* filePath, ActiasFileOpenMode openMode,
                                                         ActiasHandle* pHandle);

//! \brief Closes the file associated with the provided file handle.
//!
//! \param fileHandle - The handle of the file to close.
//!
//! \return ActiasResult indicating the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasCloseFile(ActiasHandle fileHandle);

//! \brief Moves the file pointer associated with the file handle to a new position.
//!
//! \param fileHandle - The handle of the file whose pointer is to be moved.
//! \param offset     - The new file pointer position, relative to the seek mode.
//! \param seekMode   - Specifies the reference point used to seek the new position.
//!
//! \return ActiasResult indicating the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasSeekFile(ActiasHandle fileHandle, USize offset, ActiasFileSeekMode seekMode);

//! \brief Retrieves the current position of the file pointer associated with the file handle.
//!
//! \param fileHandle - The handle of the file whose current position is to be retrieved.
//! \param pPosition  - A pointer to a variable that will receive the current position of the file pointer.
//!
//! \return ActiasResult indicating the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasTellFile(ActiasHandle fileHandle, USize* pPosition);

//! \brief Reads data from the file associated with the provided file handle into a buffer.
//!
//! \param fileHandle - The handle of the file from which to read.
//! \param pBuffer    - A pointer to the buffer where the data will be read into.
//! \param byteCount  - The number of bytes to read from the file.
//! \param pBytesRead - A pointer to a variable that will receive the number of bytes actually read, or NULL.
//!
//! \return ActiasResult indicating the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasReadFile(ActiasHandle fileHandle, void* pBuffer, USize byteCount,
                                                         USize* pBytesRead);

//! \brief Write data to the file associated with the provided file handle from a buffer.
//!
//! \param fileHandle    - The handle of the file to write the data to.
//! \param pBuffer       - A pointer to the buffer with the data to be written.
//! \param byteCount     - The number of bytes to be written.
//! \param pBytesWritten - A pointer to the variable that receives the number of bytes actually written, can be NULL.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasWriteFile(ActiasHandle fileHandle, const void* pBuffer, USize byteCount,
                                                          USize* pBytesWritten);

//! \brief Get an ActiasFileStats for a specified file.
//!
//! \param fileHandle - The handle of the file to get stats for.
//! \param pResult    - A pointer to the variable that receives the resulting stats.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasGetFileStats(ActiasHandle fileHandle, ActiasFileStats* pResult);

//! \brief Remove a file from disk.
//!
//! \param filePath - The path to the file to delete, in UTF-8 encoding.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasRemoveFile(const char* filePath);

ACTIAS_END_C
