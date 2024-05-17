#include <Actias/IO/Console.hpp>
#include <Actias/System/Assert.h>

using namespace Actias;

struct Test
{
    Test()
    {
        IO::Console::WriteLine("Hello from Test (statically initialized)!");
    }
};

static Test g_Test{};

extern "C" ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasMain()
{
    const char message[] = "Hello, World!";

    List<char> msg;
    for (char c : message)
    {
        msg.Push(c);
    }
    msg.Push(0);

    IO::Console::WriteLine(StringSlice{ msg.Data(), msg.Size() });
    return ACTIAS_SUCCESS;
}
