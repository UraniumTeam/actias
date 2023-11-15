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

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasInitializeMutex(ActiasMutex* pMutex);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasReleaseMutex(ActiasMutex* pMutex);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasLockMutex(ActiasMutex* pMutex);

ACTIAS_SYSTEM_API ActiasBool ACTIAS_ABI ActiasTryLockMutex(ActiasMutex* pMutex);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasUnlockMutex(ActiasMutex* pMutex);

ACTIAS_END_C
