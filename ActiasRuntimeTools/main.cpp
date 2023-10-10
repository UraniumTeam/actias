#include <Actias/System/Memory.h>
#include <iostream>

int main()
{
    void* memory = ActiasAlignedAlloc(16, 1024);
    ActiasAlignedFree(memory);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
