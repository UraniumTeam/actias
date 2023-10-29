/*
 * Actias SDK System
 * 
 * File and standard stream library
 * 
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

//! \brief Standard file descriptors (stdin, stdout, stderr)
typedef enum ActiasStandardDescriptor
{
    ACTIAS_STDIN  = 0x0000, //!< Standard input file descriptor.
    ACTIAS_STDOUT = 0x0001, //!< Standard output file descriptor.
    ACTIAS_STDERR = 0x0002  //!< Standard error file descriptor.
} ActiasStandardDescriptor;

//! \brief Get a standard file handle.
//!
//! \param descriptor - Standard file descriptor to the handle for.
//! \param pHandle - The pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasGetStdFileHandle(ActiasStandardDescriptor descriptor, ActiasHandle* pHandle);

//! \brief Write to a file handle.
//!
//! \param fileHandle - The file handle to write to.
//! \param pBuffer - A pointer to the buffer with the data to be written.
//! \param byteCount - The number of bytes to be written.
//! \param pBytesWritten - The pointer to the variable that receives the number of bytes written, can be NULL.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasWrite(ActiasHandle fileHandle, ACTIAS_CONST void* pBuffer, USize byteCount,
                                                      USize* pBytesWritten);

ACTIAS_END_C
