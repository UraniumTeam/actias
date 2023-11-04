/*
 * Actias SDK System
 * 
 * Runtime interoperability library
 * 
 * Copyright (c) 2023 UraniumTeam
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
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasLoadNativeModule(ACTIAS_CONST char* pFilePath, ActiasHandle* pHandle);

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
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasFindNativeSymbolAddress(ActiasHandle moduleHandle, ACTIAS_CONST char* pSymbolName,
                                                                        ActiasProc* pAddress);

//! \brief Request the runtime to load a dynamic module (ACBX only).
//!
//! \param pRunInfo - A pointer to ACBXLoaderRunInfo to use to load the module.
//! \param pModuleHandle - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasLoadModuleEx(ACTIAS_CONST ACBXLoaderRunInfo* pRunInfo,
                                                             ActiasHandle* pModuleHandle);

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
inline ActiasResult ACTIAS_ABI ActiasLoadModule(ACTIAS_CONST char* pFilePath, ActiasHandle* pHandle)
{
    ACBXLoaderRunInfo runInfo;
    ActiasZeroMemory(&runInfo, sizeof(runInfo));
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
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasFindSymbolAddress(ActiasHandle moduleHandle, ACTIAS_CONST char* pSymbolName,
                                                                  ActiasProc* pAddress);

ACTIAS_END_C
