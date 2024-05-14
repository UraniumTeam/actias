#include <Actias/System/Assert.h>
#include <Actias/System/Streams.h>
#include <corecrt_startup.h>

static ActiasResult Write(const char* message, USize length)
{
    ActiasHandle handle;
    const auto fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    if (fileHandleResult != ACTIAS_SUCCESS)
    {
        return fileHandleResult;
    }

    const auto writeResult = ActiasWrite(handle, message, length, nullptr);
    return writeResult;
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
    return Write(message, sizeof(message));
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
    Write("*", 1);
    win32_crt_call(__xi_a, __xi_z);
    Write("*", 1);
    win32_crt_call(__xc_a, __xc_z);
}

extern "C" ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasMain()
{
    win32_crt_init();
    Write("*", 1);
    const ActiasResult result = EntryPoint();
    Write("*", 1);
    return result;
}
