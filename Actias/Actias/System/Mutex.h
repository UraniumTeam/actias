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
