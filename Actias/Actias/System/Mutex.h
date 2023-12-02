/*
 * Actias SDK System
 * 
 * Synchronization primitives library
 * 
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

typedef struct ActiasMutex
{
    // This field contains OS-defined critical section data.
    // Both Windows CRITIAL_SECTION and POSIX pthread_mutex_t occupy 40 bytes of memory.
    // We store them in 5 8-byte integers to also keep maximum alignment.
    UInt64 DummyData[5];
} ActiasMutex;

//! \brief Initialize a mutex.
//!
//! \param pMutex - A pointer to the mutex to initialize.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasInitializeMutex(ActiasMutex* pMutex);

//! \brief Destroy a mutex.
//!
//! \param pMutex - A pointer to the mutex to destroy.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasReleaseMutex(ActiasMutex* pMutex);

//! \brief Lock a mutex, block current thread if not available.
//!
//! \param pMutex - A pointer to the mutex to lock.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasLockMutex(ActiasMutex* pMutex);

//! \brief Try to lock a mutex, return if not available.
//!
//! \param pMutex - A pointer to the mutex to lock.
//!
//! \return True on success.
ACTIAS_SYSTEM_API ActiasBool ACTIAS_ABI ActiasTryLockMutex(ActiasMutex* pMutex);

//! \brief Unlock a previously locked mutex.
//!
//! \param pMutex - A pointer to the mutex to unlock.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasUnlockMutex(ActiasMutex* pMutex);

ACTIAS_END_C
