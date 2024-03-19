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
#include <Actias/ACBX/Loader.h>
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

//! \brief Initialize Actias runtime.
//!
//! This function is not thread safe, it must be called once from the main thread
//! when the program is loaded.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasInit(void);

//! \brief Load a dynamic module (OS-native only).
//!
//! \param pFilePath - A pointer to a null-terminated string containing the path to the module to load.
//! \param pHandle - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasLoadNativeModule(const char* pFilePath, ActiasHandle* pHandle);

//! \brief Request the runtime to unload a dynamic module (OS-native only).
//!
//! \param moduleHandle - Handle of the module to unload.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasUnloadNativeModule(ActiasHandle moduleHandle);

//! \brief Find the address of a symbol from a dynamic module (OS-native only).
//!
//! \param moduleHandle - A handle returned from ActiasLoadNativeModule.
//! \param pSymbolName - The name of the symbol to find.
//! \param pAddress - A pointer to the variable that receives the resulting symbol address.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasFindNativeSymbolAddress(ActiasHandle moduleHandle, const char* pSymbolName,
                                                                        ActiasProc* pAddress);

//! \brief Request the runtime to load a dynamic module (ACBX only).
//!
//! \param pRunInfo - A pointer to ACBXLoaderRunInfo to use to load the module.
//! \param pModuleHandle - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasLoadModuleEx(const ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pModuleHandle);

//! \brief Request the runtime to unload a dynamic module (ACBX only).
//!
//! \param moduleHandle - Handle of the module to unload.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasUnloadModule(ActiasHandle moduleHandle);

//! \brief Request the runtime to load a dynamic module (ACBX only).
//!
//! \param pFilePath - A pointer to a null-terminated string containing the path to the module to load.
//! \param pHandle - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
inline ActiasResult ACTIAS_ABI ActiasLoadModule(const char* pFilePath, ActiasHandle* pHandle)
{
    ACBXLoaderRunInfo runInfo;
    ActiasInlineZeroMemory(&runInfo, sizeof(runInfo));
    runInfo.pModulePath = pFilePath;
    return ActiasLoadModuleEx(&runInfo, pHandle);
}

//! \brief Find the address of a symbol from a dynamic module (ACBX only).
//!
//! \param moduleHandle - A handle returned from ActiasLoadModule or ActiasLoadModuleEx.
//! \param pSymbolName - The name of the symbol to find.
//! \param pAddress - A pointer to the variable that receives the resulting symbol address.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasFindSymbolAddress(ActiasHandle moduleHandle, const char* pSymbolName,
                                                                  ActiasProc* pAddress);

ACTIAS_END_C
