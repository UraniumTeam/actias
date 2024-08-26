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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
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

VkResult ACTIAS_ABI ActiasCreateWindowSurface(VkInstance instance, ActiasWindow window, const VkAllocationCallbacks* pAllocator,
                                              VkSurfaceKHR* pSurface)
{
    return glfwCreateWindowSurface(instance, (GLFWwindow*)window, pAllocator, pSurface);
}

void ACTIAS_ABI ActiasGetFramebufferSize(ActiasWindow window, Int32* pWidth, Int32* pHeight)
{
    glfwGetFramebufferSize((GLFWwindow*)window, pWidth, pHeight);
}

const char** ACTIAS_ABI ActiasGetRequiredInstanceExtensions(UInt32* pExtensionCount)
{
    return glfwGetRequiredInstanceExtensions(pExtensionCount);
}
