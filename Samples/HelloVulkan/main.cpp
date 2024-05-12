#include <Actias/System/ActiasVulkan.h>
#include <Actias/System/Assert.h>
#include <Actias/System/Window.h>

extern "C" ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasMain()
{
    ACTIAS_Assert(ActiasInitWindows() == ACTIAS_SUCCESS);

    volkInitialize();

    VkApplicationInfo appInfo{};
    appInfo.sType      = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 1, 0);

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    VkInstance instance;
    const VkResult instanceResult = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    ACTIAS_Assert(instanceResult == VK_SUCCESS);

    volkLoadInstance(instance);

    ActiasWindow window = ActiasCreateWindow(800, 600, "Hello from Actias Window", nullptr, nullptr);
    ACTIAS_Assert(window);

    while (!ActiasWindowShouldClose(window))
    {
        ActiasPollEvents();
    }

    vkDestroyInstance(instance, nullptr);

    ActiasDestroyWindow(window);
    ActiasShutdownWindows();

    return ACTIAS_SUCCESS;
}

int main()
{
    ActiasMain();
}
