#include <Actias/System/Atomic.h>
#include <Actias/System/Streams.h>
#include <cassert>
#include <limits>

int main()
{
    const char message[] = "Hello, World!";

    ActiasHandle handle;
    auto fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    ACTIAS_UNUSED(fileHandleResult);
    assert(fileHandleResult == ACTIAS_SUCCESS);

    auto writeResult = ActiasWrite(handle, message, sizeof(message), nullptr);
    ACTIAS_UNUSED(writeResult);
    assert(writeResult == ACTIAS_SUCCESS);
}
