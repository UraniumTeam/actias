#include <Actias/System/Memory.h>
#include <stdlib.h>

void* ActiasAlignedAlloc(USize byteAlignment, USize byteSize)
{
    return ::memalign(byteAlignment, byteSize);
}

void ActiasAlignedFree(void* pointer)
{
    free(pointer);
}
