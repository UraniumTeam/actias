#include <Actias/System/Window.h>
//#include <stdlib.h>

ActiasWindow ACTIAS_ABI ActiasCreateWindow(Int32 width, Int32 height, const char* title, ActiasWindowMonitor monitor,
                                           ActiasWindow share)
{
    return (ActiasWindow)glfwCreateWindow(width, height, title, (GLFWmonitor*)monitor, (GLFWwindow*)share);
}

void ACTIAS_ABI ActiasPollEvents()
{
    glfwPollEvents();
}

void ACTIAS_ABI ActiasDestroyWindow(ActiasWindow window)
{
    glfwDestroyWindow((GLFWwindow*)window);
}

void ACTIAS_ABI ActiasTerminate()
{
    glfwTerminate();
}
