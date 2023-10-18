#include <Actias/System/Streams.h>
#include <cassert>

int main()
{
    char message[] = "Hello, World!";

    ActiasHandle handle;
    auto fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    assert(fileHandleResult == ACTIAS_SUCCESS);

    auto writeResult = ActiasWrite(handle, message, sizeof(message), nullptr);
    assert(writeResult == ACTIAS_SUCCESS);
}
