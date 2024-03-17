#include <Actias/System/Assert.h>
#include <Actias/System/Window.h>
#include <limits>

int main()
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
}
