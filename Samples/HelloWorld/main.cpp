#include <Actias/System/Assert.h>
#include <Actias/System/Atomic.h>
#include <Actias/System/Streams.h>
#include <limits>

int main()
{
    const char message[] = "Hello, World!\n";

    ActiasHandle handle;
    auto fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    ACTIAS_Assert(fileHandleResult == ACTIAS_SUCCESS);

    auto writeResult = ActiasWrite(handle, message, sizeof(message), nullptr);
    ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);
}
