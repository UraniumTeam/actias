#include <Actias/System/Assert.h>
#include <Actias/System/Window.h>
#include <Actias/System/ActiasVulkan.h>

extern "C" ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasMain()
{
    ACTIAS_Assert(ActiasInitWindows() == ACTIAS_SUCCESS);

    ActiasWindow window = ActiasCreateWindow(800, 600, "Hello from Actias Window", nullptr, nullptr);
    ACTIAS_Assert(window);

    while (!ActiasWindowShouldClose(window))
    {
        ActiasPollEvents();
    }

    ActiasDestroyWindow(window);
    ActiasShutdownWindows();

    return ACTIAS_SUCCESS;
}
