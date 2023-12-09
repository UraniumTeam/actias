#include <Actias/System/CRT/EntryPoint.h>
#include <Actias/System/Streams.h>

ACTIAS_EXPORT Int32 ACTIAS_ABI ActiasMain(const char* pCommandLine)
{
    ACTIAS_Unused(pCommandLine);
    return 0;
}

ACTIAS_EXPORT Int32 ACTIAS_ABI ActiasLibraryMain()
{
    const char message[] = "Hello from ActiasLibraryMain!\n";

    ActiasHandle handle;
    ActiasResult fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    if (fileHandleResult != ACTIAS_SUCCESS)
    {
        return 1;
    }

    ActiasResult writeResult = ActiasWrite(handle, message, sizeof(message), NULL);
    if (writeResult != ACTIAS_SUCCESS)
    {
        return 1;
    }

    return 0;
}
