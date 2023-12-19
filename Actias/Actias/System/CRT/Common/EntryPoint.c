#include <Actias/System/CRT/EntryPoint.h>
#include <Actias/System/Streams.h>

#if ACTIAS_WINDOWS
#    define ACTIAS_MAIN_CALL __cdecl
#elif ACTIAS_LINUX
#    define ACTIAS_MAIN_CALL
#else
#    error Platform not supported
#endif

ACTIAS_EXPORT Int32 ACTIAS_ABI ActiasMain(ActiasMainCallReason callReason, const char* pCommandLine)
{
    ACTIAS_Unused(callReason);
    ACTIAS_Unused(pCommandLine);

    // TODO: unreferenced symbol "main" when compiling a shared library
    //
    // switch (callReason)
    // {
    // case ACTIAS_MAIN_CALL_REASON_EXECUTABLE_START:
    //     return main(0, NULL);
    // case ACTIAS_MAIN_CALL_REASON_EXECUTABLE_EXIT:
    //     return 0;
    // default:
    //     return -1;
    // }

    return 0;
}

ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasLibraryMain(ActiasMainCallReason reason, ActiasHandle handle)
{
    ACTIAS_Unused(handle);

    const char messageStart[] = "Hello from ActiasLibraryMain!\n";
    const char messageExit[]  = "Goodbye from ActiasLibraryMain!\n";

    ActiasHandle stdout;
    ActiasResult fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &stdout);
    if (fileHandleResult != ACTIAS_SUCCESS)
    {
        return fileHandleResult;
    }

    const char* message = reason == ACTIAS_MAIN_CALL_REASON_LIBRARY_LOAD ? messageStart : messageExit;
    USize size          = reason == ACTIAS_MAIN_CALL_REASON_LIBRARY_LOAD ? sizeof(messageStart) : sizeof(messageExit);

    ActiasResult writeResult = ActiasWrite(stdout, message, size, NULL);
    if (writeResult != ACTIAS_SUCCESS)
    {
        return writeResult;
    }

    return ACTIAS_SUCCESS;
}
