#include <Actias/IO/Console.hpp>
#include <Actias/System/Assert.h>
#include <Actias/Time/DateTime.hpp>

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

    const UTCDateTime utcNow = UTCDateTime::Now();
    const FixStr128 nowISO   = utcNow.ToString(DateTimeFormatKind::ISO8601);
    const FixStr128 nowShort = utcNow.ToString(DateTimeFormatKind::Short);
    const FixStr128 nowLong  = utcNow.ToString(DateTimeFormatKind::Long);

    IO::Console::WriteLine("Current UTC time is:");
    IO::Console::Write("ISO:        ");
    IO::Console::WriteLine(nowISO);
    IO::Console::Write("Short form: ");
    IO::Console::WriteLine(nowShort);
    IO::Console::Write("Long form:  ");
    IO::Console::WriteLine(nowLong);

    const LocalDateTime localNow = DateTimeConvert::ToLocal(utcNow);

    IO::Console::WriteLine("Current Local time is:");
    IO::Console::Write("ISO:        ");
    IO::Console::WriteLine(localNow.ToString(DateTimeFormatKind::ISO8601));

    return ACTIAS_SUCCESS;
}
