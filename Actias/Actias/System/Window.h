#pragma once

#include "../../../cmake-build/windows-debug-msvc-avx/_deps/glfw-src/include/GLFW/glfw3.h"
//#include <GLFW/glfw3.h>
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

typedef struct ActiasWindowStruct* ActiasWindow;
typedef struct ActiasWindowMonitorStruct* ActiasWindowMonitor;

ACTIAS_SYSTEM_API ActiasWindow ACTIAS_ABI ActiasCreateWindow(Int32 width, Int32 height, const char* title,
                                                             ActiasWindowMonitor monitor, ActiasWindow share);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasPollEvents();

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasDestroyWindow(ActiasWindow window);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasTerminate();

ACTIAS_END_C
