#include <Actias/System/Window.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

ActiasResult ACTIAS_ABI ActiasInitWindows()
{
    int result = glfwInit();
    if (result == GLFW_TRUE)
    {
        return ACTIAS_SUCCESS;
    }

    return ACTIAS_FAIL_UNKNOWN;
}

void ACTIAS_ABI ActiasShutdownWindows()
{
    glfwTerminate();
}

ActiasWindow ACTIAS_ABI ActiasCreateWindow(Int32 width, Int32 height, const char* title, ActiasWindowMonitor monitor,
                                           ActiasWindow share)
{
    return (ActiasWindow)glfwCreateWindow(width, height, title, (GLFWmonitor*)monitor, (GLFWwindow*)share);
}

void ACTIAS_ABI ActiasPollEvents()
{
    glfwPollEvents();
}

ActiasBool ActiasWindowShouldClose(ActiasWindow window)
{
    return glfwWindowShouldClose((GLFWwindow*)window) ? ACTIAS_TRUE : ACTIAS_FALSE;
}

void ACTIAS_ABI ActiasDestroyWindow(ActiasWindow window)
{
    glfwDestroyWindow((GLFWwindow*)window);
}

void ACTIAS_ABI ActiasTerminate()
{
    glfwTerminate();
}
