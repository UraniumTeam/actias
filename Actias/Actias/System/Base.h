/*
 * Copyright 2023 the Actias project authors (see the file AUTHORS
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
#include <Actias/System/Platform.h>

#define ACTIAS_DEFAULT_ALIGNMENT 16

#if ActiasSystem_EXPORTS
#    define ACTIAS_SYSTEM_API ACTIAS_EXPORT
#else
#    define ACTIAS_SYSTEM_API ACTIAS_IMPORT
#endif
