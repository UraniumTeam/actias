#include <Actias/System/Streams.h>

int main()
{
    char message[] = "Hello, World!";

    ActiasHandle handle;
    ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);

    ActiasWrite(handle, message, sizeof(message), nullptr);
}
