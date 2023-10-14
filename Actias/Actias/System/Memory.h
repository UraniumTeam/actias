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

//! \brief Allocate uninitialized memory.
//!
//! It is a replacement for standard C malloc() function in the Actias SDK.
//!
//! \param byteSize - Size of the storage to allocate in bytes.
//!
//! \return A pointer to the allocated storage if the allocation succeeds, NULL otherwise.
void* ActiasAlloc(USize byteSize);

//! \brief Reallocate the given block of memory.
//!
//! It is a replacement for standard C realloc() function in the Actias SDK.
//!
//! \param pointer - Pointer to the memory to be reallocated.
//! \param newByteSize - New size of the storage in bytes.
//!
//! \return A pointer to the reallocated storage if the allocation succeeds, NULL otherwise.
void* ActiasRealloc(void* pointer, USize newByteSize);

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
void* ActiasAlignedAlloc(USize byteAlignment, USize byteSize);

//! \brief Deallocate memory.
//!
//! Deallocate the space allocated previously with ActiasAlloc.
//! It is a replacement for standard C free() function in the Actias SDK
//! (but not for the aligned version).
//!
//! \param pointer - Pointer to the memory to deallocate.
void ActiasFree(void* pointer);

//! \brief Deallocate memory.
//!
//! Deallocate the space allocated previously with ActiasAlignedAlloc.
//! It is a replacement for standard C free() function in the Actias SDK
//! (but only for the aligned version).
//!
//! \param pointer - Pointer to the memory to deallocate.
void ActiasAlignedFree(void* pointer);

ACTIAS_END_C
