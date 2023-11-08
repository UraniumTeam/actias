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
    ACTIAS_MEMORY_PROTECTION_NONE = 0x0,

    ACTIAS_MEMORY_PROTECTION_READ_BIT    = 0x0001, //!< Memory can be read.
    ACTIAS_MEMORY_PROTECTION_WRITE_BIT   = 0x0002, //!< Memory can be written to.
    ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT = 0x0004, //!< Memory can be executed as code.

    ACTIAS_MEMORY_PROTECTION_READ_WRITE =
        ACTIAS_MEMORY_PROTECTION_READ_BIT | ACTIAS_MEMORY_PROTECTION_WRITE_BIT, //!< Memory can be read and written to.
    ACTIAS_MEMORY_PROTECTION_READ_EXECUTE =
        ACTIAS_MEMORY_PROTECTION_READ_BIT | ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT, //!< Memory can be read and executed as code.
    ACTIAS_MEMORY_PROTECTION_READ_WRITE_EXECUTE = ACTIAS_MEMORY_PROTECTION_READ_WRITE
        | ACTIAS_MEMORY_PROTECTION_EXECUTE_BIT, //!< Memory can be read, written to and executed as code.

    ACTIAS_MEMORY_PROTECTION_ALL =
        ACTIAS_MEMORY_PROTECTION_READ_WRITE_EXECUTE, //!< Memory can be read, written to and executed as code.
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

//! \brief Change the protection on a region of pages allocated with a single call to ActiasVirtualAlloc.
//!
//! The function changes access protection for the memory of the calling process. All pages containing any
//! bytes from the provided region are affected by the call.
//!
//! \param pointer - A pointer to the beginning of the provided region.
//! \param byteSize - The size of the provided region in bytes.
//! \param protection - The desired memory protection.
//!
//! \note On Windows the pages cannot span adjacent reserved regions that were allocated by separate calls
//!       to ActiasVirtualAlloc.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasVirtualProtect(void* pointer, USize byteSize, ActiasFlags protection);

ACTIAS_END_C
