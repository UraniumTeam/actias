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

ActiasResult ActiasVirtualAlloc(void* pAddress, USize byteSize, Int32 protection)
{
    mmap(pAddress, byteSize, protection, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (errno != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }
    return ACTIAS_SUCCESS;
}

ActiasResult ActiasVirtualFree(void* pAddress, USize byteSize)
{
    munmap(pAddress, byteSize);
    if (errno != 0)
    {
        return ACTIAS_FAIL_UNKNOWN;
    }
    return ACTIAS_SUCCESS;
}