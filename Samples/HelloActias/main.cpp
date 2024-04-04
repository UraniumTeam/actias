#include <Actias/System/Streams.h>

extern "C" ACTIAS_EXPORT ActiasResult ACTIAS_ABI ActiasMain()
{
    const char message[] = "Hello, World!\n";

    ActiasHandle handle;
    const auto fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    if (fileHandleResult != ACTIAS_SUCCESS)
    {
        return fileHandleResult;
    }

    const auto writeResult = ActiasWrite(handle, message, sizeof(message), nullptr);
    return writeResult;
}
