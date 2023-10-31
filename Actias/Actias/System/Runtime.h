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

//! \brief Request the runtime to load a dynamic module (ACBX only).
//!
//! \param pFilePath - A pointer to a null-terminated string containing the path to the module to load.
//! \param pHandle - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasLoadModule(ACTIAS_CONST char* pFilePath, ActiasHandle* pHandle);

//! \brief Request the runtime to load a dynamic module (ACBX only).
//!
//! \param pRunInfo - A pointer to ACBXLoaderRunInfo to use to load the module.
//! \param pHandle - A pointer to the variable that receives the resulting handle.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasLoadModuleEx(ACTIAS_CONST ACBXLoaderRunInfo* pRunInfo, ActiasHandle* pHandle);

ACTIAS_END_C
