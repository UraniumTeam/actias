#include <Actias/System/Atomic.h>
#include <Actias/System/Streams.h>
#include <cassert>
#include <limits>

int main()
{
    const char message[] = "Hello, World!\n";

    ActiasHandle handle;
    auto fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    assert(fileHandleResult == ACTIAS_SUCCESS);

    auto writeResult = ActiasWrite(handle, message, sizeof(message), nullptr);
    assert(writeResult == ACTIAS_SUCCESS);
}
