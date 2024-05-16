#include <Actias/IO/StdoutStream.hpp>
#include <Actias/System/Assert.h>
#include <Actias/System/Streams.h>
#include <corecrt_startup.h>

using namespace Actias;

// TODO: move somewhere, is needed for compatibility
void* memcpy(void* dst, const void* src, size_t size)
{
    ActiasInlineCopyMemory(dst, src, size);
    return dst;
}

static ActiasResult Write(const char* message, USize length)
{
    Ptr stream = AllocateObject<IO::StdoutStream>();
    stream->WriteFromBuffer(message, length).Unwrap();
    return ACTIAS_SUCCESS;
}

struct Test
{
    Test()
    {
        const char message[] = "Hello from Test (statically initialized)!\n";
        Write(message, sizeof(message));
    }
};

static Test g_Test{};

static ActiasResult EntryPoint()
{
    const char message[] = "Hello, World!\n";

    List<char> msg;
    for (char c : message)
    {
        msg.Push(c);
    }
    msg.Push(0);

    return Write(msg.Data(), msg.Size());
}

extern "C"
{
    extern _PIFV __xi_a[];
    extern _PIFV __xi_z[];
    extern _PVFV __xc_a[];
    extern _PVFV __xc_z[];
}

static void win32_crt_call(_PIFV* a, _PIFV* b)
{
    while (a != b)
    {
        if (*a)
        {
            (**a)();
        }
        ++a;
    }
}

static void win32_crt_call(_PVFV* a, _PVFV* b)
{
    while (a != b)
    {
        if (*a)
        {
            (**a)();
        }
        ++a;
    }
}

static void win32_crt_init()
{
    win32_crt_call(__xi_a, __xi_z);
    win32_crt_call(__xc_a, __xc_z);
}

extern "C" ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasMain()
{
    win32_crt_init();
    return EntryPoint();
}
