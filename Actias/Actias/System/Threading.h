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

typedef ActiasResult(ACTIAS_ABI* ActiasThreadProc)(void* pParameter);

typedef struct ActiasThreadCreateInfo
{
    USize StackSize;               //!< Initial size of the created thread's stack, optional.
    ActiasThreadProc StartRoutine; //!< A pointer to a function to be executed by the created thread.
    void* StartParameter;          //!< A pointer to the data to be passed to the thread, optional.
    const char* pName;             //!< Thread debug name, optional.
    USize NameLength;              //!< Thread name length in bytes, 0 if null terminated.
} ActiasThreadCreateInfo;

typedef struct ActiasThreadInfo
{
    ActiasHandle Handle; //!< Thread's handle.
    UInt64 ID;           //!< Thread's ID.
} ActiasThreadInfo;

//! \brief Create a thread.
//!
//! \param pCreateInfo - A pointer to ActiasThreadCreateInfo to use to create the thread.
//! \param pResult - A pointer to the variable that receives the ActiasThreadInfo for the created thread.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasCreateThread(const ActiasThreadCreateInfo* pCreateInfo,
                                                             ActiasThreadInfo* pResult);

//! \brief Set name for a thread.
//!
//! \param threadHandle - The handle of the thread to set name for.
//! \param pName - The name to set.
//! \param nameLength - The length of the name in bytes, 0 if null terminated.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasSetThreadName(ActiasHandle threadHandle, const char* pName, USize nameLength);

//! \brief Release a thread created with ActiasCreateThread.
//!
//! \param threadHandle - The handle of the thread to release.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasReleaseThread(ActiasHandle threadHandle);

//! \brief Wait for a single thread to finish execution.
//!
//! \param threadHandle - The handle of the thread to wait for.
//! \param millisecondTimeout - Timeout in milliseconds.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasWaitForThread(ActiasHandle threadHandle, UInt64 millisecondTimeout);

ACTIAS_END_C
