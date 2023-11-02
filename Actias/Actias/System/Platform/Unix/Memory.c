#include <Actias/System/Memory.h>
#include <stdlib.h>
#include <sys/mman.h>

void* ActiasAlignedAlloc(USize byteAlignment, USize byteSize)
{
    return ::memalign(byteAlignment, byteSize);
}

void ActiasAlignedFree(void* pointer)
{
    free(pointer);
}

void* ActiasVirtualAlloc(void* pAddress, USize byteSize, Int32 protection)
{
    return mmap(pAddress, byteSize, protection, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

ActiasResult ActiasVirtualFree(void* pAddress, USize byteSize)
{
    return munmap(pAddress, byteSize);
}