#include <Actias/System/Memory.h>
#include <stdlib.h>

void* ActiasAlignedAlloc(USize byteAlignment, USize byteSize)
{
    return _aligned_malloc(byteSize, byteAlignment);
}

void ActiasAlignedFree(void* pointer)
{
    _aligned_free(pointer);
}
