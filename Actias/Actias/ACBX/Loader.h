/*
 * Actias SDK System
 * 
 * Binary executable loader library
 * 
 * Copyright (c) 2023 UraniumTeam
 */

#pragma once
#include <Actias/Base/CompilerTraits.h>

ACTIAS_BEGIN_C

//! \brief Represents a set of parameters used by ACBX runtime loader.
typedef struct ACBXLoaderRunInfo
{
    const char* pModulePath; //!< A path to the module to load.
} ACBXLoaderRunInfo;

ACTIAS_END_C
