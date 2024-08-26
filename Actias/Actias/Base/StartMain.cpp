#include <Actias/IO/Console.hpp>
#include <Actias/System/Core.h>
#include <Actias/System/Runtime.h>

using namespace Actias;

#if _WIN32

#    include <corecrt_startup.h>

//
// We are compiling natively on a Windows system, so we use Windows ABI for static initialization.
// I tested on Linux - it still works! Seems like these don't call any win32-specific APIs, just the initializers.
//

extern "C"
{
    extern _PIFV __xi_a[];
    extern _PIFV __xi_z[];
    extern _PVFV __xc_a[];
    extern _PVFV __xc_z[];
}

template<class TFunc>
static void DoInitTerm(TFunc* begin, TFunc* end)
{
    while (begin != end)
    {
        if (*begin)
        {
            (**begin)();
        }
        ++begin;
    }
}

static void DoStaticInitialization()
{
    DoInitTerm(__xi_a, __xi_z);
    DoInitTerm(__xc_a, __xc_z);
}

#else

static void DoStaticInitialization() {}

#endif

//
// Calls to these functions can be generated by the compiler, so we have to define them anyway.
//

extern "C" void* memcpy(void* dest, const void* src, size_t size)
{
    ActiasCopyMemory(dest, src, size);
    return dest;
}

extern "C" void* memset(void* dest, int value, size_t size)
{
    ActiasSetMemory(dest, static_cast<UInt8>(value), size);
    return dest;
}

extern "C" USize strlen(const char* str)
{
    return ActiasInlineStringLength(str);
}

extern "C" ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasStartMain(ActiasHandle moduleHandle)
{
    using ActiasMainProc = ActiasResult(ACTIAS_ABI*)();

    // Call compiler ABI specific static initialization functions.
    DoStaticInitialization();

    //
    // Initialize our runtime.
    // This will make a call to the ActiasSystem that will load a native ActiasRuntime shared library
    // for the platform we are currently running on.
    //

    const ActiasResult initResult = ActiasInit(moduleHandle);

    //
    // Runtime could have been already initialized, in this case we should not call shutdown.
    // TODO: probably we should use a reference counter in ActiasInit/Shutdown.
    //       Also what if we don't have a main function? How do we know when to call shutdown for a shared library?
    //       What if someone calls main() on an executable and then loads some of the exported functions?
    //

    const bool doShutdown = initResult != ACTIAS_FAIL_RT_INITIALIZED;

    if (initResult != ACTIAS_SUCCESS && initResult != ACTIAS_FAIL_RT_INITIALIZED)
    {
        // Probably the only reason for this to fail is that the runtime library is not in the
        // process working directory.
        return initResult;
    }

    ActiasProc mainProc;
    const ActiasResult findResult = ActiasFindSymbolAddress(moduleHandle, "ActiasMain", &mainProc);
    if (findResult != ACTIAS_SUCCESS)
    {
        //
        // ActiasMain function is not always required, e.g. shared libraries won't have it in most cases.
        // But it is required for executables, so we don't always return ACTIAS_SUCCESS to allow the caller
        // to handle this result as an error.
        //

        return ACTIAS_NO_USER_ENTRY_POINT_FOUND;
    }

    //
    // Here we have two functions that can potentially fail, but we can only return one ActiasResult.
    // The logic is as follows:
    //
    // 1. If main fails, we always return the result of main, since it's more important for the user
    //    and the failure occurred in ActiasMain could cause the failure in ActiasShutdown();
    // 2. If main succeeds, we return the result of ActiasShutdown() to let the caller know about
    //    failures that could happen even if the main function exited successfully.
    //
    // In all cases we must call both functions to cleanup references to the runtime libraries and (maybe
    // in future) files that are still opened.
    //
    // TODO: we don't yet handle exceptions or signals of any kind, but we should cleanup even in case of exception.
    //       Now Actias would just crash silently.
    //

    const ActiasResult mainResult     = reinterpret_cast<ActiasMainProc>(mainProc)();
    const ActiasResult shutdownResult = doShutdown ? ActiasShutdown() : ACTIAS_SUCCESS;
    if (mainResult != ACTIAS_SUCCESS)
    {
        return mainResult;
    }

    return shutdownResult;
}
