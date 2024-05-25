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

//! \brief Maximum supported length of a file path, in bytes.
#define ACTIAS_MAX_PATH 260

//! \brief Replace platform-specific path separators (like '\') with a '/'.
//!
//! \param pPath        - Source path, in UTF-8 encoding.
//! \param pathByteSize - Length of the provided path, in bytes.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasNormalizePath(char* pPath, USize pathByteSize);

//! \brief Get current working directory of the process, in UTF-8 encoding.
//!
//! The function returns ACTIAS_FAIL_INSUFFICIENT_SIZE on buffer overflow.
//!
//! \param pBuffer        - A buffer to write the directory path to.
//! \param bufferByteSize - Size of the buffer, including null-terminator.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasGetCurrentDirectory(char* pBuffer, USize bufferByteSize);

ACTIAS_END_C
