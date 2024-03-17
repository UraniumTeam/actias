#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

typedef struct ActiasWindowStruct* ActiasWindow;
typedef struct ActiasWindowMonitorStruct* ActiasWindowMonitor;

ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasInitWindows();

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasShutdownWindows();

ACTIAS_SYSTEM_API ActiasWindow ACTIAS_ABI ActiasCreateWindow(Int32 width, Int32 height, const char* title,
                                                             ActiasWindowMonitor monitor, ActiasWindow share);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasPollEvents();

ACTIAS_SYSTEM_API ActiasBool ActiasWindowShouldClose(ActiasWindow window);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasDestroyWindow(ActiasWindow window);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasTerminate();

ACTIAS_END_C
