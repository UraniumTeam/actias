#pragma once
#include <Actias/System/Platform.h>

#if ActiasSystemShared_EXPORTS
#    define ACTIAS_SYSTEM_API ACTIAS_EXPORT
#else
#    define ACTIAS_SYSTEM_API ACTIAS_IMPORT
#endif
