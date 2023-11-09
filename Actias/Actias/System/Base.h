#pragma once
#include <Actias/Base/Platform.h>

#if ActiasSystem_EXPORTS
#    define ACTIAS_SYSTEM_API ACTIAS_EXPORT
#else
#    define ACTIAS_SYSTEM_API ACTIAS_IMPORT
#endif
