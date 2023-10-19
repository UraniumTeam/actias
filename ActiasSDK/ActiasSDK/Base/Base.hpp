#pragma once
#include <Actias/System/Core.h>

#if ActiasSDK_EXPORTS
#    define ACTIAS_SDK_API ACTIAS_EXPORT
#else
#    define ACTIAS_SDK_API ACTIAS_IMPORT
#endif
