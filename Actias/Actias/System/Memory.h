/*
 * Actias SDK System
 * 
 * Memory management library
 * 
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

//! \brief Memory protection flags.
typedef enum ActiasMemoryProtectionFlagBits
{
    ACTIAS_MEMORY_PROTECTION_READ_BIT    = 0x0001, //!< Memory can be read.
    ACTIAS_MEMORY_PROTECTION_WRITE_BIT   = 0x0002, //!< Memory can be written to.
    ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT = 0x0004, //!< Memory can be executed as code.

    ACTIAS_MEMORY_PROTECTION_READ_WRITE_BIT =
        ACTIAS_MEMORY_PROTECTION_READ_BIT | ACTIAS_MEMORY_PROTECTION_WRITE_BIT, //!< Memory can be read and written to.
    ACTIAS_MEMORY_PROTECTION_READ_EXECUTE_BIT =
        ACTIAS_MEMORY_PROTECTION_READ_BIT | ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT, //!< Memory can be read and executed as code.
    ACTIAS_MEMORY_PROTECTION_READ_WRITE_EXECUTE_BIT = ACTIAS_MEMORY_PROTECTION_READ_WRITE_BIT
        | ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT, //!< Memory can be read, written to and executed as code.
} ActiasMemoryProtectionFlagBits;

//! \brief Allocate uninitialized memory.
//!
//! It is a replacement for standard C malloc() function in the Actias SDK.
//!
//! \param byteSize - Size of the storage to allocate in bytes.
//!
//! \return A pointer to the allocated storage if the allocation succeeds, NULL otherwise.
ACTIAS_SYSTEM_API void* ACTIAS_ABI ActiasAlloc(USize byteSize);

//! \brief Reallocate the given block of memory.
//!
//! It is a replacement for standard C realloc() function in the Actias SDK.
//!
//! \param pointer - Pointer to the memory to be reallocated.
//! \param newByteSize - New size of the storage in bytes.
//!
//! \return A pointer to the reallocated storage if the allocation succeeds, NULL otherwise.
ACTIAS_SYSTEM_API void* ACTIAS_ABI ActiasRealloc(void* pointer, USize newByteSize);

//! \brief Allocate uninitialized memory with the specified alignment.
//!
//! The size must be a multiple of alignment.
//! It is a replacement for standard C aligned_alloc() function in the Actias SDK.
//!
//! \param byteAlignment - The alignment in bytes.
//! \param byteSize - Size of the storage to allocate in bytes.
//!
//! \note Must be deallocated only with ActiasAlignedFree().
//!
//! \return A pointer to the allocated storage if the allocation succeeds, NULL otherwise.
ACTIAS_SYSTEM_API void* ACTIAS_ABI ActiasAlignedAlloc(USize byteAlignment, USize byteSize);

//! \brief Deallocate memory.
//!
//! Deallocate the space allocated previously with ActiasAlloc.
//! It is a replacement for standard C free() function in the Actias SDK
//! (but not for the aligned version).
//!
//! \param pointer - Pointer to the memory to deallocate.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasFree(void* pointer);

//! \brief Deallocate memory.
//!
//! Deallocate the space allocated previously with ActiasAlignedAlloc.
//! It is a replacement for standard C free() function in the Actias SDK
//! (but only for the aligned version).
//!
//! \param pointer - Pointer to the memory to deallocate.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasAlignedFree(void* pointer);

//! \brief Allocate pages in the virtual address space of the process.
//!
//! \param pointer - The starting address of the memory to allocate, can be NULL.
//! \param byteSize - The size of the memory to allocate in bytes.
//! \param protection - The desired memory protection.
//!
//! \return A pointer to the allocated storage if the allocation succeeds, NULL otherwise.
ACTIAS_SYSTEM_API void* ACTIAS_ABI ActiasVirtualAlloc(void* pointer, USize byteSize, ActiasFlags protection);

//! \brief Deallocate memory previously allocated with ActiasVirtualAlloc.
//!
//! \param pointer - The pointer returned from ActiasVirtualAlloc.
//! \param byteSize - The size of the memory to free in bytes.
//!
//! \note On Windows byteSize is unused, because WinAPI's VirtualFree requires
//!       this parameter to be always 0 when using MEM_RELEASE. But it is required for
//!       mmap() used on Unix systems.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasVirtualFree(void* pointer, USize byteSize);

ACTIAS_END_C
