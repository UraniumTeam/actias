#pragma once

#define GLFW_INCLUDE_NONE

#include <Actias/System/Core.h>
#include "../../../cmake-build/windows-debug-msvc-avx/_deps/glfw-src/include/GLFW/glfw3.h"

ACTIAS_BEGIN_C

typedef struct ActiasWindow
{
    GLFWwindow* window;
} ActiasWindow;

typedef struct ActiasWindowMonitor
{
    GLFWmonitor* monitor;
} ActiasWindowMonitor;

ACTIAS_SYSTEM_API ActiasWindow ACTIAS_ABI ActiasCreateWindow(Int32 width, Int32 height, const char* title,
                                                             ActiasWindowMonitor monitor, ActiasWindow share);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasPollEvents();

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasDestroyWindow(ActiasWindow window);

ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasTerminate();

ACTIAS_END_C
